project(QSourceHighliter LANGUAGES CXX)

set(CMAKE_AUTOMOC true)

file(GLOB SOURCE src/*.cpp)
file(GLOB HEADER include/*.h)
set(PROJECT_SOURCES ${SOURCE} ${HEADER})

add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCES})

find_package(Qt6 REQUIRED COMPONENTS Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

target_include_directories(${PROJECT_NAME} PUBLIC include)
