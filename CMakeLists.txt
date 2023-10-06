cmake_minimum_required(VERSION 3.26 FATAL_ERROR)
project(MindMate VERSION 1.8.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_AUTOUIC true)
set(CMAKE_AUTOMOC true)
set(CMAKE_AUTORCC true)
set(CMAKE_AUTOUIC_SEARCH_PATHS ui/)

set(APP_ID io.github.koromelodev.mindmate)

if(WIN32)
  set(BUILD_PATH ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-Windows)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_PATH})
endif()

add_subdirectory(import)
add_subdirectory(src)

if(TESTING)
  add_subdirectory(tests)
else()
  add_subdirectory(dist)
endif()
