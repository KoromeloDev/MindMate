cmake_minimum_required(VERSION 3.16)

project(MindMate VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui/")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools Network Svg
						 Multimedia)

file(GLOB TS translations/*.ts)
file(GLOB SOURCE source/*.cpp)
file(GLOB HEADERS headers/*.h)
file(GLOB UI ui/*.ui)
aux_source_directory(ui UI)
add_subdirectory(lib)

set(PROJECT_SOURCES ${TS} ${SOURCE} ${HEADERS} ${UI} resources.qrc)

add_executable(MindMate ${PROJECT_SOURCES})
qt_add_translations(MindMate TS_FILES ${TS})

target_compile_definitions(MindMate PRIVATE -DAPP_VERSION="${PROJECT_VERSION}")
target_include_directories(MindMate PRIVATE headers source ui translations
													 lib/QSourceHighliter lib/qmarkdowntextedit)
target_link_libraries(MindMate PRIVATE Qt6::Widgets Qt6::Network Qt6::Svg
											Qt6::Multimedia qmarkdowntextedit QSourceHighliter)

set_target_properties(MindMate PROPERTIES
	MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
	MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
	MACOSX_BUNDLE_SHORT_VERSION_STRING
	${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
	MACOSX_BUNDLE TRUE
	WIN32_EXECUTABLE TRUE
)

install(TARGETS MindMate BUNDLE DESTINATION . LIBRARY DESTINATION
				${CMAKE_INSTALL_LIBDIR})
