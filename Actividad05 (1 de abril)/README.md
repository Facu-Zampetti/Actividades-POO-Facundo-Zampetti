# Ejercicio 03 - Lienzo colaborativo en tiempo real
Desarrollar una aplicación de escritorio en Qt que permita dibujar a mano alzada sobre un lienzo, con comportamiento suave y continuo, incorporando sincronización colaborativa mediante el servidor VPS del equipo. La aplicación debe cumplir con los siguientes requisitos:

### Requisitos generales:
. La aplicación debe implementar un lienzo de dibujo utilizando paintEvent.

. El sistema debe permitir dibujo libre a mano alzada, con suavizado visual y control dinámico del trazo.

. El dibujo será colaborativo entre los integrantes del equipo, sincronizado a través de un backend en VPS.

### Interacción de dibujo:
. Click izquierdo (presionado): Dibuja sobre el lienzo.

. Click derecho (presionado): Actúa como goma (borra el dibujo)

### El trazo debe ser:
. Continuo (sin segmentos visibles).

. Suavizado (no deben notarse “líneas rectas” entre puntos).

. Independiente de la velocidad del mouse.

### Se debe implementar un mecanismo propio de:
. Interpolación de puntos.

. Suavizado del trazo.

. Color del trazo: se selecciona con teclas numéricas 1 al 9.

### Debe generarse una interpolación de colores entre:
. RGB inicial: (192, 19, 76).

. RGB final: (24, 233, 199).

. El cambio debe ser progresivo.

### Grosor del trazo:
. Se controla con la rueda del mouse (scroll).

. Debe afectar tanto al lápiz como a la goma.

### Interfaz gráfica:
. Debe existir una barra superior (tipo menú o toolbar) sobre el lienzo.

. Incluir un botón Guardar con estilo visual tipo Metro de Windows.

### Implementar un endpoint en el VPS que permita:
. Guardar el estado del dibujo. 

. Recuperar el dibujo actual.

### Funcionalidad:
. Al presionar Guardar, se envía el dibujo al servidor.

### Al iniciar la aplicación:
. Se debe recuperar el dibujo almacenado.

. Permitir continuar editando.

. Colaboración en tiempo real.

. El dibujo es compartido entre los integrantes del equipo.

### Reglas:
. Si un usuario guarda → actualiza el estado en el servidor.

### Los demás usuarios deben poder:
. Ver los cambios realizados por otros.

. Sin perder lo que están dibujando localmente.

### Condición clave:
. No se debe perder información al guardar.

### Se debe implementar una estrategia de:
. Fusión de trazos (merge). 

. Modelo incremental de dibujo.

. Arquitectura (obligatoria).

### Se debe implementar al menos:
. Una clase para el modelo de dibujo (almacenamiento de trazos).

. Una clase para la vista (renderizado con paintEvent).

. Una clase para la lógica de sincronización (API / VPS).

## Características

- Lienzo renderizado con paintEvent.
- Dibujo libre con click izquierdo.
- Goma con click derecho.
- Trazo suavizado e interpolado para reducir segmentos rectos visibles.
- Grosor dinámico con rueda del mouse (lápiz y goma).
- Color del trazo con teclas numéricas 1..9.
- Gradiente de color progresivo entre:
  - RGB inicial: (192, 19, 76)
  - RGB final: (24, 233, 199)
- Barra superior con botón Guardar, estilo visual tipo Metro.
- Carga del canvas desde servidor al iniciar.
- Guardado manual del estado del canvas al servidor.
- Sincronización colaborativa en tiempo real con modelo incremental.
- Merge de trazos por id para no perder información entre usuarios.
- Regla de autoría en borrado: cada usuario sólo puede borrar su propio contenido.

## Estructura principal

- main.cpp: punto de entrada de la aplicación.
- mainwindow.h / mainwindow.cpp: ventana principal, toolbar, wiring de servicios.
- canvaswidget.h / canvaswidget.cpp: vista de dibujo e interacción de mouse/teclado.
- drawingmodel.h / drawingmodel.cpp: modelo de trazos y serialización JSON.
- syncservice.h / syncservice.cpp: carga/guardado HTTP del estado completo.
- realtimesyncservice.h / realtimesyncservice.cpp: sincronización incremental en tiempo real.
- Ejercicio5.pro: archivo de proyecto qmake.

## Requisitos

- Windows
- Qt 6.x con módulo Widgets y Network
- MinGW (si compilas con kit MinGW)
- qmake y mingw32-make disponibles en PATH

## Compilación (terminal)

Desde la carpeta del proyecto:

~~~powershell
qmake Ejercicio5.pro
mingw32-make -C .
~~~

Si ya existe Makefile generado, normalmente alcanza con:

~~~powershell
mingw32-make -C .
~~~

## Ejecución

Ejecutable de release:

~~~powershell
.\release\lienzo_colaborativo.exe
~~~

Si prefieres modo debug (dependiendo de tu flujo):

~~~powershell
.\debug\lienzo_colaborativo.exe
~~~

## Compilar y correr desde Qt Creator

1. Abrir Ejercicio5.pro.
2. Seleccionar un kit de Qt 6 (por ejemplo MinGW 64-bit).
3. Build.
4. Run.

## Configuración de servidor

URL base configurada actualmente en el codigo:

- http://173.212.234.190:8082

Endpoint usado:

- GET /canvas: recuperar estado actual del lienzo.
- POST /canvas: guardar estado fusionado del lienzo.

## Flujo de colaboración en tiempo real

1. Al iniciar, el cliente hace carga inicial del canvas por HTTP.
2. Luego se activa sincronización periódica en tiempo real.
3. Los trazos locales se envían de forma incremental.
4. Antes de publicar, el cliente fusiona estado remoto + local para evitar sobrescritura ciega.
5. Los clientes remotos reciben nuevos trazos y deltas de puntos en vivo.

## Controles

- Mouse izquierdo presionado: dibujar.
- Mouse derecho presionado: goma.
- Rueda del mouse: cambiar grosor.
- Teclas 1 a 9: cambiar color según gradiente.
- Boton Guardar: persistir estado actual en VPS.

## Notas técnicas

- La autoría local se genera por cliente para distinguir trazos propios y remotos.
- El borrado está aislado por autor a nivel de render para no afectar dibujos de otros integrantes.
- El modelo usa ids de trazo para poder fusionar sin perder información.
