/*!
  goblib for M5Stack (Depends on M5Stack releated)

  @file  gob_m5s_mutex.hpp
  @brief Mutex with an interface compatible with std::mutex, as implemented in the FreeRTOS API.
*/
#pragma once
#ifndef GOBLIB_M5S_MUTEX_HPP
#define GOBLIB_M5S_MUTEX_HPP

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h> // xSemaphoreHandle
#include <gob_macro.hpp>

namespace goblib { namespace m5s {

/*! 
  @brief Compatible with std::mutex
  Can be used with lock_guard, unique_lock, and scoped_lock(C++17 or later)
*/
class Mutex
{
  public:
    using native_handle_type = xSemaphoreHandle;
    /*! @warning std::mutex can constructor to constexpr, but this is NOT. */
    Mutex() : _handle(0) { _handle = xSemaphoreCreateMutex(); }
    Mutex(Mutex&& x) : _handle(0)
    {
        if(this != &x)
        {
            _handle = x._handle;
            x._handle = 0;
        }
    }
    ~Mutex() {  unlock(); vSemaphoreDelete(_handle); }

    Mutex& operator=(Mutex&& x)
    {
        if(this != &x)
        {
            vSemaphoreDelete(_handle);
            _handle = x._handle;
            x._handle = 0;
        }
        return *this;
    }

    GOBLIB_INLINE void lock() { xSemaphoreTake(_handle, portMAX_DELAY); }
    GOBLIB_INLINE bool try_lock() { return xSemaphoreTake(_handle, 0) == pdTRUE; }
    GOBLIB_INLINE void unlock() { xSemaphoreGive(_handle); }
    GOBLIB_INLINE native_handle_type native_handle() { return _handle; }
    
  private:
    Mutex(const Mutex&) = delete;
    Mutex& operator=(const Mutex&) = delete;

  private:
    native_handle_type _handle;
};

/*! 
  @brief Compatible with std::recursive_mutex
  Can be used with lock_guard, unique_lock, and scoped_lock(C++17 or later)
*/
class RecursiveMutex
{
  public:
    using native_handle_type = xSemaphoreHandle;
    RecursiveMutex() : _handle(0) { _handle = xSemaphoreCreateRecursiveMutex(); }
    RecursiveMutex(RecursiveMutex&& x) : _handle(0)
    {
        if(this != &x)
        {
            _handle = x._handle;
            x._handle = 0;
        }
    }
    ~RecursiveMutex() { vSemaphoreDelete(_handle); }

    RecursiveMutex& operator=(RecursiveMutex&& x)
    {
        if(this != &x)
        {
            vSemaphoreDelete(_handle);
            _handle = x._handle;
            x._handle = 0;
        }
        return *this;
    }
    
    GOBLIB_INLINE void lock() { xSemaphoreTakeRecursive(_handle, portMAX_DELAY); }
    GOBLIB_INLINE bool try_lock() { return xSemaphoreTakeRecursive(_handle, 0) == pdTRUE; }
    GOBLIB_INLINE void unlock() { xSemaphoreGiveRecursive(_handle); }
    GOBLIB_INLINE native_handle_type native_handle() { return _handle; }
    
  private:
    RecursiveMutex(const Mutex&) = delete;
    RecursiveMutex& operator=(const Mutex&) = delete;
    
  private:
    native_handle_type _handle;
};

//
}}
#endif
