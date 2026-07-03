cmake_dependent_option(DPLAY_DUMMY "Build dummy dplay library" FALSE "WIN32;CMAKE_SIZEOF_VOID_P EQUAL 4" TRUE)

if(DPLAY_DUMMY)
    add_library(dplay STATIC
        dummy/src/dplay.cpp
    )
    add_library(DirectX::DirectPlayX ALIAS dplay)
    target_include_directories(dplay PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/dummy/include>")
else()
    if(MINGW)
        add_library(DirectX::DirectPlayX INTERFACE IMPORTED GLOBAL)
        set_property(TARGET DirectX::DirectPlayX PROPERTY INTERFACE_LINK_LIBRARIES "dplayx")
    else()
        set(DPLAYX_DEF "${CMAKE_CURRENT_SOURCE_DIR}/dxsdk/lib/win32/dplayx.def")
        add_library(DirectX::DirectPlayX SHARED IMPORTED GLOBAL)
        set_property(TARGET DirectX::DirectPlayX PROPERTY IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/dxsdk/lib/win32/dplayx.lib")
        set_property(TARGET DirectX::DirectPlayX PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/dxsdk/include")
    endif()
endif()
