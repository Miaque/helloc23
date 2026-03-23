cmake_minimum_required(VERSION 4.2)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
set(CMAKE_CXX_FLAGS_INIT "-stdlib=libc++")

project(hello LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_executable(hello)

target_sources(hello
        PRIVATE
        main.cpp
        PRIVATE
        FILE_SET CXX_MODULES FILES
        employee.cppm
)

target_compile_options(hello PRIVATE -stdlib=libc++)
target_link_options(hello PRIVATE -stdlib=libc++)

set_property(TARGET hello PROPERTY CXX_MODULE_STD ON)