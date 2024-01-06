cmake_minimum_required(VERSION 3.20)
project(colorEditor LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include(GenerateExportHeader)

add_subdirectory(editor)
add_subdirectory(examples)