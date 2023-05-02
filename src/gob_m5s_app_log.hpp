/*!
  @file  gob_m5s_app_log.hpp
  @brief Application logging
  @note depends on esp32-hal-log
  @note Logs below the level specified in APP_LOG_LEVEL are displayed if NDEBUG is not defined.
  @note If APP_LOG_LEVEL is not defined, CORE_DEBUG_LEVEL or LOG_LOCAL_LEVEL is used instead.

  |Level|Description|
  |---|---|
  |0|No display|
  |1|Error|
  |2|Warning|
  |3|Information|
  |4|Debug|
  |5|Verbose|
*/
#ifndef GOBLIB_M5S__LOG_HPP
#define GOBLIB_M5S__LOG_HPP

#if !defined(NDEBUG) || defined(DOXYGEN_PROCESS)
#   pragma message "[AppLog] Enabled"
#   if defined(ARDUINO_ARCH_ESP32)
#       include <esp32-hal-log.h>
#       ifndef APP_LOG_LEVEL
#           if defined(CORE_DEBUG_LEVEL)
#               pragma message "[AppLog] Using CORE_DEBUG_LEVEL"
#               define APP_LOG_LEVEL (CORE_DEBUG_LEVEL)
#           elif  defined(LOG_LOCAL_LEVEL)
#               pragma message "[AppLog] Using LOG_LOCAL_LEVEL"
#               define APP_LOG_LEVEL (LOG_LOCAL_LEVEL)
#           endif
#       else
#           pragma message "[AppLog] Using defined log level"
#       endif
#   endif
/*!
  @def APP_LOGE
  @brief Error log
 */
#   define APP_LOGE(format, ...) do { if(APP_LOG_LEVEL >= ESP_LOG_ERROR)   { log_printf(ARDUHAL_LOG_FORMAT(E, format), ##__VA_ARGS__); } } while(0)
/*!
  @def APP_LOGW
  @brief Warning log
*/
#   define APP_LOGW(format, ...) do { if(APP_LOG_LEVEL >= ESP_LOG_WARN)    { log_printf(ARDUHAL_LOG_FORMAT(W, format), ##__VA_ARGS__); } } while(0)
/*!
  @def APP_LOGI
  @brief Information log
*/
#   define APP_LOGI(format, ...) do { if(APP_LOG_LEVEL >= ESP_LOG_INFO)    { log_printf(ARDUHAL_LOG_FORMAT(I, format), ##__VA_ARGS__); } } while(0)
/*!
  @def APP_LOGD
  @brief Debug log
 */
#   define APP_LOGD(format, ...) do { if(APP_LOG_LEVEL >= ESP_LOG_DEBUG)   { log_printf(ARDUHAL_LOG_FORMAT(D, format), ##__VA_ARGS__); } } while(0)
/*!
  @def APP_LOGV
  @brief Verbose log
 */
#   define APP_LOGV(format, ...) do { if(APP_LOG_LEVEL >= ESP_LOG_VERBOSE) { log_printf(ARDUHAL_LOG_FORMAT(V, format), ##__VA_ARGS__); } } while(0)

#else
   #pragma message "[AppLog] Disabled"
   #define APP_LOGE(format, ...) 
   #define APP_LOGW(format, ...) 
   #define APP_LOGI(format, ...) 
   #define APP_LOGD(format, ...) 
   #define APP_LOGV(format, ...)
#endif

//
#endif
