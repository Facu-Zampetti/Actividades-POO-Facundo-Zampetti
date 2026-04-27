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
