/*!
  goblib for M5Stack (Depends on M5Stack releated)

  @file   gob_lgfx_animated_palette.hpp
  @brief  Palette animation helper for LGFX_Sprite.
  @note Depends on LovyanGFX
*/
#pragma once
#ifndef GOBLIB_ANIMATED_PALETTE_HPP
#define GOBLIB_ANIMATED_PALETTE_HPP

#include "gob_lgfx.hpp"

#include <cstdint>
#include <cstddef>
#include <vector>
#include <algorithm>

#include <gob_easing.hpp>
#include <gob_macro.hpp>

namespace goblib { namespace lgfx {

/*!
  @brief Animated palette color helper.
*/
class AnimatedPalette
{
  public:
    /*!
      @param sz size of palette colors.(4bit sprite = 16, 8bit = 256)
      @param attach Attach sprite
    */
    explicit AnimatedPalette(std::size_t sz, goblib::lgfx::GSprite* attach = nullptr)
            : _current(sz)
            , _save(sz)
            , _sequences()
            , _sprite(attach)
            , _pause(false)
    {
        _sequences.reserve(sz);
        if(attach) { from(*attach); }
    }
    ~AnimatedPalette(){}

    GOBLIB_INLINE goblib::lgfx::GSprite* sprite() const { return _sprite; }
    GOBLIB_INLINE bool isPause() const { return _pause; }
    GOBLIB_INLINE void pause(bool b) { _pause = b; }
    GOBLIB_INLINE void pause() { pause(true); }
    GOBLIB_INLINE void resume() { pause(false); }

    GOBLIB_INLINE bool empty() const { return _sequences.empty(); }
    /*! Clear sequences */
    GOBLIB_INLINE void clear() { _sequences.clear(); }

    /// @name For single palette
    /// @{
    /*!
      Animate the current color towards the specified color.
      @tparam T Color type.
      @param idx Palette index.
      @param color Toward to color.
      @param times Times to finish.(Unit time is pump count)
      @note Failed if idx exists in sequences.
    */
    template<typename T> bool toward(std::uint32_t idx, T color, std::int32_t times)
    {
        if(std::any_of(_sequences.begin(), _sequences.end(), [&idx](const Sequence& s) { return s._index == idx; }))
        {
            assert(0 && "Already regsitered");
            return false;
        }
        assert(_sequences.size() <= _current.size() && "Overflow");
        _sequences.emplace_back(idx, _current[idx], RGBColor(color), times);
        return true;
    }

    /*!
      Animate back and forth between the current color and a specific color.
      @tparam T Color type.
      @param idx Palette index.
      @param color Toward to color.
      @param times Times to finish.(Unit time is pump count)
      @warning Processing infinite until call clear().
      @note Failed if idx exists in sequences.
    */
    template<typename T> bool roundTrip(std::uint32_t idx, T color, std::int32_t times)
    {
        if(std::any_of(_sequences.begin(), _sequences.end(), [&idx](const Sequence& s) { return s._index == idx; }))
        {
            assert(0 && "Already regsitered");
            return false;
        }
        assert(_sequences.size() <= _current.size() && "Overflow");
        _sequences.emplace_back(idx, _current[idx], RGBColor(color), times, true);
        return true;
    }
    /// @}

    /// @name For all palettes
    /// @{
    /*!
      Fade all palettes toward from current to clr.
    */
    void fadeAll(RGBColor clr, std::int32_t times)
    {
        clear();
        std::uint32_t len = _current.size();
        for(std::uint32_t i=0;i<len;++i) { toward(i, clr, times); }
    }
    /*!
      Fade all palettes toward from current to saved palettes.
    */
    void fadeAll(std::int32_t times)
    {
        clear();
        std::uint32_t len = _current.size();
        for(std::uint32_t i=0;i<len;++i) { toward(i, _save[i], times); }
    }
    /*! Fade all palettes from RGB(0,0,0) to saved colors. */
    GOBLIB_INLINE void fadeIn(std::int32_t times)
    {
        fill(RGBColor(0,0,0));
        fadeAll(times);
    }
    /*! Fade all palettes from current to RGB(0,0,0) */
    GOBLIB_INLINE void fadeOut(std::int32_t times)
    {
        fadeAll(RGBColor(0,0,0), times);
    }
    /// @}

    /// @name Palette I/O
    /// @{
    /*! Set current color to sprite */
    void to(goblib::lgfx::GSprite& dst);
    /*! Set current color to dest */
    void to(RGBColor* dest, std::size_t sz);
    /*! Get palette colors from sprite to current,save */
    void from(goblib::lgfx::GSprite& src);
    /*! Get palette colors from src to current,save */
    void from(RGBColor* src, std::size_t sz);
    /*! Get palette colors from attached sprite to current,save */
    void apply() { if(_sprite) { from(*_sprite); } }
    /*! Fill color to current */
    GOBLIB_INLINE void fill(const RGBColor& clr)
    {
        std::fill(_current.begin(), _current.end() , clr);
    }
    /// @}
    
    /*!
      Pump sequence.
      @retval ==0 Nothing processed.
      @retval !=0 Number of processed.
    */
    std::size_t pump();

  protected:
    /*! @brief Animated palette color sequence */
    class Sequence
    {
      public:
        /*!
          @param index Palette index
          @param from Start color
          @param to Toward color
          @param times Times to finish if roundTrip is false.(Unit time is pump count)
          @param roundTrip true: roundtrip between start and to.
        */
        Sequence(std::int32_t index, const RGBColor& from, const RGBColor& to, std::int32_t times, bool roundTrip = false)
                : _index(index), _times(times), _counter(0), _toward(0),
                  _start(from), _now(from), _to{to, from}, _roundTrip(roundTrip)
        {
            _diff[0] = static_cast<std::int16_t>(to.r) - static_cast<std::int16_t>(from.r);
            _diff[1] = static_cast<std::int16_t>(to.g) - static_cast<std::int16_t>(from.g);
            _diff[2] = static_cast<std::int16_t>(to.b) - static_cast<std::int16_t>(from.b);
        }

        GOBLIB_INLINE operator RGBColor() const { return _now; }
        bool pump();

      private:
        std::uint32_t _index;   //!< Palette index
        std::int32_t _times;    //!< toward times
        std::int32_t _counter;
        std::uint32_t _toward; // toward direction
        RGBColor _start, _now;
        std::int16_t _diff[4];
        RGBColor _to[2]; // [0]:toward  [1]:return (if roundtrip)
        bool _roundTrip;
        friend class AnimatedPalette;
    };

  private:
    std::vector<RGBColor> _current; // current color
    std::vector<RGBColor> _save;    // saved color
    std::vector<Sequence> _sequences;   // animated sequences
    goblib::lgfx::GSprite* _sprite;     // attached sprite.
    bool _pause;
};


}}
#endif
