cmake_minimum_required(VERSION 3.5)

project(SQL-ITT VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensure Qt version detection
find_package(Qt6 REQUIRED COMPONENTS Widgets Network)
set(QT_VERSION_MAJOR 6)  # Explicitly set Qt major version

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

# Fix incorrect if-statement
if(QT_VERSION_MAJOR GREATER_EQUAL 6)
    qt_add_executable(SQL-ITT
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(SQL-ITT SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(SQL-ITT
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(SQL-ITT PRIVATE Qt6::Widgets Qt6::Network)

# Set MacOS/iOS bundle options
if(QT_VERSION VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.SQL-ITT)
endif()

set_target_properties(SQL-ITT PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Install instructions
include(GNUInstallDirs)
install(TARGETS SQL-ITT
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalize executable for Qt6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SQL-ITT)
endif()
