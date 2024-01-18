cmake_minimum_required(VERSION 3.20)
project(colorEditor LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include(GenerateExportHeader)

if(BUILD_WITH_QT5)
    set(QT_VERSION_BASE Qt5)
elseif(BUILD_WITH_QT6)
    set(QT_VERSION_BASE Qt6)
endif()

add_subdirectory(editor)
add_subdirectory(examples)
