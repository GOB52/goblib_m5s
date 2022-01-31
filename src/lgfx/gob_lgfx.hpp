/*!
  goblib for M5Stack (Depends on M5Stack releated)

  @file  gob_lgfx.hpp
  @brief Definition for using LovyanGFX.
  @note Depends on LovyanGFX
*/
#pragma once
#ifndef GOBLIB_LGFX_HPP
#define GOBLIB_LGFX_HPP

#include <gob_rect2d.hpp>

// RGBColor definition
#if defined(LGFX_USE_V1)
#include <lgfx/v1/misc/colortype.hpp>
#else
#include <lgfx/v0/lgfx_common.hpp>
#endif

namespace lgfx
{
namespace v0 { class LGFX_Sprite; class LovyanGFX; }
namespace v1 { class LGFX_Sprite; class LovyanGFX; }
}

namespace goblib { namespace lgfx {

/// @name Typedef
/// @{
/*! @brief Rectangle of cell */
using CellRect = goblib::shape2d::Rectangle<std::int16_t>;

/*! 
  @brief GLovyanGFX as LovyanGFX 
  @remark Switches between v0 or v1 depending on whether LGFX_USE_V1 is defined or not.
*/
#if defined(LGFX_USE_V1)
using GLovyanGFX = ::lgfx::v1::LovyanGFX;
#else
using GLovyanGFX = ::lgfx::v0::LovyanGFX;
#endif

/*! 
  @brief GSprite as LGFX_Sprite 
  @remark Switches between v0 or v1 depending on whether LGFX_USE_V1 is defined or not.
*/
#if defined(LGFX_USE_V1)
using GSprite = ::lgfx::v1::LGFX_Sprite;
#else
using GSprite = ::lgfx::v0::LGFX_Sprite;
#endif

/// @}
}}
#endif
