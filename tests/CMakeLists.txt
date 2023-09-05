project(Tests)

find_package(Qt6 REQUIRED COMPONENTS Test Gui)

file(GLOB SOURCE_BASE base/*.cpp)

foreach(testFile ${SOURCE_BASE})
    get_filename_component(testFilename "${testFile}" NAME_WLE)

    add_executable("${testFilename}" "${testFile}")
    target_link_libraries("${testFilename}" PRIVATE Qt6::Test MindMate)
endforeach()

file(GLOB SOURCE src/*.cpp)

foreach(testFile ${SOURCE})
    get_filename_component(testFilename "${testFile}" NAME_WLE)

    add_executable("${testFilename}" "${testFile}")
    target_link_libraries("${testFilename}" PRIVATE Qt6::Test Qt6::Gui MindMate)
endforeach()
