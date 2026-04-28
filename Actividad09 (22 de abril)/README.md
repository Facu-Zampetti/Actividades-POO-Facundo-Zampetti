# Ejercicio 07 - Login con clima y fondo de pantalla
. Login hecho con QtDesigner con usuario admin:1234 y bloqueo temporal tras 3 intentos fallidos.

. Consumo de una API de terceros para clima actual en Córdoba con autenticación simple (API key en header o query).

. Consulta directa a la API con parámetros de ciudad y unidades, sin usar SDKs oficiales.

. Muestra de temperatura y hora local en el Login.

. Descarga de imagen de fondo desde una URL pública con caché local.

. Soporte para proxy.

. Uso obligatorio de QNetworkAccessManager, con los connect tal como lo vimos en clase.

. Luego de un usuario válido, se abre la ventana principal.

. Ventana principal en full screen que se abre solo cuando la imagen principal ya terminó de descargarse.

. Imagen principal ajustada a la ventana manteniendo relación de aspecto y sin deformación.

. Clase base abstracta Pantalla con interfaz común y dos clases derivadas: Login y Ventana.

. Clase derivada adicional para manejo de datos (por ejemplo Clima).

. Registro de eventos en un archivo de log con fecha y descripción de cada acción.

. Lectura de configuración desde archivo para API key y ciudad.

. Validación de datos recibidos y mensajes de error amigables en la UI.

. Soporte de modo offline con datos simulados si no hay internet. Requiere detección de conexión y manejo de errores de red.

. Uso de signals/slots tal como lo vimos en clase.

. Para que no quede vacía la ventana principal, agregá tu currículum al estilo LinkedIn, con foto y descripción breve. Que quede elegante y profesional.

## Arquitectura del proyecto

```
Ejercicio07_Clima/
├── CMakeLists.txt          # Configuración de build
├── config.ini              # Configuración de usuario (API key, proxy, URL fondo)
├── src/
│   ├── main.cpp            # Punto de entrada
│   ├── pantalla.h          # Clase abstracta base (interfaz común)
│   ├── login.h / .cpp      # Pantalla de login (derivada de Pantalla)
│   ├── ventana.h / .cpp    # Ventana principal full screen (derivada de Pantalla)
│   ├── clima.h / .cpp      # Manejo de datos del clima (derivada de Pantalla)
│   ├── downloaderfondo.h/cpp  # Descarga y caché de imagen de fondo
│   ├── configuracion.h/cpp    # Lectura de config.ini
│   ├── logger.h / .cpp     # Registro de eventos en archivo de log
│   └── appcontroller.h/cpp # Controlador principal, conecta todas las partes
└── ui/
    └── login.ui            # Diseño de pantalla de login (Qt Designer)
```

### Jerarquía de clases

```
QObject
└── Pantalla  (abstracta — interfaz común: widget(), mostrar(), ocultar())
    ├── Login     — pantalla de autenticación
    ├── Ventana   — ventana principal full screen
    └── Clima     — consulta y manejo de datos meteorológicos
```

---

## Configuración (`config.ini`)

```ini
[clima]
api_key=TU_API_KEY        ; Clave de openweathermap.org (gratuita)
ciudad=Cordoba,AR         ; Ciudad a consultar
unidades=metric           ; metric = °C / imperial = °F

[fondo]
url=https://picsum.photos/1920/1080   ; URL pública de imagen de fondo

[proxy]
enabled=false             ; true para activar proxy
host=                     ; IP o hostname del proxy
port=8080
username=
password=
```

> **Nota:** La API key se obtiene gratis registrándose en [openweathermap.org](https://openweathermap.org/api). Puede tardar hasta 2 horas en activarse tras el registro.

---

## Compilación
### Requisitos

- Qt 6 (módulos: Core, Gui, Widgets, Network)
- CMake 3.16 o superior
- MinGW 13 (incluido con Qt) o MSVC

### Pasos
```powershell
# 1. Ir a la carpeta del proyecto
cd C:\Ejercicio07_Clima

# 2. Configurar con CMake (MinGW)
cmake -S . -B build `
  -DCMAKE_PREFIX_PATH="C:/Qt/6.10.2/mingw_64" `
  -DCMAKE_CXX_COMPILER="C:/Qt/Tools/mingw1310_64/bin/g++.exe" `
  -G "MinGW Makefiles"

# 3. Compilar
cmake --build build

# 4. Copiar DLLs de Qt junto al ejecutable
C:\Qt\6.10.2\mingw_64\bin\windeployqt.exe .\build\Ejercicio07_Clima.exe

# 5. Ejecutar
.\build\Ejercicio07_Clima.exe
```

---

## Uso
### Login
- Usuario: `admin`  
- Contraseña: `1234`  
- Presionar botón **Ingresar** o tecla **Enter**
- Tras 3 intentos fallidos, el login se bloquea 30 segundos automáticamente

### Foto de perfil personalizada
Copiar una imagen propia como `foto.jpg` (o `foto.png`) en la misma carpeta del `.exe`:
```
build\foto.jpg
```

La app la detecta, la recorta en círculo y la muestra en el perfil. No requiere recompilar.

### Imagen de fondo

La imagen se descarga al iniciar y se guarda en caché local. Para forzar una nueva descarga, eliminar el archivo:

```
%APPDATA%\UTN\Ejercicio07_Clima\cache\fondo_principal.jpg
```

---

## Log de eventos
Cada acción relevante queda registrada con fecha y hora en:

```
%APPDATA%\UTN\Ejercicio07_Clima\eventos.log
```

Ejemplo de entradas:
```
2026-04-27 14:32:01 | App: inicio de aplicacion
2026-04-27 14:32:01 | Login: pantalla inicializada
2026-04-27 14:32:02 | Clima: inicio de deteccion de conectividad
2026-04-27 14:32:02 | Fondo: descarga iniciada desde URL publica
2026-04-27 14:32:04 | Clima: datos online recibidos para Cordoba
2026-04-27 14:32:05 | Fondo: descarga finalizada y cacheada
2026-04-27 14:32:10 | Login: acceso exitoso
2026-04-27 14:32:10 | App: ventana principal abierta en full screen
```

---

## Modo offline
Si no hay conexión a internet o la API key no está activa todavía, la app:
1. Detecta la falta de conectividad automáticamente
2. Genera una temperatura simulada aleatoria
3. Muestra un mensaje indicando que está en modo offline
4. Funciona normalmente en todo lo demás

Cuando la conexión se restablece, la actualización automática cada 5 minutos retoma los datos reales.
