/*!
  goblib for M5Stack (Depends on M5Stack releated)

  @file  gob_m5s_stream.hpp
  @brief Stream for Arduino
  @remark Depends on SdFat
*/
#if defined(ARDUINO)
#pragma once
#ifndef GOBLIB_M5S_STREM_HPP
#define GOBLIB_M5S_STREM_HPP

#include <gob_macro.hpp>
#include <gob_stream.hpp>
#include <gob_m5s_sd.hpp>

namespace goblib { namespace m5s {

/*!
  @brief SD card file stream.
*/
class FileStream : public Stream
{
  public:
    using pos_type = typename Stream::pos_type;
    using off_type = typename Stream::off_type;

    static_assert( sizeof(std::size_t) <= sizeof(pos_type), "pos_type is not enough to represent position");

    FileStream() : Stream() {}
    virtual ~FileStream() { _file.close(); }

    /// @name Open, Close
    /// @{
    GOBLIB_INLINE virtual bool is_open() const override { return static_cast<bool>(_file); }
    GOBLIB_INLINE virtual bool open(const char* path) override
    {
        if(path && path[0]) { close(); return _file.open(path, O_RDONLY); }
        return false;
    }
    GOBLIB_INLINE virtual void close() override { _file.close(); }
    /// @}

    /// @name Property
    /// @{
    GOBLIB_INLINE virtual pos_type size() const { return const_cast<File*>(&_file)->size(); }
    /// @}
    
    /// @name Read
    /// @{
    template<typename U> GOBLIB_INLINE pos_type read(U buf, std::size_t len)
    {
        return read(reinterpret_cast<std::uint8_t*>(buf), len);
    }
    GOBLIB_INLINE virtual pos_type read(std::uint8_t* buf, std::size_t len) override
    {
        if(!is_open() || len == 0) { return 0; }
        return static_cast<pos_type>(_file.read(buf, len));
    }
    /// @}

    /// @name Seek
    /// @{
    virtual bool seek(off_type off, seekdir s) override
    {
        if(!is_open()) { return false; }
        switch(s)
        {
        case seekdir::beg: return _file.seekSet(off);
        case seekdir::cur: return _file.seekCur(off);
        case seekdir::end: return _file.seekEnd(off);
        }
        return false;
    }
    GOBLIB_INLINE virtual pos_type position() const override { return const_cast<File*>(&_file)->position(); }
    GOBLIB_INLINE virtual bool is_tail() const override { return position() >= const_cast<File*>(&_file)->size(); }
    /// @}
    
  private:
    goblib::m5s::File _file;
};

//
}}
#endif
#endif
