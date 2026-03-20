cmake_minimum_required(VERSION 3.17)

project(ShitLang)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    if (${CMAKE_BUILD_TYPE} STREQUAL "Debug" OR ${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
    else ()
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
    endif ()
    set(MSVC_OPTIMIZE_FLAGS "/GL /Oi /arch:AVX2")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${MSVC_OPTIMIZE_FLAGS}")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${MSVC_OPTIMIZE_FLAGS}")
endif ()

aux_source_directory(lua LIB_SRCS)
add_library(libShitLang STATIC ${LIB_SRCS})

set(COMPILER_SRCS tool/compiler.cpp)
add_executable(ShitCompiler ${COMPILER_SRCS})
target_link_libraries(
        ShitCompiler PRIVATE
        libShitLang
)
target_include_directories(
        ShitCompiler PRIVATE
        lua
)

set(SHITRUNNER_SRCS tool/lua.c)
add_executable(ShitLang ${SHITRUNNER_SRCS})
target_link_libraries(
        ShitLang PRIVATE
        libShitLang
)
target_include_directories(
        ShitLang PRIVATE
        lua
)

set(COMPILE_TOOL_SRCS tool/tool.cpp)
add_executable(ShitCompilerTool ${COMPILE_TOOL_SRCS})
target_include_directories(
        ShitCompilerTool PRIVATE
        lua
)
add_dependencies(
        ShitCompilerTool ShitCompiler
)
