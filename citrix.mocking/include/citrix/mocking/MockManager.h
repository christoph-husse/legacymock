
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _CITRIX_MOCKING_MOCKMANAGER_H_
#define _CITRIX_MOCKING_MOCKMANAGER_H_

#include "citrix/SystemPreincludeFixes.h"
#include "citrix/Config.h"

#include <boost/shared_ptr.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/utility/enable_if.hpp>
#include <list>
#include <map>


template<typename T> class CitrixMockingAccessor;

namespace citrix { namespace mocking { namespace internal {

/*
    If you get an error here this likely means that the class that needs to be
    allocated internally has no public default constructor. Please specialize
    this template appropiately to return a "default" instanciation as you like and
    make sure that you include the definition inside the generated mock by passing
    the associated header file via "-inc" to the generator.

    Another case are static classes. They can't be constructed by design, so you
    need to tell the generator "--pure-static-class" to prevent it from emitting
    instanciation code.

    Yet another case are abstract classes. In this case you can either use "--set-probing-instance"
    and specify the class that should be used instead for mock probing (this depends
    on the error you get and may not work for your case). Or you can specialize the
    template and instance a non-abstract derived version of the required type and return 
    it via covariant shared_ptr cast.

    ATTENTION: Don't use make_shared or boost local iteration here because it would
    obscure compiler errors, and we want user of our lib to get directly to the above help
    message!
*/
template<class T> boost::shared_ptr<T> allocateDefaultInstance() 
    { return boost::shared_ptr<T>(new T()); }
template<class T, class P1> boost::shared_ptr<T> allocateDefaultInstance(P1& p1) 
    { return boost::shared_ptr<T>(new T(p1)); }
template<class T, class P1, class P2> boost::shared_ptr<T> allocateDefaultInstance(P1& p1, P2& p2) 
    { return boost::shared_ptr<T>(new T(p1, p2)); }
template<class T, class P1, class P2, class P3> boost::shared_ptr<T> allocateDefaultInstance(P1& p1, P2& p2, P3& p3) 
    { return boost::shared_ptr<T>(new T(p1, p2, p3)); }
template<class T, class P1, class P2, class P3, class P4> boost::shared_ptr<T> allocateDefaultInstance(P1& p1, P2& p2, P3& p3, P4& p4) 
    { return boost::shared_ptr<T>(new T(p1, p2, p3, p4)); }
template<class T, class P1, class P2, class P3, class P4, class P5> boost::shared_ptr<T> allocateDefaultInstance(P1& p1, P2& p2, P3& p3, P4& p4, P5& p5) 
    { return boost::shared_ptr<T>(new T(p1, p2, p3, p4, p5)); }

template<class T> 
static typename boost::disable_if<
    boost::is_reference<T>,
    typename boost::add_reference<T>::type 
>::type allocateNullInstance() {
    static boost::shared_ptr<T> backingValue = allocateDefaultInstance<T>();
    return *backingValue;
}

template<class T> 
static typename boost::enable_if<
    boost::is_reference<T>,
    T
>::type allocateNullInstance() {
    return *((boost::remove_reference<T>::type*)NULL);
}

template<> 
static void allocateNullInstance<void>() { }

template<class T>
struct MockInstance;

template<class T>
struct MockTraits {
    BOOST_STATIC_CONSTANT(bool, isAvailable = false);
    BOOST_STATIC_CONSTANT(bool, hasGMock = false);
    BOOST_STATIC_CONSTANT(bool, hasTracer = false);
};

template<class T>
bool tryToInstallHooks();

class IUntypedMockRepository {
public:
    virtual ~IUntypedMockRepository() { }

    virtual bool wereLazyMocksConsumed() const = 0;
    virtual void deactivateManager() = 0;
};

template<class T>
class IMockRepository : public IUntypedMockRepository {
public:
    typedef typename MockTraits<T>::TOriginal TOriginal;
    typedef typename MockTraits<T>::TStaticMock TStaticMock;
    typedef typename MockTraits<T>::TMockInstance TMockInstance;

    virtual TMockInstance* getInstanceMock(void* thisPtr) = 0;
    virtual TStaticMock* getStaticMock() = 0;
    virtual bool isMemberMockEnabled(void* memberAddress) const = 0;
    virtual void unlinkMock(TMockInstance* gmock) = 0;
    virtual void linkMockToLazyInstance(TMockInstance* gmock) = 0;
    virtual boost::shared_ptr<TOriginal> linkMockToNewInstance(TMockInstance* gmock, boost::shared_ptr<TOriginal> instance) = 0;
};


template<class T>
class MockManagerAccessor;


template<class T>
class MockManager {
public:

    friend class MockManagerAccessor<T>;

    typedef typename MockTraits<T>::TOriginal TOriginal;
    typedef typename MockTraits<T>::TStaticMock TStaticMock;
    typedef typename MockTraits<T>::TMockInstance TMockInstance;
    typedef MockManager<T> TManager;

    static boost::shared_ptr<TManager> get() {
        static boost::shared_ptr<TManager> instance = boost::shared_ptr<TManager>(new TManager());
        return instance;
    }
    
    template<class T>
    bool isActive(T memberAddress = NULL) const {
        return isActive(*((void**)&memberAddress));
    }

    bool isActive(void* memberAddress = NULL) const {
        return !!currentRepo && currentRepo->isMemberMockEnabled(memberAddress);
    }

    bool tryActivate(boost::shared_ptr<IMockRepository<T>> repo) {
        deactivate();

        if(!tryToInstallHooks<T>()) {
            return false;
        }

        currentRepo = repo;
        return true;
    }

    void deactivate() {
        currentRepo.reset();
    }

    MockManager() {
    }

private:
    boost::shared_ptr<IMockRepository<T>> currentRepo;

    TMockInstance* resolveMockForPointer(void* thisPtr) {
        return currentRepo->getInstanceMock(thisPtr);
    }

    TStaticMock* resolveStaticMock() {
        return currentRepo->getStaticMock();
    }
  
    MockManager(const MockManager&);
    MockManager& operator=(const MockManager&);
};


template<class T>
class MockManagerAccessor {
protected:
    typedef typename MockTraits<T>::TStaticMock TStaticMock;
    typedef typename MockTraits<T>::TMockInstance TMockInstance;

    static TMockInstance* resolveMockForPointer(void* thisPtr) {
        return MockManager<T>::get()->resolveMockForPointer(thisPtr);
    }

    static TStaticMock* resolveStaticMock() {
        return MockManager<T>::get()->resolveStaticMock();
    }
};

}}}

#endif