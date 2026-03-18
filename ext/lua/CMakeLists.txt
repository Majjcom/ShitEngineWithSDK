cmake_minimum_required(VERSION 3.17)

project(lualib)

set(SRCS )

aux_source_directory(lua SRCS)

add_library(lualib STATIC ${SRCS})
target_include_directories(lualib PUBLIC ./)
