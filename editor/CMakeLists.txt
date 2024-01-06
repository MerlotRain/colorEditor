file(GLOB_RECURSE ALL_SOURCE "*.cpp")
file(GLOB_RECURSE ALL_HEAD "*.h")

find_package(Qt6 COMPONENTS Core Gui Widgets Xml REQUIRED)

add_library(colorEditor SHARED ${ALL_SOURCE} ${ALL_HEAD})

generate_export_header(colorEditor
    BASE_NAME COLOR_EDITOR
    EXPORT_FILE_NAME coloreditor_global.h
)

target_include_directories(colorEditor PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}    
    ${CMAKE_CURRENT_BINARY_DIR}
    UI)

target_link_libraries(colorEditor Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Xml)