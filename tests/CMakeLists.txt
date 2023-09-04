project(Tests)
file(GLOB SOURCE src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCE})

find_package(Qt6 REQUIRED COMPONENTS Test Gui)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Test Qt6::Gui MindMate)
