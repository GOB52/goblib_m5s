/*!
  goblib for M5Stack (Depends on M5Stack releated)

  @file   gob_m5s_faces.hpp
  @brief  Get input from the Faces.
  @todo Support FaceKeyborad and FaceCalculator
*/
#pragma once
#ifndef GOBLIB_M5S_FACES_HPP
#define GOBLIB_M5S_FACES_HPP

#include <gob_macro.hpp>
#include <cstdint>
#include <array> // std::array

#ifdef GOBLIB_TEST_STUB
#include <queue>
#endif

namespace goblib { namespace m5s {

/*!
  @brief Get input from the Game Boy-like panel of M5Stack Faces.
  @remark  Support software repeat.
  @todo Implement pressedFor().

*/
class FaceGB
{
  public:
    /*! @enum GBButton
      @brief Button value.
    */
    enum Button : std::uint8_t
    {
        Up =        0x01,   //!< Up (cross)
        Down =      0x02,   //!< Down (cross)
        Left =      0x04,   //!< Left (cross)
        Right =     0x08,   //!< Right (cross)
        A =         0x10,   //!< A (button)
        B =         0x20,   //!< B (button)
        Select =    0x40,   //!< Select (system button)
        Start =     0x80,   //!< Start (system button)
    };
    using ButtonType = std::underlying_type<Button>::type;
    
    /// @name Mask for button
    /// @{
    constexpr static ButtonType MASK_CROSS = (Button::Up | Button::Down | Button::Left | Button::Right);
    constexpr static ButtonType MASK_BUTTON = (Button::A | Button::B);
    constexpr static ButtonType MASK_SYSTEM = (Button::Select | Button::Start);
    constexpr static ButtonType MASK_ALL = (MASK_CROSS | MASK_BUTTON | MASK_SYSTEM);
    /// @}
    
    /// @name constants
    /// @brief default value for constructor.
    /// @{
    constexpr static std::uint8_t PIN_NO = 5;
    constexpr static std::uint8_t I2C_ADDRESS = 0x08;
    /// @}

    /*!
      @param i2c_address Address of Faces
      @param pin_no Pin No. of Faces.
    */
    FaceGB(std::uint8_t i2c_address = I2C_ADDRESS, std::uint8_t pin_no = PIN_NO);

    /*!
      call once before call pump().
    */
    void setup();
    /*!
      Update the button state for each call.
    */
    void pump();

    /// @name Check
    /// @{
    GOBLIB_INLINE bool isPressed(const ButtonType& b) const {       return getNow()  & b; }
    GOBLIB_INLINE bool isPressedEqual(const ButtonType& b) const {  return getNow() == b; }

    GOBLIB_INLINE bool wasPressed(const ButtonType& b) const {      return getEdge()  & b; }
    GOBLIB_INLINE bool wasPressedEqual(const ButtonType& b) const { return getEdge() == b; }

    GOBLIB_INLINE bool wasReleased(const ButtonType& b) const {     return getReleaseEdge()  & b; }
    GOBLIB_INLINE bool wasReleasedEqual(const ButtonType& b) const {return getReleaseEdge() == b; }
    
    GOBLIB_INLINE bool wasRepeated(const ButtonType& b) const {     return getRepeat()  & b; }
    GOBLIB_INLINE bool wasRepeatedEqual(const ButtonType& b) const {return getRepeat() == b; }
    /// @}

    /// @name Repeat cycle
    /// @{
    std::uint8_t getRepeatCycle(const Button btn) const;
    /*! Set software repeat cycle.
      @param btn Target button
      @param cnt Cycle
      @note Cycle unit is per call pump().
    */
    void setRepeatCycle(const Button btn, const std::uint8_t cnt);
    void setRepeatCycleAll(const std::uint8_t cnt);
    /// @}

#ifdef GOBLIB_TEST_STUB
    void setStubData(const ButtonType* data, std::size_t length);
    std::uint8_t readStubData();
#endif

    /// @name Get raw
    /// @{
    GOBLIB_INLINE ButtonType getNow() const         { return _now; }
    GOBLIB_INLINE ButtonType getLast() const        { return _last; }
    GOBLIB_INLINE ButtonType getEdge() const        { return _edge; }
    GOBLIB_INLINE ButtonType getReleaseEdge() const { return _edge_r; }
    GOBLIB_INLINE ButtonType getRepeat() const      { return _repeat; }
    /// @}

  private:
    std::uint8_t _i2c_address;
    std::uint8_t _pin_no;

    ButtonType _last;
    ButtonType _now;
    ButtonType _edge;
    ButtonType _edge_r;
    ButtonType _repeat;

    std::array<std::uint8_t, 8> _repeatCount;
    std::array<std::uint8_t, 8> _repeatCycle;

#ifdef GOBLIB_TEST_STUB
    std::queue<ButtonType> _record;
#endif

#if defined(GOBLIB_CPP14_OR_EARLIER)
    static const std::uint8_t REPEAT_CYCLE_OF_PUMP;
#else
    static constexpr std::uint8_t REPEAT_CYCLE_OF_PUMP = 4;
#endif
};

//
}}
#endif
