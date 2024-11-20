cmake_minimum_required(VERSION 3.29)
project(usageTracker)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Quick
        REQUIRED)
qt_standard_project_setup()

if (QT_KNOWN_POLICY_QTP0004)
    qt_policy(SET QTP0004 NEW)
endif ()

file(GLOB UNSORTED_FILES ${PROJECT_SOURCE_DIR}/qml/*.qml)

foreach (FILE IN LISTS UNSORTED_FILES)
    file(RELATIVE_PATH REL "${PROJECT_SOURCE_DIR}" "${FILE}")
    string(REPLACE "qml/" "./" TRANSFORMED_PATH "${REL}")
    message("-- Registering QML file: ${FILE}")
    set_source_files_properties(${FILE} PROPERTIES QT_RESOURCE_ALIAS ${TRANSFORMED_PATH})
endforeach ()

qt_add_executable(usageTracker src/main.cpp
        src/CpuTimer.cpp
        src/CpuTimer.h)

qt_add_qml_module(usageTracker
        URI tracker
        OUTPUT_DIRECTORY qmlGen
        RESOURCE_PREFIX /qt/qml
        QML_FILES ${UNSORTED_FILES}
        )

target_link_libraries(usageTracker PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt::Quick
)
target_include_directories(usageTracker PUBLIC src)

