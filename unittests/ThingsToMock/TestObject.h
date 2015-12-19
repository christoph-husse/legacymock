// This file was automatically generated! 

#ifndef _UNITTEST_MOCK_GENERATED_TESTOBJECT_H_
#define _UNITTEST_MOCK_GENERATED_TESTOBJECT_H_

#include "citrix/Config.h"
#include "citrix/Platform.h"
#include "citrix/type_traits/default_constructor.hpp"
#include "CustomTypesForTesting.h"


template<class T> class CitrixMockingAccessor;

struct TestObject {
template<class T> friend class ::CitrixMockingAccessor;
public:
    void void___charPtr (char* param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        void___charPtr__COUNTER__++;
    }

    void void___bool (bool param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        forceAssign(param0, false);
        void___bool__COUNTER__++;
    }

    void void___int8_t (int8_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        forceAssign(param0, -23);
        void___int8_t__COUNTER__++;
    }

    void void___int16_t (int16_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        forceAssign(param0, -3445);
        void___int16_t__COUNTER__++;
    }

    void void___int32_t (int32_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        forceAssign(param0, 1422356763);
        void___int32_t__COUNTER__++;
    }

    void void___int64_t (int64_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        forceAssign(param0, 3297109850394757343LL);
        void___int64_t__COUNTER__++;
    }

    void void___uint8_t (uint8_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        forceAssign(param0, 233);
        void___uint8_t__COUNTER__++;
    }

    void void___stdstring (std::string param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        forceAssign(param0, "I was a String in Theory!");
        void___stdstring__COUNTER__++;
    }

    void void___uint32_t (uint32_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        forceAssign(param0, 3564677783);
        void___uint32_t__COUNTER__++;
    }

    void void___uint64_t (uint64_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        forceAssign(param0, 2534545768788782938ULL);
        void___uint64_t__COUNTER__++;
    }

    void void___float (float param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        forceAssign(param0, -2.3e-8f);
        void___float__COUNTER__++;
    }

    void void___constMyOwnStructRef (const MyOwnStruct& param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        forceAssign(param0, storageForUpdated_MyOwnStruct);
        void___constMyOwnStructRef__COUNTER__++;
    }

    char* charPtr___ () { 
        charPtr_____COUNTER__++;
        return "Hello World!";
    }

    bool bool___ () { 
        bool_____COUNTER__++;
        return true;
    }

    int8_t int8_t___ () { 
        int8_t_____COUNTER__++;
        return 124;
    }

    int16_t int16_t___ () { 
        int16_t_____COUNTER__++;
        return 1236;
    }

    int32_t int32_t___ () { 
        int32_t_____COUNTER__++;
        return -1423;
    }

    int64_t int64_t___ () { 
        int64_t_____COUNTER__++;
        return -1235345345345345343LL;
    }

    uint8_t uint8_t___ () { 
        uint8_t_____COUNTER__++;
        return 143;
    }

    std::string stdstring___ () { 
        stdstring_____COUNTER__++;
        return "I am a String in Theory!";
    }

    uint32_t uint32_t___ () { 
        uint32_t_____COUNTER__++;
        return 1883345323;
    }

    uint64_t uint64_t___ () { 
        uint64_t_____COUNTER__++;
        return 1237429349238472938ULL;
    }

    float float___ () { 
        float_____COUNTER__++;
        return +2.3e-8f;
    }

    MyOwnStruct& MyOwnStructRef___ () { 
        MyOwnStructRef_____COUNTER__++;
        return storageForInitial_MyOwnStruct;
    }

    const MyOwnStruct& constMyOwnStructRef___ () { 
        constMyOwnStructRef_____COUNTER__++;
        return storageForInitial_MyOwnStruct;
    }

    const MyOwnStruct* constMyOwnStructPtr___ () { 
        constMyOwnStructPtr_____COUNTER__++;
        return &storageForInitial_MyOwnStruct;
    }

    MyOwnStruct* MyOwnStructPtr___ () { 
        MyOwnStructPtr_____COUNTER__++;
        return &storageForInitial_MyOwnStruct;
    }

    bool bool___constMyOwnStructRef__stdstringRef (const MyOwnStruct& param0, std::string& param1) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        CITRIX_MOCK_UNREFPARAM(param1);
        forceAssign(param0, storageForUpdated_MyOwnStruct);
        forceAssign(param1, storageForUpdated_stdstring);
        bool___constMyOwnStructRef__stdstringRef__COUNTER__++;
        return true;
    }

    int8_t int8_t___MyOwnStructRef__conststdstringRef (MyOwnStruct& param0, const std::string& param1) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        CITRIX_MOCK_UNREFPARAM(param1);
        forceAssign(param0, storageForUpdated_MyOwnStruct);
        forceAssign(param1, storageForUpdated_stdstring);
        int8_t___MyOwnStructRef__conststdstringRef__COUNTER__++;
        return 124;
    }

    int16_t int16_t___constMyOwnStructPtr__stdstringPtr (const MyOwnStruct* param0, std::string* param1) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        CITRIX_MOCK_UNREFPARAM(param1);
        forceAssign(param0, &storageForUpdated_MyOwnStruct);
        forceAssign(param1, &storageForUpdated_stdstring);
        int16_t___constMyOwnStructPtr__stdstringPtr__COUNTER__++;
        return 1236;
    }

    int32_t int32_t___MyOwnStructPtr__conststdstringPtr (MyOwnStruct* param0, const std::string* param1) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        CITRIX_MOCK_UNREFPARAM(param1);
        forceAssign(param0, &storageForUpdated_MyOwnStruct);
        forceAssign(param1, &storageForUpdated_stdstring);
        int32_t___MyOwnStructPtr__conststdstringPtr__COUNTER__++;
        return -1423;
    }

    const char* constcharPtr___MyOwnStructPtrconst (MyOwnStruct* const param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        forceAssign(param0, &storageForUpdated_MyOwnStruct);
        constcharPtr___MyOwnStructPtrconst__COUNTER__++;
        return "Hello World!";
    }

    char* charPtr___constMyOwnStructPtrconst (const MyOwnStruct* const param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        forceAssign(param0, &storageForUpdated_MyOwnStruct);
        charPtr___constMyOwnStructPtrconst__COUNTER__++;
        return "Hello World!";
    }

    std::string stdstring___constuint32_tPtr (const uint32_t* param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        forceAssign(param0, &storageForUpdated_uint32_t);
        stdstring___constuint32_tPtr__COUNTER__++;
        return "I am a String in Theory!";
    }

    uint32_t uint32_t___constuint64_tRef (const uint64_t& param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        forceAssign(param0, storageForUpdated_uint64_t);
        uint32_t___constuint64_tRef__COUNTER__++;
        return 1883345323;
    }

    uint64_t uint64_t___boolPtr (bool* param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        forceAssign(param0, &storageForUpdated_bool);
        uint64_t___boolPtr__COUNTER__++;
        return 1237429349238472938ULL;
    }

    float float___constfloatRef (const float& param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        forceAssign(param0, storageForUpdated_float);
        float___constfloatRef__COUNTER__++;
        return +2.3e-8f;
    }

    static void S_void___charPtr (char* param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_void___bool (bool param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_void___int8_t (int8_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_void___int16_t (int16_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_void___int32_t (int32_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_void___int64_t (int64_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_void___uint8_t (uint8_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_void___stdstring (std::string param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_void___uint32_t (uint32_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_void___uint64_t (uint64_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_void___float (float param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_void___constMyOwnStructRef (const MyOwnStruct& param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static char* S_charPtr___ () { 
        return ::citrix::type_traits::default_constructor<char*>::get();
    }

    static bool S_bool___ () { 
        return ::citrix::type_traits::default_constructor<bool>::get();
    }

    static int8_t S_int8_t___ () { 
        return ::citrix::type_traits::default_constructor<int8_t>::get();
    }

    static int16_t S_int16_t___ () { 
        return ::citrix::type_traits::default_constructor<int16_t>::get();
    }

    static int32_t S_int32_t___ () { 
        return ::citrix::type_traits::default_constructor<int32_t>::get();
    }

    static int64_t S_int64_t___ () { 
        return ::citrix::type_traits::default_constructor<int64_t>::get();
    }

    static uint8_t S_uint8_t___ () { 
        return ::citrix::type_traits::default_constructor<uint8_t>::get();
    }

    static std::string S_stdstring___ () { 
        return ::citrix::type_traits::default_constructor<std::string>::get();
    }

    static uint32_t S_uint32_t___ () { 
        return ::citrix::type_traits::default_constructor<uint32_t>::get();
    }

    static uint64_t S_uint64_t___ () { 
        return ::citrix::type_traits::default_constructor<uint64_t>::get();
    }

    static float S_float___ () { 
        return ::citrix::type_traits::default_constructor<float>::get();
    }

    static MyOwnStruct& S_MyOwnStructRef___ () { 
        return ::citrix::type_traits::default_constructor<MyOwnStruct&>::get();
    }

    static const MyOwnStruct& S_constMyOwnStructRef___ () { 
        return ::citrix::type_traits::default_constructor<const MyOwnStruct&>::get();
    }

    static const MyOwnStruct* S_constMyOwnStructPtr___ () { 
        return ::citrix::type_traits::default_constructor<const MyOwnStruct*>::get();
    }

    static MyOwnStruct* S_MyOwnStructPtr___ () { 
        return ::citrix::type_traits::default_constructor<MyOwnStruct*>::get();
    }

    static bool S_bool___constMyOwnStructRef__stdstringRef (const MyOwnStruct& param0, std::string& param1) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        CITRIX_MOCK_UNREFPARAM(param1);
        return ::citrix::type_traits::default_constructor<bool>::get();
    }

    static int8_t S_int8_t___MyOwnStructRef__conststdstringRef (MyOwnStruct& param0, const std::string& param1) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        CITRIX_MOCK_UNREFPARAM(param1);
        return ::citrix::type_traits::default_constructor<int8_t>::get();
    }

    static int16_t S_int16_t___constMyOwnStructPtr__stdstringPtr (const MyOwnStruct* param0, std::string* param1) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        CITRIX_MOCK_UNREFPARAM(param1);
        return ::citrix::type_traits::default_constructor<int16_t>::get();
    }

    static int32_t S_int32_t___MyOwnStructPtr__conststdstringPtr (MyOwnStruct* param0, const std::string* param1) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        CITRIX_MOCK_UNREFPARAM(param1);
        return ::citrix::type_traits::default_constructor<int32_t>::get();
    }

    static const char* S_constcharPtr___MyOwnStructPtrconst (MyOwnStruct* const param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        return ::citrix::type_traits::default_constructor<const char*>::get();
    }

    static char* S_charPtr___constMyOwnStructPtrconst (const MyOwnStruct* const param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        return ::citrix::type_traits::default_constructor<char*>::get();
    }

    static std::string S_stdstring___constuint32_tPtr (const uint32_t* param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        return ::citrix::type_traits::default_constructor<std::string>::get();
    }

    static uint32_t S_uint32_t___constuint64_tRef (const uint64_t& param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        return ::citrix::type_traits::default_constructor<uint32_t>::get();
    }

    static uint64_t S_uint64_t___boolPtr (bool* param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        return ::citrix::type_traits::default_constructor<uint64_t>::get();
    }

    static float S_float___constfloatRef (const float& param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        return ::citrix::type_traits::default_constructor<float>::get();
    }

protected:
    void protected_void___charPtr (char* param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    void protected_void___bool (bool param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    void protected_void___int8_t (int8_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    void protected_void___int16_t (int16_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    void protected_void___int32_t (int32_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    void protected_void___int64_t (int64_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    void protected_void___uint8_t (uint8_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    void protected_void___stdstring (std::string param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    void protected_void___uint32_t (uint32_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    void protected_void___uint64_t (uint64_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    void protected_void___float (float param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    void protected_void___constMyOwnStructRef (const MyOwnStruct& param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    char* protected_charPtr___ () { 
        return ::citrix::type_traits::default_constructor<char*>::get();
    }

    bool protected_bool___ () { 
        return ::citrix::type_traits::default_constructor<bool>::get();
    }

    int8_t protected_int8_t___ () { 
        return ::citrix::type_traits::default_constructor<int8_t>::get();
    }

    int16_t protected_int16_t___ () { 
        return ::citrix::type_traits::default_constructor<int16_t>::get();
    }

    int32_t protected_int32_t___ () { 
        return ::citrix::type_traits::default_constructor<int32_t>::get();
    }

    int64_t protected_int64_t___ () { 
        return ::citrix::type_traits::default_constructor<int64_t>::get();
    }

    uint8_t protected_uint8_t___ () { 
        return ::citrix::type_traits::default_constructor<uint8_t>::get();
    }

    std::string protected_stdstring___ () { 
        return ::citrix::type_traits::default_constructor<std::string>::get();
    }

    uint32_t protected_uint32_t___ () { 
        return ::citrix::type_traits::default_constructor<uint32_t>::get();
    }

    uint64_t protected_uint64_t___ () { 
        return ::citrix::type_traits::default_constructor<uint64_t>::get();
    }

    float protected_float___ () { 
        return ::citrix::type_traits::default_constructor<float>::get();
    }

    MyOwnStruct& protected_MyOwnStructRef___ () { 
        return ::citrix::type_traits::default_constructor<MyOwnStruct&>::get();
    }

    const MyOwnStruct& protected_constMyOwnStructRef___ () { 
        return ::citrix::type_traits::default_constructor<const MyOwnStruct&>::get();
    }

    const MyOwnStruct* protected_constMyOwnStructPtr___ () { 
        return ::citrix::type_traits::default_constructor<const MyOwnStruct*>::get();
    }

    MyOwnStruct* protected_MyOwnStructPtr___ () { 
        return ::citrix::type_traits::default_constructor<MyOwnStruct*>::get();
    }

    bool protected_bool___constMyOwnStructRef__stdstringRef (const MyOwnStruct& param0, std::string& param1) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        CITRIX_MOCK_UNREFPARAM(param1);
        return ::citrix::type_traits::default_constructor<bool>::get();
    }

    int8_t protected_int8_t___MyOwnStructRef__conststdstringRef (MyOwnStruct& param0, const std::string& param1) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        CITRIX_MOCK_UNREFPARAM(param1);
        return ::citrix::type_traits::default_constructor<int8_t>::get();
    }

    int16_t protected_int16_t___constMyOwnStructPtr__stdstringPtr (const MyOwnStruct* param0, std::string* param1) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        CITRIX_MOCK_UNREFPARAM(param1);
        return ::citrix::type_traits::default_constructor<int16_t>::get();
    }

    int32_t protected_int32_t___MyOwnStructPtr__conststdstringPtr (MyOwnStruct* param0, const std::string* param1) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        CITRIX_MOCK_UNREFPARAM(param1);
        return ::citrix::type_traits::default_constructor<int32_t>::get();
    }

    const char* protected_constcharPtr___MyOwnStructPtrconst (MyOwnStruct* const param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        return ::citrix::type_traits::default_constructor<const char*>::get();
    }

    char* protected_charPtr___constMyOwnStructPtrconst (const MyOwnStruct* const param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        return ::citrix::type_traits::default_constructor<char*>::get();
    }

    std::string protected_stdstring___constuint32_tPtr (const uint32_t* param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        return ::citrix::type_traits::default_constructor<std::string>::get();
    }

    uint32_t protected_uint32_t___constuint64_tRef (const uint64_t& param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        return ::citrix::type_traits::default_constructor<uint32_t>::get();
    }

    uint64_t protected_uint64_t___boolPtr (bool* param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        return ::citrix::type_traits::default_constructor<uint64_t>::get();
    }

    float protected_float___constfloatRef (const float& param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        return ::citrix::type_traits::default_constructor<float>::get();
    }

    static void S_protected_void___charPtr (char* param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_protected_void___bool (bool param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_protected_void___int8_t (int8_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_protected_void___int16_t (int16_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_protected_void___int32_t (int32_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_protected_void___int64_t (int64_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_protected_void___uint8_t (uint8_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_protected_void___stdstring (std::string param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_protected_void___uint32_t (uint32_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_protected_void___uint64_t (uint64_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_protected_void___float (float param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_protected_void___constMyOwnStructRef (const MyOwnStruct& param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static char* S_protected_charPtr___ () { 
        return ::citrix::type_traits::default_constructor<char*>::get();
    }

    static bool S_protected_bool___ () { 
        return ::citrix::type_traits::default_constructor<bool>::get();
    }

    static int8_t S_protected_int8_t___ () { 
        return ::citrix::type_traits::default_constructor<int8_t>::get();
    }

    static int16_t S_protected_int16_t___ () { 
        return ::citrix::type_traits::default_constructor<int16_t>::get();
    }

    static int32_t S_protected_int32_t___ () { 
        return ::citrix::type_traits::default_constructor<int32_t>::get();
    }

    static int64_t S_protected_int64_t___ () { 
        return ::citrix::type_traits::default_constructor<int64_t>::get();
    }

    static uint8_t S_protected_uint8_t___ () { 
        return ::citrix::type_traits::default_constructor<uint8_t>::get();
    }

    static std::string S_protected_stdstring___ () { 
        return ::citrix::type_traits::default_constructor<std::string>::get();
    }

    static uint32_t S_protected_uint32_t___ () { 
        return ::citrix::type_traits::default_constructor<uint32_t>::get();
    }

    static uint64_t S_protected_uint64_t___ () { 
        return ::citrix::type_traits::default_constructor<uint64_t>::get();
    }

    static float S_protected_float___ () { 
        return ::citrix::type_traits::default_constructor<float>::get();
    }

    static MyOwnStruct& S_protected_MyOwnStructRef___ () { 
        return ::citrix::type_traits::default_constructor<MyOwnStruct&>::get();
    }

    static const MyOwnStruct& S_protected_constMyOwnStructRef___ () { 
        return ::citrix::type_traits::default_constructor<const MyOwnStruct&>::get();
    }

    static const MyOwnStruct* S_protected_constMyOwnStructPtr___ () { 
        return ::citrix::type_traits::default_constructor<const MyOwnStruct*>::get();
    }

    static MyOwnStruct* S_protected_MyOwnStructPtr___ () { 
        return ::citrix::type_traits::default_constructor<MyOwnStruct*>::get();
    }

    static bool S_protected_bool___constMyOwnStructRef__stdstringRef (const MyOwnStruct& param0, std::string& param1) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        CITRIX_MOCK_UNREFPARAM(param1);
        return ::citrix::type_traits::default_constructor<bool>::get();
    }

    static int8_t S_protected_int8_t___MyOwnStructRef__conststdstringRef (MyOwnStruct& param0, const std::string& param1) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        CITRIX_MOCK_UNREFPARAM(param1);
        return ::citrix::type_traits::default_constructor<int8_t>::get();
    }

    static int16_t S_protected_int16_t___constMyOwnStructPtr__stdstringPtr (const MyOwnStruct* param0, std::string* param1) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        CITRIX_MOCK_UNREFPARAM(param1);
        return ::citrix::type_traits::default_constructor<int16_t>::get();
    }

    static int32_t S_protected_int32_t___MyOwnStructPtr__conststdstringPtr (MyOwnStruct* param0, const std::string* param1) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        CITRIX_MOCK_UNREFPARAM(param1);
        return ::citrix::type_traits::default_constructor<int32_t>::get();
    }

    static const char* S_protected_constcharPtr___MyOwnStructPtrconst (MyOwnStruct* const param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        return ::citrix::type_traits::default_constructor<const char*>::get();
    }

    static char* S_protected_charPtr___constMyOwnStructPtrconst (const MyOwnStruct* const param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        return ::citrix::type_traits::default_constructor<char*>::get();
    }

    static std::string S_protected_stdstring___constuint32_tPtr (const uint32_t* param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        return ::citrix::type_traits::default_constructor<std::string>::get();
    }

    static uint32_t S_protected_uint32_t___constuint64_tRef (const uint64_t& param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        return ::citrix::type_traits::default_constructor<uint32_t>::get();
    }

    static uint64_t S_protected_uint64_t___boolPtr (bool* param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        return ::citrix::type_traits::default_constructor<uint64_t>::get();
    }

    static float S_protected_float___constfloatRef (const float& param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        return ::citrix::type_traits::default_constructor<float>::get();
    }

private:
    void private_void___charPtr (char* param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    void private_void___bool (bool param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    void private_void___int8_t (int8_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    void private_void___int16_t (int16_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    void private_void___int32_t (int32_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    void private_void___int64_t (int64_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    void private_void___uint8_t (uint8_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    void private_void___stdstring (std::string param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    void private_void___uint32_t (uint32_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    void private_void___uint64_t (uint64_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    void private_void___float (float param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    void private_void___constMyOwnStructRef (const MyOwnStruct& param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    char* private_charPtr___ () { 
        return ::citrix::type_traits::default_constructor<char*>::get();
    }

    bool private_bool___ () { 
        return ::citrix::type_traits::default_constructor<bool>::get();
    }

    int8_t private_int8_t___ () { 
        return ::citrix::type_traits::default_constructor<int8_t>::get();
    }

    int16_t private_int16_t___ () { 
        return ::citrix::type_traits::default_constructor<int16_t>::get();
    }

    int32_t private_int32_t___ () { 
        return ::citrix::type_traits::default_constructor<int32_t>::get();
    }

    int64_t private_int64_t___ () { 
        return ::citrix::type_traits::default_constructor<int64_t>::get();
    }

    uint8_t private_uint8_t___ () { 
        return ::citrix::type_traits::default_constructor<uint8_t>::get();
    }

    std::string private_stdstring___ () { 
        return ::citrix::type_traits::default_constructor<std::string>::get();
    }

    uint32_t private_uint32_t___ () { 
        return ::citrix::type_traits::default_constructor<uint32_t>::get();
    }

    uint64_t private_uint64_t___ () { 
        return ::citrix::type_traits::default_constructor<uint64_t>::get();
    }

    float private_float___ () { 
        return ::citrix::type_traits::default_constructor<float>::get();
    }

    MyOwnStruct& private_MyOwnStructRef___ () { 
        return ::citrix::type_traits::default_constructor<MyOwnStruct&>::get();
    }

    const MyOwnStruct& private_constMyOwnStructRef___ () { 
        return ::citrix::type_traits::default_constructor<const MyOwnStruct&>::get();
    }

    const MyOwnStruct* private_constMyOwnStructPtr___ () { 
        return ::citrix::type_traits::default_constructor<const MyOwnStruct*>::get();
    }

    MyOwnStruct* private_MyOwnStructPtr___ () { 
        return ::citrix::type_traits::default_constructor<MyOwnStruct*>::get();
    }

    bool private_bool___constMyOwnStructRef__stdstringRef (const MyOwnStruct& param0, std::string& param1) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        CITRIX_MOCK_UNREFPARAM(param1);
        return ::citrix::type_traits::default_constructor<bool>::get();
    }

    int8_t private_int8_t___MyOwnStructRef__conststdstringRef (MyOwnStruct& param0, const std::string& param1) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        CITRIX_MOCK_UNREFPARAM(param1);
        return ::citrix::type_traits::default_constructor<int8_t>::get();
    }

    int16_t private_int16_t___constMyOwnStructPtr__stdstringPtr (const MyOwnStruct* param0, std::string* param1) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        CITRIX_MOCK_UNREFPARAM(param1);
        return ::citrix::type_traits::default_constructor<int16_t>::get();
    }

    int32_t private_int32_t___MyOwnStructPtr__conststdstringPtr (MyOwnStruct* param0, const std::string* param1) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        CITRIX_MOCK_UNREFPARAM(param1);
        return ::citrix::type_traits::default_constructor<int32_t>::get();
    }

    const char* private_constcharPtr___MyOwnStructPtrconst (MyOwnStruct* const param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        return ::citrix::type_traits::default_constructor<const char*>::get();
    }

    char* private_charPtr___constMyOwnStructPtrconst (const MyOwnStruct* const param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        return ::citrix::type_traits::default_constructor<char*>::get();
    }

    std::string private_stdstring___constuint32_tPtr (const uint32_t* param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        return ::citrix::type_traits::default_constructor<std::string>::get();
    }

    uint32_t private_uint32_t___constuint64_tRef (const uint64_t& param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        return ::citrix::type_traits::default_constructor<uint32_t>::get();
    }

    uint64_t private_uint64_t___boolPtr (bool* param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        return ::citrix::type_traits::default_constructor<uint64_t>::get();
    }

    float private_float___constfloatRef (const float& param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        return ::citrix::type_traits::default_constructor<float>::get();
    }

    static void S_private_void___charPtr (char* param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_private_void___bool (bool param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_private_void___int8_t (int8_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_private_void___int16_t (int16_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_private_void___int32_t (int32_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_private_void___int64_t (int64_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_private_void___uint8_t (uint8_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_private_void___stdstring (std::string param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_private_void___uint32_t (uint32_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_private_void___uint64_t (uint64_t param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_private_void___float (float param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static void S_private_void___constMyOwnStructRef (const MyOwnStruct& param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
    }

    static char* S_private_charPtr___ () { 
        return ::citrix::type_traits::default_constructor<char*>::get();
    }

    static bool S_private_bool___ () { 
        return ::citrix::type_traits::default_constructor<bool>::get();
    }

    static int8_t S_private_int8_t___ () { 
        return ::citrix::type_traits::default_constructor<int8_t>::get();
    }

    static int16_t S_private_int16_t___ () { 
        return ::citrix::type_traits::default_constructor<int16_t>::get();
    }

    static int32_t S_private_int32_t___ () { 
        return ::citrix::type_traits::default_constructor<int32_t>::get();
    }

    static int64_t S_private_int64_t___ () { 
        return ::citrix::type_traits::default_constructor<int64_t>::get();
    }

    static uint8_t S_private_uint8_t___ () { 
        return ::citrix::type_traits::default_constructor<uint8_t>::get();
    }

    static std::string S_private_stdstring___ () { 
        return ::citrix::type_traits::default_constructor<std::string>::get();
    }

    static uint32_t S_private_uint32_t___ () { 
        return ::citrix::type_traits::default_constructor<uint32_t>::get();
    }

    static uint64_t S_private_uint64_t___ () { 
        return ::citrix::type_traits::default_constructor<uint64_t>::get();
    }

    static float S_private_float___ () { 
        return ::citrix::type_traits::default_constructor<float>::get();
    }

    static MyOwnStruct& S_private_MyOwnStructRef___ () { 
        return ::citrix::type_traits::default_constructor<MyOwnStruct&>::get();
    }

    static const MyOwnStruct& S_private_constMyOwnStructRef___ () { 
        return ::citrix::type_traits::default_constructor<const MyOwnStruct&>::get();
    }

    static const MyOwnStruct* S_private_constMyOwnStructPtr___ () { 
        return ::citrix::type_traits::default_constructor<const MyOwnStruct*>::get();
    }

    static MyOwnStruct* S_private_MyOwnStructPtr___ () { 
        return ::citrix::type_traits::default_constructor<MyOwnStruct*>::get();
    }

    static bool S_private_bool___constMyOwnStructRef__stdstringRef (const MyOwnStruct& param0, std::string& param1) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        CITRIX_MOCK_UNREFPARAM(param1);
        return ::citrix::type_traits::default_constructor<bool>::get();
    }

    static int8_t S_private_int8_t___MyOwnStructRef__conststdstringRef (MyOwnStruct& param0, const std::string& param1) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        CITRIX_MOCK_UNREFPARAM(param1);
        return ::citrix::type_traits::default_constructor<int8_t>::get();
    }

    static int16_t S_private_int16_t___constMyOwnStructPtr__stdstringPtr (const MyOwnStruct* param0, std::string* param1) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        CITRIX_MOCK_UNREFPARAM(param1);
        return ::citrix::type_traits::default_constructor<int16_t>::get();
    }

    static int32_t S_private_int32_t___MyOwnStructPtr__conststdstringPtr (MyOwnStruct* param0, const std::string* param1) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        CITRIX_MOCK_UNREFPARAM(param1);
        return ::citrix::type_traits::default_constructor<int32_t>::get();
    }

    static const char* S_private_constcharPtr___MyOwnStructPtrconst (MyOwnStruct* const param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        return ::citrix::type_traits::default_constructor<const char*>::get();
    }

    static char* S_private_charPtr___constMyOwnStructPtrconst (const MyOwnStruct* const param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        return ::citrix::type_traits::default_constructor<char*>::get();
    }

    static std::string S_private_stdstring___constuint32_tPtr (const uint32_t* param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        return ::citrix::type_traits::default_constructor<std::string>::get();
    }

    static uint32_t S_private_uint32_t___constuint64_tRef (const uint64_t& param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        return ::citrix::type_traits::default_constructor<uint32_t>::get();
    }

    static uint64_t S_private_uint64_t___boolPtr (bool* param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        return ::citrix::type_traits::default_constructor<uint64_t>::get();
    }

    static float S_private_float___constfloatRef (const float& param0) { 
        CITRIX_MOCK_UNREFPARAM(param0);
        return ::citrix::type_traits::default_constructor<float>::get();
    }

public:
     TestObject(char* param0) : __constructor_Name__("void___charPtr"), storageForUpdated_MyOwnStruct(true) {
        CITRIX_MOCK_UNREFPARAM(param0);
        __init__internal__();
    }

     TestObject(bool param0) : __constructor_Name__("void___bool"), storageForUpdated_MyOwnStruct(true) {
        CITRIX_MOCK_UNREFPARAM(param0);
        __init__internal__();
    }

     TestObject(int8_t param0) : __constructor_Name__("void___int8_t"), storageForUpdated_MyOwnStruct(true) {
        CITRIX_MOCK_UNREFPARAM(param0);
        __init__internal__();
    }

     TestObject(int16_t param0) : __constructor_Name__("void___int16_t"), storageForUpdated_MyOwnStruct(true) {
        CITRIX_MOCK_UNREFPARAM(param0);
        __init__internal__();
    }

     TestObject(int32_t param0) : __constructor_Name__("void___int32_t"), storageForUpdated_MyOwnStruct(true) {
        CITRIX_MOCK_UNREFPARAM(param0);
        __init__internal__();
    }

     TestObject(int64_t param0) : __constructor_Name__("void___int64_t"), storageForUpdated_MyOwnStruct(true) {
        CITRIX_MOCK_UNREFPARAM(param0);
        __init__internal__();
    }

     TestObject(uint8_t param0) : __constructor_Name__("void___uint8_t"), storageForUpdated_MyOwnStruct(true) {
        CITRIX_MOCK_UNREFPARAM(param0);
        __init__internal__();
    }

     TestObject(std::string param0) : __constructor_Name__("void___stdstring"), storageForUpdated_MyOwnStruct(true) {
        CITRIX_MOCK_UNREFPARAM(param0);
        __init__internal__();
    }

     TestObject(uint32_t param0) : __constructor_Name__("void___uint32_t"), storageForUpdated_MyOwnStruct(true) {
        CITRIX_MOCK_UNREFPARAM(param0);
        __init__internal__();
    }

     TestObject(uint64_t param0) : __constructor_Name__("void___uint64_t"), storageForUpdated_MyOwnStruct(true) {
        CITRIX_MOCK_UNREFPARAM(param0);
        __init__internal__();
    }

     TestObject(float param0) : __constructor_Name__("void___float"), storageForUpdated_MyOwnStruct(true) {
        CITRIX_MOCK_UNREFPARAM(param0);
        __init__internal__();
    }

     TestObject(const MyOwnStruct& param0) : __constructor_Name__("void___constMyOwnStructRef"), storageForUpdated_MyOwnStruct(true) {
        CITRIX_MOCK_UNREFPARAM(param0);
        __init__internal__();
    }

    std::string __constructor_Name__;
    int32_t void___charPtr__COUNTER__;
    int32_t void___charPtr__COUNTER__const;
    int32_t void___bool__COUNTER__;
    int32_t void___bool__COUNTER__const;
    int32_t void___int8_t__COUNTER__;
    int32_t void___int8_t__COUNTER__const;
    int32_t void___int16_t__COUNTER__;
    int32_t void___int16_t__COUNTER__const;
    int32_t void___int32_t__COUNTER__;
    int32_t void___int32_t__COUNTER__const;
    int32_t void___int64_t__COUNTER__;
    int32_t void___int64_t__COUNTER__const;
    int32_t void___uint8_t__COUNTER__;
    int32_t void___uint8_t__COUNTER__const;
    int32_t void___stdstring__COUNTER__;
    int32_t void___stdstring__COUNTER__const;
    int32_t void___uint32_t__COUNTER__;
    int32_t void___uint32_t__COUNTER__const;
    int32_t void___uint64_t__COUNTER__;
    int32_t void___uint64_t__COUNTER__const;
    int32_t void___float__COUNTER__;
    int32_t void___float__COUNTER__const;
    int32_t void___constMyOwnStructRef__COUNTER__;
    int32_t void___constMyOwnStructRef__COUNTER__const;
    int32_t charPtr_____COUNTER__;
    int32_t charPtr_____COUNTER__const;
    int32_t bool_____COUNTER__;
    int32_t bool_____COUNTER__const;
    int32_t int8_t_____COUNTER__;
    int32_t int8_t_____COUNTER__const;
    int32_t int16_t_____COUNTER__;
    int32_t int16_t_____COUNTER__const;
    int32_t int32_t_____COUNTER__;
    int32_t int32_t_____COUNTER__const;
    int32_t int64_t_____COUNTER__;
    int32_t int64_t_____COUNTER__const;
    int32_t uint8_t_____COUNTER__;
    int32_t uint8_t_____COUNTER__const;
    int32_t stdstring_____COUNTER__;
    int32_t stdstring_____COUNTER__const;
    int32_t uint32_t_____COUNTER__;
    int32_t uint32_t_____COUNTER__const;
    int32_t uint64_t_____COUNTER__;
    int32_t uint64_t_____COUNTER__const;
    int32_t float_____COUNTER__;
    int32_t float_____COUNTER__const;
    int32_t MyOwnStructRef_____COUNTER__;
    int32_t MyOwnStructRef_____COUNTER__const;
    int32_t constMyOwnStructRef_____COUNTER__;
    int32_t constMyOwnStructRef_____COUNTER__const;
    int32_t constMyOwnStructPtr_____COUNTER__;
    int32_t constMyOwnStructPtr_____COUNTER__const;
    int32_t MyOwnStructPtr_____COUNTER__;
    int32_t MyOwnStructPtr_____COUNTER__const;
    int32_t bool___constMyOwnStructRef__stdstringRef__COUNTER__;
    int32_t bool___constMyOwnStructRef__stdstringRef__COUNTER__const;
    int32_t int8_t___MyOwnStructRef__conststdstringRef__COUNTER__;
    int32_t int8_t___MyOwnStructRef__conststdstringRef__COUNTER__const;
    int32_t int16_t___constMyOwnStructPtr__stdstringPtr__COUNTER__;
    int32_t int16_t___constMyOwnStructPtr__stdstringPtr__COUNTER__const;
    int32_t int32_t___MyOwnStructPtr__conststdstringPtr__COUNTER__;
    int32_t int32_t___MyOwnStructPtr__conststdstringPtr__COUNTER__const;
    int32_t constcharPtr___MyOwnStructPtrconst__COUNTER__;
    int32_t constcharPtr___MyOwnStructPtrconst__COUNTER__const;
    int32_t charPtr___constMyOwnStructPtrconst__COUNTER__;
    int32_t charPtr___constMyOwnStructPtrconst__COUNTER__const;
    int32_t stdstring___constuint32_tPtr__COUNTER__;
    int32_t stdstring___constuint32_tPtr__COUNTER__const;
    int32_t uint32_t___constuint64_tRef__COUNTER__;
    int32_t uint32_t___constuint64_tRef__COUNTER__const;
    int32_t uint64_t___boolPtr__COUNTER__;
    int32_t uint64_t___boolPtr__COUNTER__const;
    int32_t float___constfloatRef__COUNTER__;
    int32_t float___constfloatRef__COUNTER__const;

    char* storageForInitial_charPtr;
    char* storageForUpdated_charPtr;
    int8_t storageForInitial_int8_t;
    int8_t storageForUpdated_int8_t;
    int16_t storageForInitial_int16_t;
    int16_t storageForUpdated_int16_t;
    bool storageForInitial_bool;
    bool storageForUpdated_bool;
    int32_t storageForInitial_int32_t;
    int32_t storageForUpdated_int32_t;
    int64_t storageForInitial_int64_t;
    int64_t storageForUpdated_int64_t;
    uint8_t storageForInitial_uint8_t;
    uint8_t storageForUpdated_uint8_t;
    uint16_t storageForInitial_uint16_t;
    uint16_t storageForUpdated_uint16_t;
    uint32_t storageForInitial_uint32_t;
    uint32_t storageForUpdated_uint32_t;
    uint64_t storageForInitial_uint64_t;
    uint64_t storageForUpdated_uint64_t;
    double storageForInitial_double;
    double storageForUpdated_double;
    float storageForInitial_float;
    float storageForUpdated_float;
    std::string storageForInitial_stdstring;
    std::string storageForUpdated_stdstring;
    MyOwnStruct storageForInitial_MyOwnStruct;
    MyOwnStruct storageForUpdated_MyOwnStruct;


    void __init__internal__() {
        void___charPtr__COUNTER__ = 0;
        void___charPtr__COUNTER__const = 0;
        void___bool__COUNTER__ = 0;
        void___bool__COUNTER__const = 0;
        void___int8_t__COUNTER__ = 0;
        void___int8_t__COUNTER__const = 0;
        void___int16_t__COUNTER__ = 0;
        void___int16_t__COUNTER__const = 0;
        void___int32_t__COUNTER__ = 0;
        void___int32_t__COUNTER__const = 0;
        void___int64_t__COUNTER__ = 0;
        void___int64_t__COUNTER__const = 0;
        void___uint8_t__COUNTER__ = 0;
        void___uint8_t__COUNTER__const = 0;
        void___stdstring__COUNTER__ = 0;
        void___stdstring__COUNTER__const = 0;
        void___uint32_t__COUNTER__ = 0;
        void___uint32_t__COUNTER__const = 0;
        void___uint64_t__COUNTER__ = 0;
        void___uint64_t__COUNTER__const = 0;
        void___float__COUNTER__ = 0;
        void___float__COUNTER__const = 0;
        void___constMyOwnStructRef__COUNTER__ = 0;
        void___constMyOwnStructRef__COUNTER__const = 0;
        charPtr_____COUNTER__ = 0;
        charPtr_____COUNTER__const = 0;
        bool_____COUNTER__ = 0;
        bool_____COUNTER__const = 0;
        int8_t_____COUNTER__ = 0;
        int8_t_____COUNTER__const = 0;
        int16_t_____COUNTER__ = 0;
        int16_t_____COUNTER__const = 0;
        int32_t_____COUNTER__ = 0;
        int32_t_____COUNTER__const = 0;
        int64_t_____COUNTER__ = 0;
        int64_t_____COUNTER__const = 0;
        uint8_t_____COUNTER__ = 0;
        uint8_t_____COUNTER__const = 0;
        stdstring_____COUNTER__ = 0;
        stdstring_____COUNTER__const = 0;
        uint32_t_____COUNTER__ = 0;
        uint32_t_____COUNTER__const = 0;
        uint64_t_____COUNTER__ = 0;
        uint64_t_____COUNTER__const = 0;
        float_____COUNTER__ = 0;
        float_____COUNTER__const = 0;
        MyOwnStructRef_____COUNTER__ = 0;
        MyOwnStructRef_____COUNTER__const = 0;
        constMyOwnStructRef_____COUNTER__ = 0;
        constMyOwnStructRef_____COUNTER__const = 0;
        constMyOwnStructPtr_____COUNTER__ = 0;
        constMyOwnStructPtr_____COUNTER__const = 0;
        MyOwnStructPtr_____COUNTER__ = 0;
        MyOwnStructPtr_____COUNTER__const = 0;
        bool___constMyOwnStructRef__stdstringRef__COUNTER__ = 0;
        bool___constMyOwnStructRef__stdstringRef__COUNTER__const = 0;
        int8_t___MyOwnStructRef__conststdstringRef__COUNTER__ = 0;
        int8_t___MyOwnStructRef__conststdstringRef__COUNTER__const = 0;
        int16_t___constMyOwnStructPtr__stdstringPtr__COUNTER__ = 0;
        int16_t___constMyOwnStructPtr__stdstringPtr__COUNTER__const = 0;
        int32_t___MyOwnStructPtr__conststdstringPtr__COUNTER__ = 0;
        int32_t___MyOwnStructPtr__conststdstringPtr__COUNTER__const = 0;
        constcharPtr___MyOwnStructPtrconst__COUNTER__ = 0;
        constcharPtr___MyOwnStructPtrconst__COUNTER__const = 0;
        charPtr___constMyOwnStructPtrconst__COUNTER__ = 0;
        charPtr___constMyOwnStructPtrconst__COUNTER__const = 0;
        stdstring___constuint32_tPtr__COUNTER__ = 0;
        stdstring___constuint32_tPtr__COUNTER__const = 0;
        uint32_t___constuint64_tRef__COUNTER__ = 0;
        uint32_t___constuint64_tRef__COUNTER__const = 0;
        uint64_t___boolPtr__COUNTER__ = 0;
        uint64_t___boolPtr__COUNTER__const = 0;
        float___constfloatRef__COUNTER__ = 0;
        float___constfloatRef__COUNTER__const = 0;
        storageForInitial_charPtr = "Hello World!";
        storageForUpdated_charPtr = "There is no World :(";
        storageForInitial_int8_t = 124;
        storageForUpdated_int8_t = -23;
        storageForInitial_int16_t = 1236;
        storageForUpdated_int16_t = -3445;
        storageForInitial_bool = true;
        storageForUpdated_bool = false;
        storageForInitial_int32_t = -1423;
        storageForUpdated_int32_t = 1422356763;
        storageForInitial_int64_t = -1235345345345345343LL;
        storageForUpdated_int64_t = 3297109850394757343LL;
        storageForInitial_uint8_t = 143;
        storageForUpdated_uint8_t = 233;
        storageForInitial_uint16_t = 1239;
        storageForUpdated_uint16_t = 24455;
        storageForInitial_uint32_t = 1883345323;
        storageForUpdated_uint32_t = 3564677783;
        storageForInitial_uint64_t = 1237429349238472938ULL;
        storageForUpdated_uint64_t = 2534545768788782938ULL;
        storageForInitial_double = -13345323.122345;
        storageForUpdated_double = +13345323.122345;
        storageForInitial_float = +2.3e-8f;
        storageForUpdated_float = -2.3e-8f;
        storageForInitial_stdstring = "I am a String in Theory!";
        storageForUpdated_stdstring = "I was a String in Theory!";
    }

};

#endif

