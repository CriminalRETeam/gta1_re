cmake_minimum_required(VERSION 3.27)
project(regta C CXX)

include(CMakeDependentOption)

option(MGL_ON_SDL3 "Run MGL on top of SDL3" ON)

if(CMAKE_C_COMPILER_ID MATCHES "MSVC" AND CMAKE_C_COMPILER_VERSION VERSION_LESS "13")
    include_directories("${PROJECT_SOURCE_DIR}/3rdparty/vc6/include")
    add_compile_definitions("-Dinline=__inline")
    add_compile_definitions("-D__func__=\"FUNC\"")
    add_compile_definitions("-DWINBOOL=int")
    add_link_options("/FIXED")
    add_link_options("/INCREMENTAL:NO")
endif()

if(WIN32)
    add_library(Miles::Miles SHARED IMPORTED)
    set_property(TARGET Miles::Miles PROPERTY IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/miles/lib/win32/mss32.lib")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/miles/lib/win32/MSS32.DLL")
        set_property(TARGET Miles::Miles PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/miles/lib/win32/MSS32.DLL")
    endif()
    set_property(TARGET Miles::Miles PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/miles/include")

    add_library(Smack::Smack SHARED IMPORTED)
    set_property(TARGET Smack::Smack PROPERTY IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/smack/lib/win32/smackw32.lib")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/smack/lib/win32/SMACKW32.DLL")
        set_property(TARGET Smack::Smack PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/smack/lib/win32/SMACKW32.DLL")
    endif()
    set_property(TARGET Smack::Smack PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/smack/include")

    if(MINGW)
        add_library(DirectX::DirectPlayX INTERFACE IMPORTED)
        set_property(TARGET DirectX::DirectPlayX PROPERTY INTERFACE_LINK_LIBRARIES "dplayx")
    else()
        set(DPLAYX_DEF "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/dxsdk/lib/win32/dplayx.def")
        add_library(DirectX::DirectPlayX SHARED IMPORTED)
        set_property(TARGET DirectX::DirectPlayX PROPERTY IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/dxsdk/lib/win32/dplayx.lib")
        set_property(TARGET DirectX::DirectPlayX PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/dxsdk/include")
    endif()
    add_custom_target(create-implibs DEPENDS ${generated_implibs})
endif()

if(MGL_ON_SDL3)
    find_package(SDL3 REQUIRED CONFIG COMPONENTS SDL3-shared)
    add_subdirectory(mgl-on-sdl3 EXCLUDE_FROM_ALL)
    add_library(MGL::MGL ALIAS mgl_on_sdl3)
else()
    if(WIN32)
        add_library(MGL::MGL STATIC IMPORTED)
        set_property(TARGET MGL::MGL PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/scitech/lib/win32/vc/mgllt.lib")
        set_property(TARGET MGL::MGL PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/scitech/include")
    endif()
endif()

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 98)

add_executable(regta WIN32
    src/city.cpp
    src/city.h
    src/cutscene.cpp
    src/cutscene.h
    src/error.cpp
    src/error.h
    src/font.cpp
    src/font.h
    src/game.cpp
    src/game.h
    src/graphics.cpp
    src/graphics.h
    src/input.cpp
    src/input.h
    src/interface.cpp
    src/interface.h
    src/main.cpp
    src/main.h
    src/map.cpp
    src/map.h
    src/math_lut.cpp
    src/math_lut.h
    src/menu.cpp
    src/menu.h
    src/mission.cpp
    src/mission.h
    src/multiplayer.cpp
    src/multiplayer.h
    src/save.cpp
    src/save.h
    src/sound.cpp
    src/sound.h
    src/text.cpp
    src/text.h
    src/util.cpp
    src/util.h
)
target_link_libraries(regta PRIVATE MGL::MGL)
target_link_libraries(regta PRIVATE Miles::Miles)
target_link_libraries(regta PRIVATE Smack::Smack)
target_link_libraries(regta PRIVATE DirectX::DirectPlayX)
target_link_libraries(regta PRIVATE winmm)
if(TARGET create-implibs)
    add_dependencies(regta create-implibs)
endif()
if(MGL_ON_SDL3)
    target_link_libraries(regta PRIVATE SDL3::Headers)
endif()

cmake_dependent_option(REGTA_COPY_RUNTIME_DEPS "Copy regta runtime dependencies" OFF "WIN32" OFF)
if(REGTA_COPY_RUNTIME_DEPS)
    add_custom_command(POST_BUILD TARGET regta COMMAND "${CMAKE_COMMAND}" -E copy_if_different $<TARGET_FILE:Miles::Miles> $<TARGET_FILE_DIR:regta>)
    add_custom_command(POST_BUILD TARGET regta COMMAND "${CMAKE_COMMAND}" -E copy_if_different $<TARGET_FILE:Smack::Smack> $<TARGET_FILE_DIR:regta>)
    if(MGL_ON_SDL3)
        add_custom_command(POST_BUILD TARGET regta COMMAND "${CMAKE_COMMAND}" -E copy_if_different $<TARGET_FILE:SDL3::SDL3-shared> $<TARGET_FILE_DIR:regta>)
    endif()
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "14")
        target_compile_options(regta PRIVATE "-GX")
    endif()
endif()
