cmake_minimum_required(VERSION 3.10)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(ProjectName "MD-Sign")
set(CMAKE_INSTALL_PREFIX /usr/local)
set(version 0.1a)
find_library(MONOCYPHER_LIB monocypher)

add_library(mdos-tools-common STATIC
    #src
    common/src/crc32.cpp
    common/src/uuid.cpp
    #headers
    common/include/types.hpp
    common/include/crc32.hpp
    common/include/uuid.hpp
)

add_executable(mdostools-mkcfg
    #src
    mkcfg/src/mkcfg.cpp
    mkcfg/src/front.cpp
    mkcfg/src/back.cpp
    #headers
    mkcfg/include/front.hpp
    mkcfg/include/back.hpp
)

target_link_libraries(mdostools-mkcfg PUBLIC mdos-tools-common)
target_include_directories(mdostools-mkcfg PRIVATE common/include)

add_executable(mdostools-verifycfg
    verifycfg/src/verifycfg.cpp
)

target_link_libraries(mdostools-verifycfg PUBLIC mdos-tools-common)
target_include_directories(mdostools-verifycfg PRIVATE common/include)

install(TARGETS mdostools-mkcfg mdostools-verifycfg RUNTIME DESTINATION bin)
