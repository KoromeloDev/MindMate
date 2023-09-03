cmake_minimum_required(VERSION 3.26 FATAL_ERROR)
project(MindMate VERSION 1.5.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_AUTOUIC true)
set(CMAKE_AUTOMOC true)
set(CMAKE_AUTORCC true)
set(CMAKE_AUTOUIC_SEARCH_PATHS ui/)

file(GLOB TS i18n/*.ts)
file(GLOB SOURCE src/*.cpp)
file(GLOB HEADERS include/*.h)
file(GLOB UI ui/*.ui)
set(PROJECT_SOURCES ${TS} ${SOURCE} ${HEADERS} ${UI} resources.qrc)

set(APP_ID io.github.koromelodev.mindmate)

if(WIN32)
  set(BUILD_PATH ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-Windows)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_PATH})
  set(CMAKE_RC_COMPILER_INIT windres)
  enable_language(RC)
  set(PROJECT_SOURCES ${PROJECT_SOURCES} resources/icon.rc)
endif()

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_compile_definitions(${PROJECT_NAME} PRIVATE
                           -DAPP_VERSION="${PROJECT_VERSION}"
                           -DPROJECT_NAME="${PROJECT_NAME}")
if(FLATPAK)
  target_compile_definitions(${PROJECT_NAME} PRIVATE -DFLATPAK=${FLATPAK}
                             -DAPP_ID="${APP_ID}")
endif()

add_subdirectory(import)
add_subdirectory(dist)
target_include_directories(${PROJECT_NAME} PUBLIC include src ui i18n)

find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools Network Svg
             Multimedia)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Widgets Qt6::Network
                      Qt6::Svg Qt6::Multimedia QSourceHighliter)

set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE true)

qt_add_translations(${PROJECT_NAME} TS_FILES ${TS})

if(WIN32 OR FLATPAK)
  install(TARGETS ${PROJECT_NAME}
          BUNDLE DESTINATION .
          LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
else()
  install(TARGETS ${PROJECT_NAME}
          BUNDLE DESTINATION .
          LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
          RUNTIME DESTINATION /usr/bin)
endif()
