# Set VCPKG_ROOT to your vcpkg installation directory or add the following to your cmake call:
# -DCMAKE_TOOLCHAIN=<path_to_vcpkg>/scripts/buildsystems/vcpkg.cmake

cmake_minimum_required(VERSION 3.16)
set(CMAKE_VERBOSE_MAKEFILE ON)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")  
endif()
if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
  set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
endif()

project(cpp-modern LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
# set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

find_package(Catch2 3)

if(NOT Catch2_FOUND)
  Include(FetchContent)

  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.6.0 # or a later release
  )
  FetchContent_MakeAvailable(Catch2)
endif()

enable_testing()

# include(CTest)
include(Catch)

add_subdirectory(helpers)
add_subdirectory(concepts)
add_subdirectory(ranges)
add_subdirectory(compile-time-programming)
add_subdirectory(compare)
add_subdirectory(type-deduction)
add_subdirectory(type-traits)
add_subdirectory(sfinae)
add_subdirectory(vocabulary-types)
add_subdirectory(move-semantics)
add_subdirectory(structured-bindings)

# Exercises
add_subdirectory(_exercises/ex-compare)
add_subdirectory(_exercises/ex-concepts)
add_subdirectory(_exercises/ex-ranges)
add_subdirectory(_exercises/ex-auto-declarations)
add_subdirectory(_exercises/ex-fold-expressions)
add_subdirectory(_exercises/ex-optional)
add_subdirectory(_exercises/ex-string-view)
add_subdirectory(_exercises/ex-variant)
add_subdirectory(_exercises/ex-ctad)
add_subdirectory(_exercises/ex-enable-if)
add_subdirectory(_exercises/ex-move-semantics)