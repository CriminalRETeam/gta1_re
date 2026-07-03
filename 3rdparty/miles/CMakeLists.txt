cmake_dependent_option(MILES_DUMMY "Build dummy miles library" FALSE "WIN32;CMAKE_SIZEOF_VOID_P EQUAL 4" TRUE)

if(MILES_DUMMY)
    add_library(miles STATIC
        src/miles.c
    )
    add_library(Miles::Miles ALIAS miles)
    target_include_directories(miles PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>")
else()
    add_library(Miles::Miles SHARED IMPORTED GLOBAL)
    set_property(TARGET Miles::Miles PROPERTY IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/lib/win32/mss32.lib")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/win32/MSS32.DLL")
        set_property(TARGET Miles::Miles PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/win32/MSS32.DLL")
    endif()
    set_property(TARGET Miles::Miles PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include")
endif()
