cmake_minimum_required(VERSION 3.19)
project(TPPlanner LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(TPPlanner
    main.cpp
    loginwindow.cpp
    loginwindow.h
    mainwindow.cpp
    mainwindow.h
    task.cpp
    task.h
    taskmanager.cpp
    taskmanager.h
    sessionmanager.cpp
    sessionmanager.h
    taskdialog.cpp
    taskdialog.h
)

target_link_libraries(TPPlanner PRIVATE Qt6::Widgets)