cmake_minimum_required(VERSION 3.16)
project(QSourceHighliter VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

file(GLOB SOURCE *.cpp)
file(GLOB HEADERS *.h)

set(PROJECT_SOURCES ${SOURCE} ${HEADERS})

add_library(QSourceHighliter ${PROJECT_SOURCES})

target_link_libraries(QSourceHighliter PRIVATE Qt6::Widgets)
