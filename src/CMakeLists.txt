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
endif()

find_package(Qt6 REQUIRED COMPONENTS LinguistTools Widgets Svg Multimedia)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Widgets Qt6::Svg
                      Qt6::Multimedia Qt6::Network QSourceHighliter)

target_include_directories(${PROJECT_NAME} PUBLIC include base/include)

qt_add_translations(${PROJECT_NAME} TS_FILES ${TS})

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
