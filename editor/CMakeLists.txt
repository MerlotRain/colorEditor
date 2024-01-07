file(GLOB_RECURSE ALL_SOURCE "*.cpp")
file(GLOB_RECURSE ALL_HEAD "*.h")

find_package(${QT_VERSION_BASE} COMPONENTS Core Gui Widgets REQUIRED)

add_library(colorEditor SHARED ${ALL_SOURCE} ${ALL_HEAD})

generate_export_header(colorEditor
    BASE_NAME COLOR_EDITOR
    EXPORT_FILE_NAME coloreditor_global.h
)

target_include_directories(colorEditor PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}    
    ${CMAKE_CURRENT_BINARY_DIR}
    resources.qrc
    )

target_link_libraries(colorEditor 
    ${QT_VERSION_BASE}::Core 
    ${QT_VERSION_BASE}::Gui 
    ${QT_VERSION_BASE}::Widgets)