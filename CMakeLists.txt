cmake_minimum_required(VERSION 3.5)

project(lab2 LANGUAGES C)

set(CMAKE_C_EXTENSIONS OFF)

add_executable(lab2 main.c)

if (CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    target_compile_options(lab2 PRIVATE -Wall -Werror -O2)
endif()

find_library(AKRYPT_BASE_LIB NAMES akrypt-base HINTS "/home/mainuser/krypt/libakrypt/build")
find_library(AKRYPT_LIB NAMES akrypt HINTS "/home/mainuser/krypt/libakrypt/build")

if (AKRYPT_BASE_LIB AND AKRYPT_LIB)
    target_link_libraries(lab2 PRIVATE ${AKRYPT_BASE_LIB} ${AKRYPT_LIB})

    target_include_directories(lab2 PRIVATE
        "/home/mainuser/krypt/libakrypt/libakrypt-0.x/source/"
        "/home/mainuser/krypt/libakrypt/build"
    )
else()
    message(FATAL_ERROR "Required libraries not found: akrypt-base or akrypt")
endif()
