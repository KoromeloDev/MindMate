project(QSourceHighliter VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC True)

file(GLOB SOURCE src/*.cpp)
file(GLOB HEADERS include/*.h)
set(PROJECT_SOURCES ${SOURCE} ${HEADERS})
add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC src include)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
