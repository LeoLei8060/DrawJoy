cmake_minimum_required(VERSION 3.16)
project(DrawJoy VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/utf-8)
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/canvas.cpp
    src/canvas.h
    src/shapes/shape.h
    src/shapes/polyline.h
    src/shapes/polyline.cpp
    src/shapes/ellipse.h
    src/shapes/ellipse.cpp
    src/shapes/rectangle.h
    src/shapes/rectangle.cpp
    src/shapes/arrow.h
    src/shapes/arrow.cpp
    src/shapes/freehand.h
    src/shapes/freehand.cpp
    src/shapes/highlighter.h
    src/shapes/highlighter.cpp
    src/shapes/mosaic.h
    src/shapes/mosaic.cpp
    src/shapes/text.h
    src/shapes/text.cpp
    resources.qrc
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
)
