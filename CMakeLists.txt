cmake_minimum_required(VERSION 3.10)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(ProjectName "MD-Sign")
set(CMAKE_INSTALL_PREFIX /usr/local)
set(version 0.1a)
find_library(MONOCYPHER_LIB monocypher)

add_executable("mkbootcfg" 
    #src
    mkbootcfg/src/mkbootcfg.cpp
    mkbootcfg/src/crc32.cpp
    mkbootcfg/src/uuid.cpp
    mkbootcfg/src/front.cpp
    mkbootcfg/src/back.cpp
    #headers
    mkbootcfg/include/crc32.hpp
    mkbootcfg/include/uuid.hpp
    mkbootcfg/include/types.hpp
    mkbootcfg/include/front.hpp
    mkbootcfg/include/back.hpp
)

install(TARGETS mkbootcfg RUNTIME DESTINATION bin)
