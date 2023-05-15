cmake_minimum_required(VERSION 3.16)

project(MindMate VERSION 1.1.5 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ui/)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(LINUX)
  set(DESKTOP_INSTALL_DIR $ENV{HOME}/.local/share/applications)
  set(DESKTOP_FILE_TEMPLATE ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.desktop)
  set(APP_ICON icons/MindMateLogo.svg)
  set(PATH_ICON /usr/share/icons/hicolor/scalable/apps)
else(WIN32)
  set(CMAKE_RC_COMPILER_INIT windres)
  enable_language(RC)
endif()


find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools Network Svg
             Multimedia)

file(GLOB TS translations/*.ts)
file(GLOB SOURCE sources/*.cpp)
file(GLOB HEADERS headers/*.h)
file(GLOB UI ui/*.ui)
aux_source_directory(ui UI)
add_subdirectory(lib)

if(LINUX)
  set(PROJECT_SOURCES ${TS} ${SOURCE} ${HEADERS} ${UI} resources.qrc)
else(WIN32)
  set(PROJECT_SOURCES ${TS} ${SOURCE} ${HEADERS} ${UI} resources.qrc icon.rc)
endif()

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
qt_add_translations(${PROJECT_NAME} TS_FILES ${TS})

target_compile_definitions(${PROJECT_NAME} PRIVATE
                           -DAPP_VERSION="${PROJECT_VERSION}"
                           -DCHECKUPDATES=true)
target_include_directories(${PROJECT_NAME} PRIVATE headers source ui
                           translations lib/QSourceHighliter)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Network Qt6::Svg
                      Qt6::Multimedia QSourceHighliter)

set_target_properties(${PROJECT_NAME} PROPERTIES
                      MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
                      MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
                      MACOSX_BUNDLE_SHORT_VERSION_STRING
                      ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
                      MACOSX_BUNDLE TRUE
                      WIN32_EXECUTABLE TRUE)

if(LINUX)
  install(TARGETS ${PROJECT_NAME}
          BUNDLE DESTINATION .
          LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
          RUNTIME DESTINATION /usr/bin)
  install(FILES ${APP_ICON}
          DESTINATION ${PATH_ICON}
          RENAME ${PROJECT_NAME})
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.desktop
          DESTINATION ${DESKTOP_INSTALL_DIR})
else()
  install(TARGETS ${PROJECT_NAME}
          BUNDLE DESTINATION .
          LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()
