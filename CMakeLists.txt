cmake_minimum_required(VERSION 3.16)

project(MindMate VERSION 1.1.3 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui/")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(LINUX)
	set(DESKTOP_FILE_INSTALL_DIR "$ENV{HOME}/.local/share/applications")
	set(DESKTOP_FILE_TEMPLATE "${CMAKE_CURRENT_SOURCE_DIR}/MindMate.desktop")
	set(APP_ICON "${CMAKE_CURRENT_SOURCE_DIR}/icons/MindMateLogo.svg")
	set(DESKTOP_FILE_ICON "${CMAKE_INSTALL_PREFIX}/share/icons/MindMateLogo.svg")

	set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/MindMate.desktop
															PROPERTIES "ICON" "${DESKTOP_FILE_ICON}")

	configure_file(${DESKTOP_FILE_TEMPLATE}
								 ${CMAKE_CURRENT_BINARY_DIR}/MindMate.desktop)
else(WIN32)
	set(CMAKE_RC_COMPILER_INIT windres)
	enable_language(RC)
endif()


find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools Network Svg
						 Multimedia)

file(GLOB TS translations/*.ts)
file(GLOB SOURCE source/*.cpp)
file(GLOB HEADERS headers/*.h)
file(GLOB UI ui/*.ui)
aux_source_directory(ui UI)
add_subdirectory(lib)

set(PROJECT_SOURCES ${TS} ${SOURCE} ${HEADERS} ${UI} resources.qrc icon.rc)

add_executable(MindMate ${PROJECT_SOURCES})
qt_add_translations(MindMate TS_FILES ${TS})

target_compile_definitions(MindMate PRIVATE -DAPP_VERSION="${PROJECT_VERSION}")
target_include_directories(MindMate PRIVATE headers source ui translations
													 lib/QSourceHighliter)
target_link_libraries(MindMate PRIVATE Qt6::Widgets Qt6::Network Qt6::Svg
											Qt6::Multimedia QSourceHighliter)

set_target_properties(MindMate PROPERTIES
											MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
											MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
											MACOSX_BUNDLE_SHORT_VERSION_STRING
											${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
											MACOSX_BUNDLE TRUE
											WIN32_EXECUTABLE TRUE)

if(LINUX)
	install(TARGETS MindMate
					BUNDLE DESTINATION .
					LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
					RUNTIME DESTINATION /usr/bin)
	install(FILES ${APP_ICON}
					DESTINATION "${CMAKE_INSTALL_PREFIX}/share/icons/")
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/MindMate.desktop
					DESTINATION ${DESKTOP_FILE_INSTALL_DIR})
else()
	install(TARGETS MindMate
					BUNDLE DESTINATION .
					LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()
