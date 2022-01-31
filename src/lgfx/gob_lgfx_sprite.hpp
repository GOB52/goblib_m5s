/*!
  goblib for M5Stack (Depends on M5Stack releated)

  @file  gob_lgfx_sprite.hpp
  @brief Specialized sprites.
  Fast copy for from 4bit sprite to 16bit sprite.
  @note Depends on LovyanGFX
*/
#pragma once
#ifndef GOBLIB_M5S_LGFX_SPRITE_HPP
#define GOBLIB_M5S_LGFX_SPRITE_HPP

#if defined(LGFX_USE_V1)
#include <lgfx/v1/LGFX_Sprite.hpp>
#else
#include <lgfx/v0/LGFX_Sprite.hpp>
#endif
#include "gob_lgfx.hpp"
#include "gob_macro.hpp"

namespace goblib { namespace lgfx {

/*!
  @brief Spcialized 4bit sprite.
*/
class GSprite4 : public GSprite
{
  public:
    explicit GSprite4(GLovyanGFX* parent = nullptr)
            : GSprite(parent)
            , _table16{}
            , _table256{}
    { setColorDepth(4); }
    GSprite4(std::int32_t w, std::int32_t h)
    {
        setColorDepth(4);
        createSprite(w, h);
    }
    virtual ~GSprite4() {}

    // 4to16 (without transparency)
    GOBLIB_INLINE void pushSpriteTo16(GLovyanGFX* dst, std::int32_t x, std::int32_t y)
    {
        push_sprite_to16(dst, x, y);
    }

    template<typename T> GOBLIB_INLINE
    void pushSpriteTo16(GLovyanGFX* dst, std::int32_t x, std::int32_t y, const T& transp)
    {
        push_sprite_to16(dst, x, y, _write_conv.convert(transp) & _write_conv.colormask);
    }

    void makePaletteTable256()
    {
        makePaletteTable();
        //  RGBColor* sp = getPalette();
        std::uint32_t* qp = _table256;
        for(int i=0;i<256;++i)
        {
            *qp++ = (_table16[i%16].raw << 16) | _table16[i/16].raw;
        }
    }

    // for Debug
    //    void printPalette();

    
  protected:
    bool use_dma() const
    {
#if defined(LGFX_USE_V1)
        return _panel_sprite.getSpriteBuffer()->use_dma();
#else
        return const_cast<SpriteBuffer*>(&_img)->use_dma();
#endif
    }

    void makePaletteTable()
    {
        RGBColor* sp = getPalette();
        ::lgfx::swap565_t* qp = _table16;
        std::uint_fast8_t len = std::min(16U, getPaletteCount());
        while(len--) { *qp++ = *sp++; } // convert to 565
    }

    GOBLIB_INLINE void push_sprite_to16(GLovyanGFX* dst, std::int32_t x, std::int32_t y, std::uint32_t transp = ~0)
    {
#if defined(LGFX_USE_V1)
        assert((getColorDepth() & ::lgfx::color_depth_t::bit_mask) == 4 && "Invalid src");
        assert((dst->getColorDepth() & ::lgfx::color_depth_t::bit_mask) == 16 && "Illegal target");
#else
        assert(getColorDepth() == 4 && "Invalid src");
        assert(dst->getColorDepth() == 16 && "Illegal target");
#endif
        
        ::lgfx::pixelcopy_t p(_img, dst->getColorDepth(), getColorDepth(), dst->hasPalette(), _palette, transp);
        p.fp_copy = (transp == ~0) ? fp_copy_4to16 : fp_copy_4to16_transp;
        p.palette = (transp == ~0) ? static_cast<void*>(_table256) : static_cast<void*>(_table16);
        dst->pushImage(x, y, width(), height(), &p, use_dma());
    }

#if defined(LGFX_USE_V1)
    static std::uint32_t fp_copy_4to16(void* __restrict__ dst, std::uint32_t index, std::uint32_t last, ::lgfx::pixelcopy_t* __restrict__ param);
    static std::uint32_t fp_copy_4to16_transp(void* __restrict__ dst, std::uint32_t index, std::uint32_t last, ::lgfx::pixelcopy_t* __restrict__ param);
    static std::uint32_t fp_skip_transp(std::uint32_t index, std::uint32_t last, ::lgfx::pixelcopy_t* param);
#else
    static std::int32_t fp_copy_4to16( void* __restrict__ dst, std::int32_t index, std::int32_t last, ::lgfx::pixelcopy_t* __restrict__ param);
    static std::int32_t fp_copy_4to16_transp( void* __restrict__ dst, std::int32_t index, std::int32_t last, ::lgfx::pixelcopy_t* __restrict__ param);
    static std::int32_t fp_skip_transp(std::int32_t index, std::int32_t last, ::lgfx::pixelcopy_t* param);
#endif
    
    ::lgfx::swap565_t   _table16[16];   //!< 4bit single pixel to 565 single pixel
    std::uint32_t   _table256[256]; //!< 4bit double pixels to 565 double pixels
};

/*!
  @brief Spcialized 4bit sprite for Cell animation.
*/
class GCellSprite4 : public GSprite4
{
  public:
    explicit GCellSprite4(GLovyanGFX* parent = nullptr) : GSprite4(parent) {}
    GCellSprite4(std::int32_t w, std::int32_t h) : GSprite4(w,h) {}
    virtual ~GCellSprite4() {}

    template<typename T> GOBLIB_INLINE void pushCellTo16(GLovyanGFX* dst, const CellRect& r, std::int32_t x, std::int32_t y, const T& transp)
    {
        push_cell(dst, r, x, y, _write_conv.convert(transp) & _write_conv.colormask);
    }
    GOBLIB_INLINE void pushCellTo16(GLovyanGFX* dst, const CellRect& r, std::int32_t x, std::int32_t y)
    {

        push_cell(dst, r, x, y);
    }

  protected:
    void push_cell(GLovyanGFX* dst, const CellRect& srect, std::int32_t x, std::int32_t y, std::uint32_t transp = ~0)
    {
#if defined(LGFX_USE_V1)   
        assert((getColorDepth() & ::lgfx::color_depth_t::bit_mask) == 4 && "Invalid src");
        assert((dst->getColorDepth() & ::lgfx::color_depth_t::bit_mask) == 16 && "Illegal target");
#else
        assert(getColorDepth() == 4 && "Invalid src");
        assert(dst->getColorDepth() == 16 && "Illegal target");
#endif
        std::int32_t cx,cy,cw,ch;
        dst->getClipRect(&cx,&cy,&cw,&ch);

#if 0
        CellRect orect(cx, cy, cw, ch);
        CellRect drect(x, y, srect.width(), srect.height());
        orect &= drect;
        if(orect.empty()) return;
        dst->setClipRect(x, y, orect.width(), orect.height());

        ::lgfx::pixelcopy_t p(_img, dst->getColorDepth(), getColorDepth(), dst->hasPalette(), _palette, transp);
        //  p.fp_copy = transp == ~0 ? fp_copy_4to16 : fp_copy_4to16_transp;
        //  p.palette = transp == ~0 ? static_cast<void*>(_table256) : static_cast<void*>(_table16);
        //  p.fp_copy = fp_copy_4to16;
        //p.palette = _table256;
        //  fprintf(stderr, ">>>>>>>>>>>>>>>>>>>>> %s:(%d,%d) %d,%d => (%d,%d)\n",
        //          __func__, p.src_x, p.src_y, x,y, x - srect.left(), y - srect.top());

        fprintf(stderr, ">>>>> srect(%d,%d,%d,%d) oclipt(%d,%d,%d,%d) drect(%d,%d,%d,%d) orect(%d,%d,%d,%d)\n",
                srect.left(), srect.top(), srect.width(), srect.height(),
                cx,cy,cw,ch,
                drect.left(), drect.top(), drect.width(), drect.height(),
                orect.left(), orect.top(), orect.width(), orect.height());


        dst->pushImage(x - srect.left(), y - srect.top(), orect.width(), orect.height(), &p, use_dma());
#else
        dst->setClipRect(x, y, srect.width(), srect.height());
        ::lgfx::pixelcopy_t p(_img, dst->getColorDepth(), getColorDepth(), dst->hasPalette(), _palette, transp);
        //  p.fp_copy = transp == ~0 ? fp_copy_4to16 : fp_copy_4to16_transp;
        //  p.palette = transp == ~0 ? static_cast<void*>(_table256) : static_cast<void*>(_table16);
        p.fp_copy = fp_copy_4to16_transp;
        p.fp_skip = fp_skip_transp;
        p.palette = _table16;
        dst->pushImage(x - srect.left(), y - srect.top(), width(), height(), &p, use_dma());
#endif
        dst->setClipRect(cx, cy, cw, ch);
    }
};

//
}}
#endif
