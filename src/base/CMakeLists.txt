project(base)

file(GLOB SOURCE src/*.cpp)
file(GLOB HEADER include/*.h)

add_library(${PROJECT_NAME} STATIC ${SOURCE} ${HEADER})

find_package(Qt6 REQUIRED COMPONENTS Core Network)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Core Qt6::Network)

target_include_directories(${PROJECT_NAME} PUBLIC include)
