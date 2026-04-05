# Ejercicio 02 - Panel de monitoreo VPS
En un Empty qmake Project, crear una aplicación Qt de escritorio profesional que funcione como panel de monitoreo de un server en la nube (su propio VPS).

Debe existir un endpoint en el server que devuelva datos de salud (uptime, carga, memoria, disco o estado general), y la aplicación debe presentar un panel claro y legible con esos datos.

El panel debe mostrar: estado general (OK, alerta, caído), métricas principales, último chequeo, y un historial corto de eventos.

Debe haber widgets interactivos (por ejemplo: QSpinBox, QPushButton, QLineEdit, QLabel). La elección y el rol de cada widget debe estar justificada por el problema.

Inspirarse en un sistema de monitoreo profesional (por ejemplo: Grafana, Zabbix, Prometheus, Netdata o Datadog) para el diseño del panel.

Incluir al menos un control para refresco manual y otro para configurar un umbral o intervalo de chequeo.

Crear al menos una clase propia que contenga la lógica del monitoreo (no todo en main).


## Estructura

- `PanelMonitoreoVPS.pro`: proyecto qmake.
- `src/`: codigo fuente de la app Qt.
- `server/health_endpoint.py`: endpoint HTTP `/health` para desplegar en el VPS.

## Requisitos del ejercicio cubiertos

- Panel profesional de escritorio en Qt Widgets.
- Endpoint de salud del servidor con uptime, carga, memoria y disco.
- Estado general: `OK`, `ALERT`, `DOWN`.
- Metricas principales visibles y legibles.
- Ultimo chequeo.
- Historial corto de eventos (ultimos 25).
- Widgets interactivos: `QLineEdit`, `QSpinBox`, `QPushButton`, `QLabel`, `QProgressBar`, `QListWidget`.
- Control de refresco manual (`Refrescar ahora`).
- Configuracion de intervalo y umbrales (`QSpinBox`).
- Clase propia para logica: `MonitorService`.

## Ejecutar endpoint en VPS (Linux)

1. Copiar carpeta `server/` al VPS.
2. Instalar Python 3 y pip si no estan instalados.
3. En el VPS:

```bash
cd server
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
python health_endpoint.py
```

4. Abrir puerto `8080` en firewall/security group si hace falta.
5. Probar:

```bash
curl http://173.212.234.190:8080/health
```

## Abrir en Qt Creator

1. Abrir `PanelMonitoreoVPS.pro`.
2. Seleccionar kit de Qt Widgets.
3. Ejecutar.

La app inicia con endpoint por defecto: `http://173.212.234.190:8080/health`.
Puedes cambiarlo con `QLineEdit`, ajustar intervalo y umbrales con `QSpinBox`, aplicar, y refrescar manualmente.
