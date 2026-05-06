QT += core gui widgets

CONFIG += c++17

TEMPLATE = app
TARGET = TPPlanner

SOURCES += \
    main.cpp \
    loginwindow.cpp \
    mainwindow.cpp \
    task.cpp \
    taskmanager.cpp \
    sessionmanager.cpp \
    taskdialog.cpp

HEADERS += \
    loginwindow.h \
    mainwindow.h \
    task.h \
    taskmanager.h \
    sessionmanager.h \
    taskdialog.h
