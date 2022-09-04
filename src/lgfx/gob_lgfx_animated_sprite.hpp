/*!
  goblib for M5Stack (Depends on M5Stack releated)

  @file  gob_lgfx_animated_sprite.hpp
  @brief For cell animation
  @note Depends on LovyanGFX
*/
#pragma once
#ifndef GOBLIB_M5S_ANIMATED_SPRITE_HPP
#define GOBLIB_M5S_ANIMATED_SPRITE_HPP

#include <gob_macro.hpp>
#include <gob_animation.hpp>
#include "gob_lgfx.hpp"
#include <vector>

namespace goblib { namespace lgfx {

/*!
  @class CellSprite
  @brief Draw any area of the sprite.
*/
class CellSprite : public GSprite
{
  public:
    explicit CellSprite(GLovyanGFX* parent = nullptr) : GSprite(parent) {}
    virtual ~CellSprite() {}

    template<typename T> GOBLIB_INLINE
    void pushCell(GLovyanGFX* dst, const CellRect& r, std::int32_t x, std::int32_t y, const T& transp)
    {
        push_cell(dst, r, x, y, _write_conv.convert(transp) & _write_conv.colormask);
    }
    GOBLIB_INLINE void pushCell(GLovyanGFX* dst, const CellRect& r, std::int32_t x, std::int32_t y)
    {
        push_cell(dst, r, x, y);
    }
    
  protected:
    /*!
      @brief Draw any area of ther sprite.
      @param srect Area in the sprite to push.
      @param flipH Flip horizontally?
      @param flipV Flip vertically?
      @note useing pushImageRotateZoom
    */
    void push_cell(GLovyanGFX* dst, const CellRect& srect, std::int32_t x, std::int32_t y, bool flipH, bool flipV, std::uint32_t transp = ~0)
    {
        std::int32_t cx,cy,cw,ch;
        dst->getClipRect(&cx,&cy,&cw,&ch);
        dst->setClipRect(x, y, srect.width(), srect.height());

        float px = srect.left() + srect.width()  * 0.5f - ((srect.width()  & 1) ^ 1) * 0.5f;
        float py = srect.top()  + srect.height() * 0.5f - ((srect.height() & 1) ^ 1) * 0.5f;
        dst->pushImageRotateZoom(x  + srect.width() * 0.5f,
                                 y  + srect.height() * 0.5f, 
                                 px, py,
                                 0, flipH ? -1.0f : 1.0f, flipV ? -1.0f : 1.0f,
                                 width(), height(),
                                 _img, transp, getColorDepth(), _palette.img24());
        dst->setClipRect(cx, cy, cw, ch);
    }

    /*!
      @brief Draw any area of ther sprite.
      @param srect Area in the sprite to push.
      @note using pushImage
    */
    void push_cell(GLovyanGFX* dst, const CellRect& srect, std::int32_t x, std::int32_t y, std::uint32_t transp = ~0)
    {
        std::int32_t cx,cy,cw,ch;
        dst->getClipRect(&cx,&cy,&cw,&ch);
        dst->setClipRect(x, y, srect.width(), srect.height());
        // No flip.
        ::lgfx::pixelcopy_t p(_img, dst->getColorDepth(), getColorDepth(), dst->hasPalette(), _palette, transp);
#ifdef LGFX_USE_V1
        dst->pushImage(x - srect.left(), y - srect.top(), width(), height(), &p, _panel_sprite.getSpriteBuffer()->use_dma());
#else
        dst->pushImage(x - srect.left(), y - srect.top(), width(), height(), &p, _img.use_dma());
#endif
        dst->setClipRect(cx, cy, cw, ch);
    }
};

/*!
  @class AnimatedSprite
  @brief Play sliced cell animation by sequencer.
*/
class AnimatedSprite : public CellSprite
{
  public:
    explicit AnimatedSprite(GLovyanGFX* parent = nullptr)
            : CellSprite(parent)
            , _slice_width(0)
            , _slice_height(0)
            , _pause(false)
            , _flipH(false)
            , _flipV(false)
            , _sequencer(nullptr)
    { _cells.reserve(255); }
    virtual ~AnimatedSprite() {}

    GOBLIB_INLINE std::size_t cells() const { return _cells.size(); }

    /// @name Flip
    /// @{
    GOBLIB_INLINE bool isFlipH() const { return _flipH; }
    GOBLIB_INLINE bool isFlipV() const { return _flipV; }
    GOBLIB_INLINE void flipH(bool b) { _flipH = b; }
    GOBLIB_INLINE void flipV(bool b) { _flipV = b; }
    GOBLIB_INLINE void flipH() { _flipH = !_flipH; }
    GOBLIB_INLINE void flipV() { _flipV = !_flipV; }
    /// @}
    
    /*!
      @warning Sliced cell max is 255.
    */
    std::size_t slice(std::int32_t w, std::int32_t h)
    {
        _cells.clear();
        if( (height() / h) * (width() / w) > 255) return 0;

        for(std::int32_t yy = 0; yy < height(); yy += h)
        {
            for(std::int32_t xx = 0; xx < width(); xx += w)
            {
                _cells.push_back(CellRect(xx, yy, w, h));
            }
        }
        _slice_width = w;
        _slice_height = h;
        return _cells.size();
    }

    template<typename T> GOBLIB_INLINE
    void pushCell(std::uint8_t c, std::int32_t x, std::int32_t y, const T& transp)
    {
        push_cell(_parent, _cells[c], x, y, _flipH, _flipV, _write_conv.convert(transp) & _write_conv.colormask);
    }

    /*!
      @tparam T color type.
      @param c cell index.
      @param dst destnation LovyanGFX
      @param x coordinate to draw at destination.
      @param y coordinate to draw at destination.
      @param transp transparent color.      
    */
    template<typename T> GOBLIB_INLINE
    void pushCell(GLovyanGFX* dst, std::uint8_t c, std::int32_t x, std::int32_t y, const T& transp)
    {
        push_cell(dst, _cells[c], x, y, _flipH, _flipV, _write_conv.convert(transp) & _write_conv.colormask);
    }
    template<typename T> GOBLIB_INLINE
    void pushCell(std::uint8_t c, std::int32_t x, std::int32_t y)
    {
        push_cell(_parent, _cells[c], x, y, _flipH, _flipV);
    }
    GOBLIB_INLINE void pushCell(GLovyanGFX* dst, std::uint8_t c, std::int32_t x, std::int32_t y)
    {
        push_cell(dst, _cells[c], x, y, _flipH, _flipV);
    }

    template<typename T> GOBLIB_INLINE
    void pushCell(std::int32_t x, std::int32_t y, const T& transp)
    {
        push_cell(_parent, _cells[_sequencer->cell()], x, y, _flipH ^ _sequencer->isFlipH(), _flipV ^ _sequencer->isFlipV(), _write_conv.convert(transp) & _write_conv.colormask);
    }
    template<typename T> GOBLIB_INLINE
    void pushCell(GLovyanGFX* dst, std::int32_t x, std::int32_t y, const T& transp)
    {
        push_cell(dst, _cells[_sequencer->cell()], x, y, _flipH ^ _sequencer->isFlipH(), _flipV ^ _sequencer->isFlipV(), _write_conv.convert(transp) & _write_conv.colormask);
    }

    GOBLIB_INLINE void pushCell(std::int32_t x, std::int32_t y)
    {
        push_cell(_parent, _cells[_sequencer->cell()], x, y, _flipH ^ _sequencer->isFlipH(), _flipV ^ _sequencer->isFlipV());
    }
    GOBLIB_INLINE void pushCell(GLovyanGFX* dst, std::int32_t x, std::int32_t y)
    {
        push_cell(dst, _cells[_sequencer->cell()], x, y, _flipH ^ _sequencer->isFlipH(), _flipV ^ _sequencer->isFlipV());
    }
    
    GOBLIB_INLINE void play(goblib::graph::AnimationSequencer* seq){ _sequencer = seq; _sequencer->reset(); }

    /*! 
      apply animation sequencer.
      @retval != nullptr Finished animation sequencer. 
    */
    GOBLIB_INLINE goblib::graph::AnimationSequencer* pump()
    {
        return _sequencer ? ( _sequencer->pump() ? _sequencer : nullptr) : nullptr;
    }
    
  protected:
    std::int32_t _slice_width, _slice_height;
    std::vector<CellRect> _cells;
    bool _pause, _flipH, _flipV;
    goblib::graph::AnimationSequencer* _sequencer;
};

}}
#endif
