cmake_dependent_option(SMACK_DUMMY "Build dummy smack library" FALSE "WIN32;CMAKE_SIZEOF_VOID_P EQUAL 4" TRUE)

if(SMACK_DUMMY)
    add_library(smack STATIC
        src/smack.c
    )
    add_library(Smack::Smack ALIAS smack)
    target_include_directories(smack PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>")
else()
    add_library(Smack::Smack SHARED IMPORTED GLOBAL)
    set_property(TARGET Smack::Smack PROPERTY IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/lib/win32/smackw32.lib")
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/smack/lib/win32/SMACKW32.DLL")
        set_property(TARGET Smack::Smack PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/win32/SMACKW32.DLL")
    endif()
    set_property(TARGET Smack::Smack PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include")
endif()
