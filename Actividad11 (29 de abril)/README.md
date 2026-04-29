# Ejercicio 09 - Coordenadas en base de datos
. Implementar una clase Pintura derivada de QWidget para dibujo a mano alzada con mouse.

. Ajustar el grosor del pincel con la rueda del mouse.

. Cambiar color del pincel con teclas R, G y B.

. Borrar el lienzo completo con Escape.

. Soportar deshacer hasta las últimas 10 acciones con Ctrl+Z.

. Construir login con Qt Designer y validar contra usuarios almacenados en SQLite.

. Registrar en logs los accesos exitosos y los intentos fallidos, sin guardar contraseñas.

. Persistir en base de datos las coordenadas del dibujo para poder reconstruir el trazo.

. Se puede usar SQLiteStudio o una herramienta equivalente para administrar la base.


## Tecnologías

- C++17
- Qt 6 (`Widgets`, `Sql`)
- CMake (>= 3.16)
- SQLite

## Estructura del proyecto

```text
C:\Ejercicio9_BD
|- CMakeLists.txt
|- README.md
|- resources
|  |- dark_tech.qss
|  |- resources.qrc
|- src
	|- main.cpp
	|- database.h / database.cpp
	|- logindialog.h / logindialog.cpp / logindialog.ui
	|- mainwindow.h / mainwindow.cpp
	|- pintura.h / pintura.cpp
```

## Requisitos previos

1. Qt 6 instalado con kit compatible con tu compilador.
2. CMake en `PATH`.
3. Visual Studio Build Tools (si compilas con MSVC).

En este entorno se uso:

- Qt: `C:/Qt/6.7.3/msvc2019_64`
- Generador: Visual Studio 2022

## Compilacion desde cero (PowerShell)

```powershell
cd C:\Ejercicio9_BD
cmake -S . -B build -DCMAKE_PREFIX_PATH=C:/Qt/6.7.3/msvc2019_64
cmake --build build --config Debug
```

## Ejecución

```powershell
cd C:\Ejercicio9_BD
.\build\Debug\Ejercicio09_CoordenadasBD.exe
```

## Usuario inicial

Si la tabla de usuarios esta vacia, se crea automaticamente:

- Usuario: `admin`
- Contrasena: `admin123`

## Controles de dibujo

- Mouse izquierdo: dibujar trazo
- Rueda del mouse: aumentar/disminuir grosor del pincel
- `R`: color rojo
- `G`: color verde
- `B`: color azul
- `Escape`: borrar todo el lienzo
- `Ctrl+Z`: deshacer (hasta 10 trazos)

## Base de datos SQLite

El archivo `datos.sqlite` se crea junto al ejecutable en la carpeta desde donde corre la app.

Ejemplo usando compilacion MSVC:

```text
C:\Ejercicio9_BD\build\Debug\datos.sqlite
```

Si ejecutas desde otro kit/perfil (por ejemplo Qt Creator con MinGW), se crea en ese output folder.

### Tablas principales

- `users`: usuarios y hash de contraseña
- `access_logs`: auditoría de accesos (exitos/fallos)
- `strokes`: metadatos del trazo (usuario, color, grosor, orden)
- `stroke_points`: puntos `(x, y)` por trazo en orden de dibujo

## Verificación rápida funcional

1. Login correcto con `admin/admin123`.
2. Login incorrecto para generar intento fallido.
3. Dibujo + cambio de color + cambio de grosor.
4. `Ctrl+Z` sobre varios trazos.
5. `Escape` para limpiar.
6. Cerrar y abrir para verificar reconstrucción de trazos persistidos.
