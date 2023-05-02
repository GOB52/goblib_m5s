/*!
  goblib for M5Stack (Depends on M5Stack releated)

  @file  gob_lgfx.hpp
  @brief Definition for using LovyanGFX.
  @note Depends on LovyanGFX or M5GFX
*/
#ifndef GOBLIB_LGFX_HPP
#define GOBLIB_LGFX_HPP

//#include <SdFat.h>
#if __has_include(<M5GFX.h>)
#pragma message "using M5GFX"
# include <M5GFX.h>
#elif __has_include(<LovyanGFX.h>)
#pragma message "using LovyanGFX"
# include <LovyanGFX.h>
#else
# error "Must be required M5GFX or LovyanGFX"
#endif
#include <gob_rect2d.hpp>

//namespace lgfx { namespace v1 { class LGFX_Sprite; class LovyanGFX; }}

namespace goblib { namespace lgfx {

/// @name Typedef
/// @{
/*! @brief Rectangle of cell */
using CellRect = goblib::shape2d::Rectangle<std::int16_t>;

/*! 
  @brief GLovyanGFX as LovyanGFX 
  @remark Switches between v0 or v1 depending on whether LGFX_USE_V1 is defined or not.
*/
//using GLovyanGFX = ::lgfx::v1::LovyanGFX;

/*! 
  @brief GSprite as LGFX_Sprite 
  @remark Switches between v0 or v1 depending on whether LGFX_USE_V1 is defined or not.
*/
//using GSprite = ::lgfx::v1::LGFX_Sprite;

/*! Create sprite from resource at SD card.
 */
bool createSprite(LGFX_Sprite& s, const char* path);

/// @}
}}
#endif
