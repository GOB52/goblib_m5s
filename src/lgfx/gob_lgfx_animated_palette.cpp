/*!
  goblib for M5Stack (Depends on M5Stack releated)

  @file   gob_lgfx_animated_palette.cpp
  @brief  Palette animation helper for LGFX_Sprite.
  @note Depends on LovyanGFX
*/
#include "gob_lgfx.hpp"
#include "gob_lgfx_animated_palette.hpp"
#include <lgfx/v1/LGFX_Sprite.hpp>

namespace goblib { namespace lgfx {

std::size_t AnimatedPalette::pump()
{
    if(isPause()) { return 0U; }

    auto processed = _sequences.size();
    auto it = _sequences.begin();
    while(it != _sequences.end())
    {
        auto& seq = *it;
        bool b = seq.pump();
        _current[seq._index] = seq;
        it = b ? _sequences.erase(it) : it + 1;
    }
    if(_sprite && processed) { to(*_sprite); }
    return processed;
}

void AnimatedPalette::to(LGFX_Sprite& dst)
{
    to(dst.getPalette(), dst.getPaletteCount());
}

void AnimatedPalette::to(RGBColor* dest, std::size_t sz)
{
    sz = std::min(sz, _current.size());
    auto s = _current.data();
    while(sz--) { *dest++ = *s++; }
}


void AnimatedPalette::from(LGFX_Sprite& src)
{
    from(src.getPalette(), src.getPaletteCount());
}

void AnimatedPalette::from(RGBColor* src, std::size_t sz)
{
    sz = std::min(sz, _current.size());
    auto d = _current.data();
    while(sz--) { *d++ = *src++; }
    _save = _current;
}

bool AnimatedPalette::Sequence::pump()
{
    auto t = goblib::easing::linear(static_cast<float>(_counter) / _times);
    _now = RGBColor(_start.r + _diff[0] * t,
                    _start.g + _diff[1] * t,
                    _start.b + _diff[2] * t);
    if(_counter++ == _times)
    {
        _now = _to[_toward];
        if(!_roundTrip) { return true; }
        _diff[0] = static_cast<std::int16_t>(_to[_toward^1].r) - static_cast<std::int16_t>(_to[_toward].r);
        _diff[1] = static_cast<std::int16_t>(_to[_toward^1].g) - static_cast<std::int16_t>(_to[_toward].g);
        _diff[2] = static_cast<std::int16_t>(_to[_toward^1].b) - static_cast<std::int16_t>(_to[_toward].b);
        _start = _now;
        _toward ^= 1;
        _counter = 0;
    }
    return false;
}

//
}}
