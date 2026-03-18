cmake_minimum_required(VERSION 3.17)

project(ShitEditor)

if (WIN32 OR (UNIX AND NOT ANDROID))
    add_subdirectory(Package)
endif ()
option(MARK_MAYPACK_FILES "Mark MayPack Files" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(ext/MobDebugCppServer)

if (WIN32 OR (UNIX AND NOT ANDROID))
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
endif ()

set(SRCS )

file(GLOB_RECURSE SRCS "src/*.cpp" "src/*.h")

add_executable(ShitEditor ${SRCS})

target_link_libraries(
        ShitEditor PRIVATE
        MayEngine mobdebugger_cpp_server lualib
)

target_include_directories(
        ShitEditor PRIVATE
        ${CMAKE_SOURCE_DIR}/ext/include
)
target_link_directories(
        ShitEditor PRIVATE
        ${CMAKE_SOURCE_DIR}/ext/lib/${CMAKE_BUILD_TYPE}
)

target_compile_definitions(
        ShitEditor PRIVATE
        MI_SHARED_LIB
        MI_WIN_NOREDIRECT
        MAYENGINE_USE_MLOGV2
        MAY_LOGGING
        MAY_FORCE_CONSOLE
)
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug" OR ${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
    target_compile_definitions(
            ShitEditor PRIVATE
            _DEBUG
    )
endif ()

if (WIN32 OR (UNIX AND NOT ANDROID))
#    add_dependencies(ShitEditor SEPackage)
endif ()
