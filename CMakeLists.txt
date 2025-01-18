cmake_minimum_required(VERSION 3.16)

project(LeePickerX VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ui)

set(PYTHON_INCLUDE "C:/Users/thang/AppData/Local/Programs/Python/Python37/")
set(PYTHON_LIB "C:/Users/thang/AppData/Local/Programs/Python/Python37/libs/python37.lib")


include_directories(${PYTHON_INCLUDE}/include)
link_libraries(${PYTHON_LIB})

set(PROJECT_SOURCES
        include/Definations.h
        include/mainwindow.h
        include/leepickeritem.h
        include/leepickerscene.h
        include/leepickerview.h
        include/singleton.h
        include/LeePickerPython.h
        include/leeSendCommands.h
        src/main.cpp
        src/mainwindow.cpp
        src/leepickeritem.cpp
        src/leepickerscene.cpp
        src/leepickerview.cpp
        src/leeGlobal.hpp
        src/leeSendCommands.cpp
        ui/leePicker.ui
        ui/leePicker.qrc
        ui/ScriptEditor.ui
        ui/leePicker.qrc
        LeePickerX.rc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(LeePickerX
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET LeePickerX APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(LeePickerX SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(LeePickerX
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(LeePickerX PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::Network ${PYTHON_LIB})
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.LeePickerX)
endif()
set_target_properties(LeePickerX PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS LeePickerX
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(LeePickerX)
endif()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Scripts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/icons DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Saved DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Addons DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/startup DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
