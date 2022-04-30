/*
  Helper for type
*/
#ifndef HELPER_HPP
#define HELPER_HPP

#include <string>
#include <type_traits>

namespace test { namespace helper {

// Like typeid without RTTI.
using TypeId_type = void*;
template<class T> TypeId_type TypeId()
{
    static std::remove_reference<T>* tptr = nullptr;
    return &tptr;
}

// Like typeid(foo).name() without RTTI.
template<typename T> const char* TypeName()
{
    static std::string s = __PRETTY_FUNCTION__;
    return s.c_str();
}

//
}}
#endif
