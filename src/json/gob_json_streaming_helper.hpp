/*!
  @file  gob_json_stream_helper.hpp
  @brief json-streaming-parser helper
  @note depends on json-streaming-parser
  @note https://github.com/squix78/json-streaming-parser
*/
#ifndef GOBLIB_JSON_STREAMING_HELPER_HPP
#define GOBLIB_JSON_STREAMING_HELPER_HPP

namespace goblib { namespace json {


// for store value
struct ElemBase
{
    const char* key;
    ElemBase(const char* s) : key(s) {}
    virtual void store(const String& s) {}
};

template<typename T> struct Elem : public ElemBase
{
    T* value;
    Elem(const char* key, T* p) : ElemBase(key), value(p) { assert(p); }
    virtual void store(const String& s) override { _store(s); } // _store function is called based on the type of T. (SFINAE)

    // Integer
    template<typename U = T, typename std::enable_if<!std::is_same<U, bool>::value && std::is_integral<U>::value, std::nullptr_t>::type = nullptr>
    void _store(const String& s)
    {
        *value = static_cast<T>(s.toInt());
    }
    // Floating-point
    template<typename U = T, typename std::enable_if<std::is_floating_point<U>::value, std::nullptr_t>::type = nullptr>
    void _store(const String& s)
    {
        *value = static_cast<T>(s.toFloat());
    }
    // String
    template<typename U = T, typename std::enable_if<std::is_same<U, std::string>::value, std::nullptr_t>::type = nullptr>
    void _store(const String& s)
    {
        *value = s.c_str();
    }
    // Boolean
    template<typename U = T, typename std::enable_if<std::is_same<U, bool>::value, std::nullptr_t>::type = nullptr>
    void _store(const String& s)
    {
        *value = (s.compareTo("true") == 0);
    }

#if 0
    /* ForecastListener::Time_t
       Date time (ISO8601) string to UTC time_t.
       e.g. "2022-11-17T23:00:00+09:00" (UTC+9) / "2022-11-17T23:00:00Z" (UTC)
       NOTICE: strptime() is NOT standard!
     */
    template<typename U = T, typename std::enable_if<std::is_same<U, ForecastListener::Time_t>::value, std::nullptr_t>::type = nullptr>
    void _store(const String& s)
    {
        *value = parseISO8601(s);
    }
#endif
};











//
}}
#endif
