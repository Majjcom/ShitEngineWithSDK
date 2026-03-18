cmake_minimum_required(VERSION 3.17)

project(MayGame)

if (WIN32 OR (UNIX AND NOT ANDROID))
    add_subdirectory(Package)
endif ()
option(MARK_MAYPACK_FILES "Mark MayPack Files" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32 OR (UNIX AND NOT ANDROID))
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
endif ()

set(SRCS )

file(GLOB_RECURSE SRCS "src/*.cpp" "src/*.h")

add_executable(MayGame ${SRCS})

set_target_properties(MayGame PROPERTIES OUTPUT_NAME "ShitGame")

target_link_libraries(
        MayGame PRIVATE
        MayEngine
)

target_include_directories(
        MayGame PRIVATE
        ${CMAKE_SOURCE_DIR}/ext/include
)
target_link_directories(
        MayGame PRIVATE
        ${CMAKE_SOURCE_DIR}/ext/lib/${CMAKE_BUILD_TYPE}
)

target_compile_definitions(
        MayGame PRIVATE
        MI_SHARED_LIB
        MI_WIN_NOREDIRECT
        MAYENGINE_USE_MLOGV2
        MAY_LOGGING
        MAY_FORCE_CONSOLE
)
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug" OR ${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
    target_compile_definitions(
            MayGame PRIVATE
            _DEBUG
    )
endif ()

add_dependencies(MayGame Package)
