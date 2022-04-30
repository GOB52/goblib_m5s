/*!
  goblib for M5Stack (Depends on M5Stack releated)

  @file  gob_m5s_sd.hpp
  @brief Wrapping SD card system
  @remark Depends on SdFat
*/
#ifndef GOBLIB_M5S_SD_HPP
#define GOBLIB_M5S_SD_HPP
#include <SdFat.h>
#include <gob_singleton.hpp>


namespace goblib { namespace m5s {

// File definition.
#if   SDFAT_FILE_TYPE == 1
//#pragma message "SDFAT_FILE_TYPE 1"
using File = File32;

#elif SDFAT_FILE_TYPE == 2
//#pragma message "SDFAT_FILE_TYPE 2"
using File = ExFile;

#elif SDFAT_FILE_TYPE == 3
//#pragma message "SDFAT_FILE_TYPE 3"
using File = FsFile;

#else
#error "SDFAT_FILE_TYPE unknown"
#endif

/*!
  @brief Wrapping and singleton SdFat instance.
*/
class SD : public goblib::Singleton<SD>
{
  public:
    virtual ~SD(){}

    SdFat& sd() { return _sd; }
    
  protected:
    friend class Singleton<SD>;
    SD() : Singleton<SD>(), _sd() {}

  private:
    SdFat _sd;
};

//
}}
#endif

