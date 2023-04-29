cmake_minimum_required(VERSION 3.5)

project(MindMate VERSION 0.3.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui/")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets LinguistTools Network Svg
						 Multimedia)
find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools Network Svg
						 Multimedia)

include_directories("headers" "source" "ui" "translations" "qmarkdowntextedit")
add_subdirectory("qmarkdowntextedit")

file(GLOB TS "translations/*.ts")
file(GLOB QM "translations/*.qm")
file(GLOB SOURCE "source/*.cpp")
file(GLOB HEADERS "headers/*.h")
file(GLOB UI "ui/*.ui")
aux_source_directory(ui UI)

set(PROJECT_SOURCES ${TS} ${SOURCE} ${HEADERS} ${UI} resources.qrc)

qt_add_executable(MindMate MANUAL_FINALIZATION ${PROJECT_SOURCES})

qt_add_translations(MindMate TS_FILES ${TS})

# Define target properties for Android with Qt 6 as:
#set_property(TARGET MindMate APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#							${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

target_link_libraries(MindMate PRIVATE Qt6::Widgets Qt6::Network Qt6::Svg
											Qt6::Multimedia qmarkdowntextedit)

set_target_properties(MindMate PROPERTIES
	MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
	MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
	MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
	MACOSX_BUNDLE TRUE
	WIN32_EXECUTABLE TRUE
)

install(TARGETS MindMate BUNDLE DESTINATION . LIBRARY DESTINATION
				${CMAKE_INSTALL_LIBDIR})

qt_finalize_executable(MindMate)
