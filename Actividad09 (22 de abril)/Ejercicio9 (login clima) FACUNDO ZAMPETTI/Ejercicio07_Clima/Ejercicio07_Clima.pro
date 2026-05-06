QT += core gui widgets network

CONFIG += c++17

TEMPLATE = app
TARGET = Ejercicio07_Clima

INCLUDEPATH += src

SOURCES += \
    src/main.cpp \
    src/logger.cpp \
    src/configuracion.cpp \
    src/clima.cpp \
    src/downloaderfondo.cpp \
    src/login.cpp \
    src/ventana.cpp \
    src/appcontroller.cpp

HEADERS += \
    src/pantalla.h \
    src/logger.h \
    src/configuracion.h \
    src/clima.h \
    src/downloaderfondo.h \
    src/login.h \
    src/ventana.h \
    src/appcontroller.h

FORMS += \
    ui/login.ui
