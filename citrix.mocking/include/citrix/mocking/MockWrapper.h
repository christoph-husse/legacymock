
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _CITRIX_MOCKING_MOCKWRAPPER_H_
#define _CITRIX_MOCKING_MOCKWRAPPER_H_

#include <boost/scoped_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/static_assert.hpp>
#include <boost/preprocessor/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <set>

#include "citrix/SystemPreincludeFixes.h"
#include "citrix/Config.h"

#include "MockManager.h"

namespace citrix { namespace mocking { 
    
    namespace internal {
        template<class T, class TStaticImpl, class TInstanceImpl>
        class MockRepositoryBase : public IMockRepository<T> {
        private:
            typename TStaticImpl mockStatic;
            typename TInstanceImpl mockDefault;
            std::map<T*, TMockInstance*> premappedMocks;
            std::list<TMockInstance*> lazyMocks;

        public:
            typedef typename MockTraits<T>::TOriginal TOriginal;
            typedef typename MockTraits<T>::TStaticMock TStaticMock;
            typedef typename MockTraits<T>::TMockInstance TMockInstance;

            virtual bool wereLazyMocksConsumed() const {
                return lazyMocks.empty();
            }

            virtual void deactivateManager() {
                MockManager<T>::get()->deactivate();
            }

            virtual bool isMemberMockEnabled(void* memberAddress) const {
                return MockFixtureBase::isMemberMockEnabled(memberAddress);
            }

            virtual TMockInstance* getInstanceMock(void* thisPtr) {
                T* const baseThis = static_cast<T*>(thisPtr);
                std::map<T*, TMockInstance*>::iterator it = premappedMocks.lower_bound(baseThis);

                if ((it == premappedMocks.end()) || (it->first != baseThis)) {

                    /*
                        Getting here means we have not found any exact match of object pointer versus
                        registered mock. In most cases this means we simply have not yet associated
                        a mock with this instance. But there is a twist. When you use multi-inheritance
                        in C++, covariant casts may shift the "this" pointer. If then, the base class
                        a method was invoked through, does not implement the call (overridden, abstract, etc.)
                        hooking will lead us to the more derived type that provides the actual implementation.
                        Unfortunately, the "this" pointer is still shifted and points into the base class.
                        We need to resolve this by checking if the right-most registered instance, that
                        is smaller than our instance pointer extends over it. If it does, this means that
                        the current instance is just a shifted "this" pointer, belonging to the more derived
                        mock that is already registered. And since this is the one that already implements
                        the method, we can just return it's mock instead.
                    */
                    if((it != premappedMocks.begin()) && !premappedMocks.empty()) {
                        // get potentially derived "this"
                        it--;
                        T* const derivedThis = it->first;

                        // TODO: what is with member variables?

                        // see if its really derived
                        unsigned char* derivedExtend = (unsigned char*)derivedThis + sizeof(T);
                        if(derivedExtend > (unsigned char*)baseThis) {
                            return it->second;
                        }
                    }

                    if(lazyMocks.empty()) {
                        TInstanceImpl* default = getDefaultMock();

                        if(!default)
                            throw "No further instance mocks available.";

                        return default;
                    }

                    TMockInstance* mock = lazyMocks.front();
                    lazyMocks.pop_front();

                    premappedMocks.insert(std::make_pair(static_cast<T*>(baseThis), mock));

                    return mock;
                } else {
                    return it->second;
                }
            }

            virtual TInstanceImpl* getDefaultMock() {
                return &mockDefault;
            }

            virtual TStaticImpl* getStaticMock() {
                return &mockStatic;
            }

            virtual void unlinkMock(TMockInstance* gmock) {
                CITRIX_MOCK_UNREFPARAM(gmock);
            }

            void unlinkLazyMock(TMockInstance* gmock) {
                for(std::list<TMockInstance*>::iterator it = lazyMocks.begin(); it != lazyMocks.end(); it++) {
                    if(*it == gmock) {
                        lazyMocks.erase(it);
                        return;
                    }
                }
            }

            virtual void linkMockToLazyInstance(TMockInstance* gmock) {
                lazyMocks.push_back(gmock);
            }

            virtual boost::shared_ptr<TOriginal> linkMockToNewInstance(TMockInstance* gmock, boost::shared_ptr<TOriginal> instance) {
                premappedMocks.insert(std::make_pair(instance.get(), gmock));
                return instance;
            }
        };

        template<class T, class TStaticImpl, class TInstanceImpl>
        class MockRepositoryNewGuard {
        private:
            MockRepositoryBase<T, TStaticImpl, TInstanceImpl>* repo;
            TInstanceImpl* mock;
        public:
            MockRepositoryNewGuard(
                    MockRepositoryBase<T, TStaticImpl, TInstanceImpl>* repo, 
                    TInstanceImpl* mock) : repo(repo), mock(mock) {
                repo->linkMockToLazyInstance(mock);
            }

            ~MockRepositoryNewGuard() {
                repo->unlinkLazyMock(mock);
            }
        };

        class MockFixtureBase {
            template<typename, typename, typename> friend class MockRepoAccessor;
            template<typename> friend class DisableMemberMockGuard;
        public:

            MockFixtureBase() {
                if(getInstance()) {
                    throw "A fixture is already active in the current thread.";
                }

                setInstance(this);
            }

            ~MockFixtureBase() {
            #if _HAS_EXCEPTIONS
                try {
            #endif
                    for(size_t i = 0; i < activeRepos.size(); i++) {
                        activeRepos[i]->deactivateManager();
                    }
                    activeRepos.clear();

                    setInstance(NULL);
            #if _HAS_EXCEPTIONS
                } catch(...) {
                    setInstance(NULL);
                    throw;
                }
            #endif
            }

            template<class TMethod>
            static bool isMemberMockEnabled(TMethod memberAddress) {
                return isMemberMockEnabled(*((void**)&memberAddress));
            }

            static bool isMemberMockEnabled(void* memberAddress) {
                if(memberAddress == NULL) {
                    return true;
                }

                return getInstance()->disabledMembers.find(memberAddress) == 
                    getInstance()->disabledMembers.end();
            }

            template<class TMethod>
            void enableMemberMock(TMethod memberAddress) {
                enableMemberMock(*((void**)&memberAddress));
            }

            void enableMemberMock(void* memberAddress) {
                if(isMemberMockEnabled(memberAddress)) {
                    return;
                }

                disabledMembers.erase(memberAddress);
            }

            template<class TMethod>
            void disableMemberMock(TMethod memberAddress) {
                disableMemberMock(*((void**)&memberAddress));
            }

            void disableMemberMock(void* memberAddress) {
                if(!isMemberMockEnabled(memberAddress)) {
                    return;
                }

                disabledMembers.insert(memberAddress);
            }

            bool wereLazyMocksConsumed() {
                for(size_t i = 0; i < activeRepos.size(); i++) {
                    if(!activeRepos[i]->wereLazyMocksConsumed())
                        return false;
                }

                return true;
            }

        private:
            std::set<void*> disabledMembers;
            std::vector<boost::shared_ptr<IUntypedMockRepository>> activeRepos;

            template<class T, class TRepository>
            static TRepository* getRepo() {
                static boost::weak_ptr<TRepository> repoCache;
                boost::shared_ptr<TRepository> repo = repoCache.lock();

                if(!repo) {
                    if(!getInstance()) {
                        throw "There is no fixture associated with the current thread.";
                    }

                    repo = boost::shared_ptr<TRepository>(new TRepository());
                    if(!MockManager<T>::get()->tryActivate(repo)) {
                        throw "Not all hooks could be installed.";
                    }

                    repoCache = repo;
                    getInstance()->activeRepos.push_back(repo);
                }

                return repo.get();
            }

            static void setInstance(MockFixtureBase* newInstance) {
                manageInstance(true, newInstance);
            }

            static MockFixtureBase* getInstance() {
                return manageInstance(false);
            }

            static MockFixtureBase* manageInstance(bool doSet = false, MockFixtureBase* newInstance = NULL) {
                // this vodoo magic avoid having a C++ file just for defining a static class variable...
                static MockFixtureBase* cache = NULL;
        
                if(doSet) {
                    cache = newInstance;
                }

                return cache;
            }

            MockFixtureBase(const MockFixtureBase&);
            MockFixtureBase& operator=(const MockFixtureBase&);
        };

        template<class T, class TStaticImpl, class TInstanceImpl>
        class MockRepoAccessor {
        protected:
            typedef MockRepositoryBase<T, TStaticImpl, TInstanceImpl> TRepo;

            static TRepo* getRepo() {
                return MockFixtureBase::getRepo<T, TRepo>();
            }
        };

        // Gets bound to an instance if code under test creates one. Can't be passed to code under test.
        template<class T, class TStaticImpl, class TInstanceImpl>
        class LazyMockBase : public TInstanceImpl, MockRepoAccessor<T, TStaticImpl, TInstanceImpl> {
        private:
            typedef MockRepositoryBase<T, TStaticImpl, TInstanceImpl> TRepo;

            LazyMockBase(const LazyMockBase&);
            LazyMockBase& operator=(const LazyMockBase&);

        public:
            virtual ~LazyMockBase() {
                getRepo()->unlinkMock(this);
            }

            LazyMockBase() {
                getRepo()->linkMockToLazyInstance(this);
            }
        };

        // Is bound to an instance immediately and can be passed to code under test
        template<class T, class TStaticImpl, class TInstanceImpl>
        class MockBase : public TInstanceImpl, MockRepoAccessor<T, TStaticImpl, TInstanceImpl> {
        private:
            typedef MockTraits<T> TMock;
  
            boost::shared_ptr<T> instance;

            MockBase(const MockBase&);
            MockBase& operator=(const MockBase&);

            boost::shared_ptr<T> make_instance(MockRepositoryBase<T, TStaticImpl, TInstanceImpl>* repo) {
                MockRepositoryNewGuard<T, TStaticImpl, TInstanceImpl> lock(repo, this);
                return boost::shared_ptr<typename TMock::TProbingInstance>(new typename TMock::TProbingInstance());
            }

            #define BOOST_PP_LOCAL_MACRO(N)                                                 \
                template <BOOST_PP_ENUM_PARAMS(N, class A)>                                 \
                boost::shared_ptr<T> make_instance(                                         \
                        MockRepositoryBase<T, TStaticImpl, TInstanceImpl>* repo,            \
                        BOOST_PP_ENUM_BINARY_PARAMS(N, A, a)) {                             \
                    MockRepositoryNewGuard<T, TStaticImpl, TInstanceImpl> lock(repo, this); \
                    return boost::shared_ptr<typename TMock::TProbingInstance>(             \
                        new typename TMock::TProbingInstance(BOOST_PP_ENUM_PARAMS(N, a)));  \
                }

            #define BOOST_PP_LOCAL_LIMITS (1, LEGACYMOCK_MAX_VARIADIC_ARG_COUNT)
            #include BOOST_PP_LOCAL_ITERATE()

        public:

            ~MockBase() {
                getRepo()->unlinkMock(this);
            }

            MockBase() {
                instance = getRepo()->linkMockToNewInstance(this, make_instance(getRepo()));
            }

            #define BOOST_PP_LOCAL_MACRO(N)                                                                                     \
                template <BOOST_PP_ENUM_PARAMS(N, class A)>                                                                     \
                MockBase(BOOST_PP_ENUM_BINARY_PARAMS(N, A, a)) {                                                                \
                    instance = getRepo()->linkMockToNewInstance(this, make_instance(getRepo(), BOOST_PP_ENUM_PARAMS(N, a)));    \
                }

            #define BOOST_PP_LOCAL_LIMITS (1, LEGACYMOCK_MAX_VARIADIC_ARG_COUNT)
            #include BOOST_PP_LOCAL_ITERATE()

            operator T*() {
                return instance.get();
            }

            T* operator ->() {
                return instance.get();
            }

            operator T&() {
                return *instance;
            }

            operator boost::shared_ptr<T>() {
                return instance;
            }
        };

        template<class T, class TStaticImpl, class TInstanceImpl>
        struct StaticMockBaseAccessor : MockRepoAccessor<T, TStaticImpl, TInstanceImpl> {
            static TStaticImpl& get() {
                return *getRepo()->getStaticMock();
            }
        };

        template<class T, class TStaticImpl, class TInstanceImpl>
        TStaticImpl& StaticMockBase() {
            return StaticMockBaseAccessor<T, TStaticImpl, TInstanceImpl>::get();
        }

        template<class T, class TStaticImpl, class TInstanceImpl>
        struct DefaultMockBaseAccessor : MockRepoAccessor<T, TStaticImpl, TInstanceImpl> {
            static TInstanceImpl& get() {
                return *getRepo()->getDefaultMock();
            }
        };

        template<class T, class TStaticImpl, class TInstanceImpl>
        TInstanceImpl& DefaultMockBase() {
            return DefaultMockBaseAccessor<T, TStaticImpl, TInstanceImpl>::get();
        }

        // Disables a method mock within a scope.
        template<class TMethod>
        class DisableMemberMockGuard {
        private:
            void* methodAddr;
            MockFixtureBase* fixture;
            bool wasDisabled;

        public:
            virtual ~DisableMemberMockGuard() {
                if(!wasDisabled) {
                    fixture->enableMemberMock(methodAddr);
                }
            }

            DisableMemberMockGuard(TMethod method) :
                    methodAddr(*((void**)&method)),
                    fixture(MockFixtureBase::getInstance()),
                    wasDisabled(!MockFixtureBase::isMemberMockEnabled(methodAddr)) 
            {
                if(!wasDisabled) {
                    fixture->disableMemberMock(methodAddr);
                }
            }
        };
    }

    template<template<class> class TContext>
    class MockWrapper {
    private:
        MockWrapper();
        ~MockWrapper();
    public:

        template<class T>
        class MockRepository : public internal::MockRepositoryBase<T, typename TContext<T>::TStatic, typename TContext<T>::TInstance> 
        {
        };

        class MockFixture : public internal::MockFixtureBase { };

        template<class T>
        class Mock : public internal::MockBase<T, typename TContext<T>::TStatic, typename TContext<T>::TInstance> 
        {
        public:
            Mock() {}

            #define BOOST_PP_LOCAL_MACRO(N)                         \
                template <BOOST_PP_ENUM_PARAMS(N, class A)>         \
                Mock(BOOST_PP_ENUM_BINARY_PARAMS(N, A, a)) :        \
                    internal::MockBase<                             \
                            T,                                      \
                            typename TContext<T>::TStatic,          \
                            typename TContext<T>::TInstance>        \
                        (BOOST_PP_ENUM_PARAMS(N, a)) {              \
                }

            #define BOOST_PP_LOCAL_LIMITS (1, LEGACYMOCK_MAX_VARIADIC_ARG_COUNT)
            #include BOOST_PP_LOCAL_ITERATE()
        };

        template<class T>
        class LazyMock : public internal::LazyMockBase<T, typename TContext<T>::TStatic, typename TContext<T>::TInstance> 
        {
        };

        template<class T>
        class Accessor : public internal::MockTraits<T>::TAccessor 
        {
        };

        // Gets adhoc-bound to an instance if there is not other mock available or already bound.
        template<class T>
        static typename TContext<T>::TInstance& DefaultMock() {
            return internal::DefaultMockBase<T, typename TContext<T>::TStatic, typename TContext<T>::TInstance>();
        }

        template<class T>
        static typename TContext<T>::TStatic& StaticMock() {
            return internal::StaticMockBase<T, typename TContext<T>::TStatic, typename TContext<T>::TInstance>();
        }
    };

}}

#endif