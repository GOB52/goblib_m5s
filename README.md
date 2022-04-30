# goblib_m5s
[Japanese/日本語](README.ja.md)  
Library uses STL and depends on M5Stack

## Overview
It is intended for use in environments where C++11 or later can be compiled.

## Require
[M5Stack](https://github.com/m5stack/M5Stack) 0.3.9 or later  
[LovyanGFX](https://github.com/lovyan03/LovyanGFX) 0.4.17 (support v0,v1)  
[SdFat](https://github.com/greiman/SdFat) 2.1.2 or later  
[goblib](https://github.com/GOB52/goblib) 0.1.0 or later

## How to install
Please git clone to the appropriate location in your environment, or download and extract the ZIP file.  
It depends on your development environment whether or not you need to add an option to the compiler to indicate the appropriate location.

## How to make document

You can make a document of this library by [Doxygen](https://www.doxygen.nl/)  => [Doxyfile](doc/Doxyfile)  
In case you use shell script [doxy.sh](doc/doxy.sh), then output version from library.properties, revision from repository.
