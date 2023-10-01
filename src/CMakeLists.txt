file(GLOB SOURCE src/*.cpp)
file(GLOB SOURCE_BASE base/src/*.cpp)
file(GLOB HEADER include/*.h)
file(GLOB HEADER_BASE base/include/*.h)
file(GLOB UI ui/*.ui)
file(GLOB TS i18n/*.ts)
set(PROJECT_SOURCES ${SOURCE} ${SOURCE_BASE} ${HEADER} ${HEADER_BASE} ${UI}
                    ${TS} resources/resources.qrc)

if(WIN32)
  set(CMAKE_RC_COMPILER_INIT windres)
  enable_language(RC)
  set(PROJECT_SOURCES ${PROJECT_SOURCES} resources/icon.rc)
endif()

if(TESTING)
  add_library(${PROJECT_NAME} ${PROJECT_SOURCES})
else()
  add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

  if(WIN32)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${QT_BINARIES_DIR}")

    if(WINDEPLOYQT_EXECUTABLE)
        message(STATUS "Windeployqt found at ${WINDEPLOYQT_EXECUTABLE}")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND
                           "${WINDEPLOYQT_EXECUTABLE}"
                           "${BUILD_PATH}/${PROJECT_NAME}.exe")
    else()
        message(FATAL_ERROR "Windeployqt not found.")
    endif()
  endif()
endif()

find_package(Qt6 REQUIRED COMPONENTS LinguistTools Widgets Svg Multimedia)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Widgets Qt6::Svg
                      Qt6::Multimedia Qt6::Network QSourceHighliter)

target_include_directories(${PROJECT_NAME} PUBLIC include base/include)

qt_add_translations(${PROJECT_NAME} TS_FILES ${TS} LUPDATE_OPTIONS -noobsolete)

set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE true)

target_compile_definitions(${PROJECT_NAME} PRIVATE
                           -DAPP_VERSION="${PROJECT_VERSION}"
                           -DPROJECT_NAME="${PROJECT_NAME}")

if(FLATPAK)
  target_compile_definitions(${PROJECT_NAME} PRIVATE -DFLATPAK=${FLATPAK}
                             -DAPP_ID="${APP_ID}")
endif()

if(WIN32 OR FLATPAK)
  install(TARGETS ${PROJECT_NAME} BUNDLE DESTINATION .
          LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
else()
  install(TARGETS ${PROJECT_NAME} BUNDLE DESTINATION .
          LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
          RUNTIME DESTINATION /usr/bin)
endif()
