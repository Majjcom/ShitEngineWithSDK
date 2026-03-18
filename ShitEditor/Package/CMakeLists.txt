cmake_minimum_required(VERSION 3.10)

project(MayGame)

add_custom_target(
        SEPackage
        DEPENDS sedata.mp
)

set(ADDITIONAL_CMD )

if (NOT ${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(ADDITIONAL_CMD -v)
endif ()

add_custom_command(
        OUTPUT sedata.mp
        COMMAND ${CMAKE_SOURCE_DIR}/tools/MayPack.exe --pack --sign -i ${CMAKE_CURRENT_SOURCE_DIR}/res -o ${CMAKE_BINARY_DIR}/bin/sedata.mp --keycache=${CMAKE_CURRENT_SOURCE_DIR}/kcache.json --keypath=${CMAKE_CURRENT_SOURCE_DIR}/pri.key --key=0XUDGMMi
)
