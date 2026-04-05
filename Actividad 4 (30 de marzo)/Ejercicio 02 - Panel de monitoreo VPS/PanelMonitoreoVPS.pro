QT += core gui widgets network

CONFIG += c++17

TEMPLATE = app
TARGET = PanelMonitoreoVPS

SOURCES += \
    src/main.cpp \
    src/MainWindow.cpp \
    src/MonitorService.cpp

HEADERS += \
    src/MainWindow.h \
    src/MonitorService.h
