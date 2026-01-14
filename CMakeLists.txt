cmake_minimum_required(VERSION 4.2.1)

project(name
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*
)

add_executable(bin ${SOURCES})

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(bin PRIVATE
        -Weverything
        -Wno-c++98-compat
        -Wno-c++98-compat-pedantic
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(bin PRIVATE
        -Wall
    )
endif()

option(ENABLE_WERROR "Treat warnings as errors" ON)
if(ENABLE_WERROR)
    target_compile_options(bin PRIVATE -Werror)
endif()
