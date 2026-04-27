cmake_minimum_required(VERSION 3.16)
project(Ejercicio07_Clima VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/ui)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)

set(SOURCES
    src/main.cpp
    src/pantalla.h
    src/logger.h
    src/logger.cpp
    src/configuracion.h
    src/configuracion.cpp
    src/clima.h
    src/clima.cpp
    src/downloaderfondo.h
    src/downloaderfondo.cpp
    src/login.h
    src/login.cpp
    src/ventana.h
    src/ventana.cpp
    src/appcontroller.h
    src/appcontroller.cpp
)

set(FORMS
    ui/login.ui
)

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${FORMS}
)

target_include_directories(${PROJECT_NAME} PRIVATE src)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
)
