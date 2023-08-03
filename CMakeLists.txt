cmake_minimum_required(VERSION 3.27)
project(MindMate VERSION 1.1.13 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC True)
set(CMAKE_AUTOMOC True)
set(CMAKE_AUTORCC True)
set(CMAKE_AUTOUIC_SEARCH_PATHS ui/)

file(GLOB TS translations/*.ts)
file(GLOB SOURCE sources/*.cpp)
file(GLOB HEADERS headers/*.h)
file(GLOB UI ui/*.ui)
set(PROJECT_SOURCES ${TS} ${SOURCE} ${HEADERS} ${UI} resources.qrc)

if(LINUX)
  set(APP_ID io.github.koromelodev.mindmate)
  set(APP_ICON icons/MindMateLogo.svg)
  set(APP_ICON_PNG icons/${APP_ID}.png)
  set(METAINFO ${APP_ID}.metainfo.xml)
  set(DESKTOP_FILE ${PROJECT_NAME}.desktop)

  if (FLATPAK)
    set(PATH_ICON /app/share/icons/hicolor/scalable/apps)
    set(PATH_ICON_PNG /app/share/icons/hicolor/128x128/apps)
    set(DESKTOP_INSTALL_DIR /app/share/applications)
    set(METAINFO_PATH /app/share/metainfo)
    configure_file(${DESKTOP_FILE} ${APP_ID}.desktop COPYONLY)
    set(DESKTOP_FILE ${APP_ID}.desktop)
    file(APPEND ${DESKTOP_FILE} Terminal=true)
  else()
    set(PATH_ICON /usr/share/icons/hicolor/scalable/apps)
    set(PATH_ICON_PNG /usr/share/icons/hicolor/128x128/apps)
    set(DESKTOP_INSTALL_DIR $ENV{HOME}/.local/share/applications)
    set(METAINFO_PATH /usr/share/metainfo)

  endif()
else(WIN32)
  set(CMAKE_RC_COMPILER_INIT windres)
  enable_language(RC)
  set(PROJECT_SOURCES ${PROJECT_SOURCES} icon.rc)
endif()

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_compile_definitions(${PROJECT_NAME} PRIVATE
                           -DAPP_VERSION="${PROJECT_VERSION}"
                           -DPROJECT_NAME="${PROJECT_NAME}")
if(FLATPAK)
  target_compile_definitions(${PROJECT_NAME} PRIVATE -DCHECKUPDATES=False
                             -DFLATPAK=${FLATPAK})
else()
  target_compile_definitions(${PROJECT_NAME} PRIVATE -DCHECKUPDATES=True)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC headers source ui
                           translations)
find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools Network Svg
             Multimedia)
add_subdirectory(lib)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Widgets Qt6::Network
                      Qt6::Svg Qt6::Multimedia QSourceHighliter)

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

if(LINUX)
  install(FILES ${APP_ICON}
          DESTINATION ${PATH_ICON}
          RENAME ${APP_ID}.svg)
  install(FILES ${APP_ICON_PNG}
          DESTINATION ${PATH_ICON_PNG})
  install(FILES ${DESKTOP_FILE}
          DESTINATION ${DESKTOP_INSTALL_DIR})
  install(FILES ${METAINFO}
          DESTINATION ${METAINFO_PATH})
endif()
