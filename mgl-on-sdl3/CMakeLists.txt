add_library(mgl_on_sdl3 STATIC
    main.c
)
target_compile_definitions(mgl_on_sdl3 PUBLIC SDL_NOLONGLONG)
target_include_directories(mgl_on_sdl3 PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>")
target_link_libraries(mgl_on_sdl3 PRIVATE SDL3::SDL3-shared)
