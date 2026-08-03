cmake_minimum_required(VERSION 3.25)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(hexis-gui
    main.cpp
    mainwindow.cpp
    mainwindow.h
)

target_link_libraries(hexis-gui PRIVATE Qt6::Widgets hexis)
