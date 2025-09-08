cmake_minimum_required(VERSION 3.10)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(ProjectName "MD-Sign")
set(CMAKE_INSTALL_PREFIX /usr/local)
set(version 0.1a)
find_library(MONOCYPHER_LIB monocypher)

add_executable("mkbootcfg" 
    #src
    mkbootcfg/src/mkbootcfg.cpp
    #headers
)

install(TARGETS mkbootcfg RUNTIME DESTINATION bin)
