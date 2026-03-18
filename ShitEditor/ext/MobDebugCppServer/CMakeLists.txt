cmake_minimum_required(VERSION 3.30)
project(mobdebugger_cpp_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    set(IS_ROOT_PROJECT ON)
    add_subdirectory(ext/boost)
else ()
    set(IS_ROOT_PROJECT OFF)
endif ()

add_library(mobdebugger_cpp_server STATIC src/mobdebug_server.cpp)

target_include_directories(
        mobdebugger_cpp_server PUBLIC
        include
)
target_link_libraries(
        mobdebugger_cpp_server PRIVATE
        boost_asio
        boost_regex
)
if (MSVC)
    target_compile_options(
            mobdebugger_cpp_server PRIVATE
            /utf-8
    )
endif ()

if (IS_ROOT_PROJECT)
    add_executable(debugger_test test/main.cpp)
    target_link_libraries(debugger_test PRIVATE mobdebugger_cpp_server)
    if (MSVC)
        target_compile_options(debugger_test PRIVATE /utf-8)
    endif ()
endif ()
