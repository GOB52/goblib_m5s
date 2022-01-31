/*!
  goblib for M5Stack (Depends on M5Stack releated)

  @file  gob_m5s_debug.hpp
  @brief Debugging support for M5Stack
*/
#include "gob_m5s_debug.hpp"
#include <esp_heap_caps.h>
#include <esp_system.h>
#include <cstdio>

namespace goblib { namespace m5s {

ScopedLeakCheck::ScopedLeakCheck(const char* str, const std::uint32_t line, bool abort)
        : _str(str), _size(0), _line(line), _abort(abort)
{
    _size =  esp_get_free_heap_size();
}

ScopedLeakCheck::~ScopedLeakCheck()
{
    auto es =  esp_get_free_heap_size();
    if(es < _size)
    {
        printf("%s:%u:Detect leak %u => %u\n", _str, _line, _size, es);
        //  heap_caps_dump(MALLOC_CAP_8BIT);
        if(_abort) { assert(0 && "DETECT LEAK"); abort(); }
    }

}

//
}}
