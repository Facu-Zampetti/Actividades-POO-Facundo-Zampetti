# Planificador de trabajos prácticos (Individual) 

## Construir una app Qt para planificar trabajos prácticos, con login, seguimiento de entregas y persistencia local.

### Alcance mínimo:

. Login con validación y usuarios locales en archivo (CSV o JSON).

. Recordar sesión de forma local (archivo simple) para no pedir login en el mismo equipo. Persistencia de 5 minutos (simulación de sesión).

. Ventana principal con tablero de trabajos prácticos en grilla (QGridLayout), con cada fila armada con QLabel y botones de acciones, y filtro por estado/prioridad.

. Alta/edición/eliminación de trabajos prácticos.

. Editor de notas asociado al trabajo práctico con guardado manual.

. Historial de acciones visible en la UI y guardado en archivo.

### Condiciones:

. No usar QML.

. Usar QWidget (no QMainWindow).

. Organizar el código en clases (no todo en main.cpp).




# TP Planner - Planificador de Trabajos Prácticos

## Requisitos

* Qt 6.x (recomendado 6.5 o superior)
* Qt Creator
* Compilador compatible (MinGW o MSVC)
* CMake (incluido con Qt Creator)


## Compilación y ejecución (Qt Creator)

1. Abrir Qt Creator
2. Ir a **File → Open File or Project**
3. Seleccionar el archivo:

   ```
   CMakeLists.txt
   ```
4. Ejecutar con el botón de la flecha verde (Run) o `Ctrl + R`


## Archivos necesarios

Colocar estos archivos en la carpeta de ejecución (build):

* `users.json`
* `tasks.json` (puede estar vacío: `[]`)
* `history.txt` (opcional, se crea automáticamente)


## Usuarios de prueba

Ejemplo de `users.json`:

```
[
  { "username": "admin", "password": "1234" }
]
```


## Funcionalidades

* Login con validación desde archivo JSON
* Persistencia de sesión (5 minutos)
* Alta, edición y eliminación de tareas
* Notas asociadas a cada tarea
* Filtros por estado y prioridad
* Historial de acciones en archivo
* Interfaz gráfica con Qt Widgets (sin QML)


## Estructura del proyecto

```
TPPlanner/
 ├── main.cpp
 ├── loginwindow.*
 ├── mainwindow.*
 ├── task.*
 ├── taskmanager.*
 ├── sessionmanager.*
 ├── taskdialog.*
 ├── CMakeLists.txt
```

## Autor

Facundo Zampetti
