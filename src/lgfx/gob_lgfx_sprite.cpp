/*!
  goblib for M5Stack (Depends on M5Stack releated)

  @file  gob_lgfx_sprite.hpp
  @brief Specialized sprites.
  Fast copy for from 4bit sprite to 16bit sprite.
  @note Depends on LovyanGFX
*/
#include "gob_lgfx_sprite.hpp"
#include <cassert>

namespace goblib { namespace lgfx {

/*! Fast copy for 4bit sprite to 16bit sprite without transparency.
*/
#if defined(LGFX_USE_V1)
std::uint32_t GSprite4::fp_copy_4to16(void* __restrict__ dst, std::uint32_t index, std::uint32_t last, ::lgfx::pixelcopy_t* __restrict__ param)
#else
std::int32_t GSprite4::fp_copy_4to16(void* __restrict__ dst, std::int32_t index, std::int32_t last, ::lgfx::pixelcopy_t* __restrict__ param)
#endif
{
    auto s = static_cast<const std::uint8_t*>(param->src_data) +
            (((param->src_x + param->src_y * param->src_bitwidth) * param->src_bits) >> 3);
    auto d = reinterpret_cast<std::uint32_t*>(static_cast<::lgfx::swap565_t*>(dst) + index);
    auto pal = static_cast<const std::uint32_t*>(param->palette);

    // each 2 pixels.
    do
    {
        *d++ = pal[*s++]; // write 2pixels.
        ++index;
        ++index;
    }while(index < last);
    return index;
}

/*! Fast copy for 4bit sprite to 16bit sprite with transparency. */
#if defined(LGFX_USE_V1)
std::uint32_t GSprite4::fp_copy_4to16_transp(void* __restrict__ dst, std::uint32_t index, std::uint32_t last, ::lgfx::pixelcopy_t* __restrict__ param)
#else
std::int32_t GSprite4::fp_copy_4to16_transp(void* __restrict__ dst, std::int32_t index, std::int32_t last, ::lgfx::pixelcopy_t* __restrict__ param)
#endif
{
#if 0
    auto s = static_cast<const std::uint8_t*>(param->src_data);
    auto d = static_cast<::lgfx::swap565_t*>(dst);
    auto pal = static_cast<const ::lgfx::swap565_t*>(param->palette);
    std::uint32_t shift = (param->src_x & 1) ? 0 : 4;
    do {
        std::uint32_t i = (param->src_x + param->src_y * param->src_bitwidth) * param->src_bits;
        //std::uint32_t raw = (s[i >> 3] >> (-(i + param->src_bits) & 7)) & param->src_mask;
        std::uint32_t raw = (s[i >> 3] >> shift) & 0x0f;
        if (!raw) break;
        d[index] = pal[raw];
        ++param->src_x;
        shift ^= 4;
        //        param->src_x32 += param->src_x32_add;
        //        param->src_y32 += param->src_y32_add;
    } while (++index != last);
    return index;
#else
    auto s = static_cast<const std::uint8_t*>(param->src_data);
    auto d = static_cast<::lgfx::swap565_t*>(dst);
    auto pal = static_cast<const ::lgfx::swap565_t*>(param->palette);
    std::uint32_t shift = (param->src_x & 1) ? 0 : 4;
    do {
        std::uint32_t i = (param->src_x + param->src_y * param->src_bitwidth) * param->src_bits;
        //std::uint32_t raw = (s[i >> 3] >> (-(i + param->src_bits) & 7)) & param->src_mask;
        std::uint32_t raw = (s[i >> 3] >> shift) & 0x0f;
        if(!raw) break;
        d[index] = pal[raw];
        ++param->src_x;
        shift ^= 4;
        //        param->src_x32 += param->src_x32_add;
        //        param->src_y32 += param->src_y32_add;
    } while (++index != last);
    return index;
#endif
}

/*! Skip transparency. */
#if defined(LGFX_USE_V1)
std::uint32_t GSprite4::fp_skip_transp(std::uint32_t index, std::uint32_t last, ::lgfx::pixelcopy_t* param)
#else
std::int32_t GSprite4::fp_skip_transp(std::int32_t index, std::int32_t last, ::lgfx::pixelcopy_t* param)
#endif
{
    auto s = static_cast<const std::uint8_t*>(param->src_data);
    std::uint32_t shift = (param->src_x & 1) ? 0 : 4;
    do {
        std::uint32_t i = (param->src_x + param->src_y * param->src_bitwidth) * param->src_bits;
        std::uint32_t raw = (s[i >> 3] >> shift) & 0x0f;
        if(raw) break;
        ++param->src_x;
        shift ^= 4;
    } while (++index != last);
    return index;
}

//
}}
