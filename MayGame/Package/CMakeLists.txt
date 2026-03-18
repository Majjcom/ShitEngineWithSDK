cmake_minimum_required(VERSION 3.10)

project(MayGame)

add_custom_target(
        Package
        DEPENDS res shit data.mp
)

set(ADDITIONAL_CMD )

if (NOT ${CMAKE_BUILD_TYPE} STREQUAL "Release")
        set(ADDITIONAL_CMD -v)
endif ()

add_custom_command(
        OUTPUT data.mp
        COMMAND ${CMAKE_BINARY_DIR}/bin/createPackage.bat
)

add_custom_target(
        CompileLuaCode
        DEPENDS compiled_lua
)

add_dependencies(
        Package
        CompileLuaCode
)

add_custom_command(
        OUTPUT compiled_lua
        COMMAND ${CMAKE_SOURCE_DIR}/tools/MayLuaCompileTool ${CMAKE_SOURCE_DIR}/tools/MayLuaCompiler ${CMAKE_CURRENT_SOURCE_DIR}/lua ${CMAKE_CURRENT_SOURCE_DIR}/res
)

add_custom_target(
        res
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/bin/Package/res
        DEPENDS CompileLuaCode
)

add_custom_target(
        shit
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shit ${CMAKE_BINARY_DIR}/bin/Package/shit
)
