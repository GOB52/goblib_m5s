/*!
  goblib for M5Stack (Depends on M5Stack releated)

  @file  gob_lgfx.cpp
  @brief Definition for using LovyanGFX.
  @note Depends on LovyanGFX
*/
#include <LovyanGFX.hpp>
#include "gob_lgfx.hpp"
#include "../gob_m5s_sd.hpp"

namespace goblib { namespace lgfx {

bool createSprite(GSprite& s, const char* path)
{
    goblib::m5s::File file;
    file.open(path, O_READ);
    if(!file) { return false; }

    std::size_t len = file.available();
    if(len == 0) { return false; }

    auto bmp = new std::uint8_t[len];
    if(!bmp) { return false; }

    bool b = false;
    if(len == file.read(bmp, len))
    {
        s.createFromBmp(bmp, len);
        b = true;
    }
    delete[] bmp;
    return b;
}

//
}}
