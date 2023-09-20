project(Tests)

find_package(Qt6 REQUIRED COMPONENTS Test Gui)

file(GLOB SOURCE_BASE base/*.cpp)
set(SOURCE_NAME "")
set(LAST_FILE "")

foreach(testFile ${SOURCE_BASE})
    get_filename_component(testFilename ${testFile} NAME_WLE)
    string(APPEND SOURCE_NAME "${BUILD_PATH}/${testFilename}.exe;")
    add_executable(${testFilename} ${testFile})
    target_link_libraries(${testFilename} PRIVATE Qt6::Test MindMate)
endforeach()

file(GLOB SOURCE src/*.cpp)

foreach(testFile ${SOURCE})
    get_filename_component(testFilename ${testFile} NAME_WLE)
    string(APPEND SOURCE_NAME "${BUILD_PATH}/${testFilename}.exe;")
    add_executable(${testFilename} ${testFile})
    target_link_libraries(${testFilename} PRIVATE Qt6::Test Qt6::Gui MindMate)
    string(APPEND LAST_FILE ${testFilename})
endforeach()

find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${QT_BINARIES_DIR}")

if(WIN32)
  if(WINDEPLOYQT_EXECUTABLE)
      message(STATUS "Windeployqt found at ${WINDEPLOYQT_EXECUTABLE}")
      add_custom_command(TARGET ${LAST_FILE} POST_BUILD COMMAND
                         "${WINDEPLOYQT_EXECUTABLE}" ${SOURCE_NAME})
  else()
      message(FATAL_ERROR "Windeployqt not found.")
  endif()
endif()
