# Tablero Kanban Colaborativo (Qt + FastAPI + MySQL)

Aplicación de escritorio desarrollada en Python con interfaz en Qt (PySide6), backend en FastAPI y persistencia en MySQL.
Permite gestionar tareas tipo Kanban con operaciones CRUD completas, movimiento entre columnas, reordenamiento y sincronización colaborativa por polling.

## Tabla de contenidos

- [Características](#características)
- [Arquitectura](#arquitectura)
- [Stack tecnológico](#stack-tecnológico)
- [Estructura del proyecto](#estructura-del-proyecto)
- [Requisitos previos](#requisitos-previos)
- [Instalación y ejecución local](#instalación-y-ejecución-local)
- [Configuración del cliente Qt](#configuración-del-cliente-qt)
- [Endpoints principales](#endpoints-principales)
- [Modelo de datos](#modelo-de-datos)
- [Autenticación](#autenticación)
- [Ejecución en VPS](#ejecución-en-vps)
- [Troubleshooting](#troubleshooting)
- [Roadmap](#roadmap)
- [Autor](#autor)

## Características

- CRUD de columnas.
- CRUD de tarjetas.
- Movimiento de tarjetas entre columnas.
- Reordenamiento de tarjetas dentro de una columna.
- Actualización colaborativa del tablero (polling configurable, cada 3-5 segundos).
- Persistencia total en MySQL: al reiniciar, el tablero se conserva.
- Autenticación por Bearer Token o Basic Auth.

## Arquitectura

### Cliente (Desktop)
- App Qt (PySide6).
- Renderiza columnas y tarjetas tipo Kanban.
- Consume la API vía HTTP.
- Sincroniza estado con polling periódico.

### Backend (API)
- FastAPI con SQLAlchemy.
- Exposición de endpoints REST para columnas y tarjetas.
- Lógica de orden y movimiento de tarjetas.
- CORS habilitado para acceso remoto.

### Base de datos
- MySQL.
- Tablas:
  - `columns`
  - `cards`
  - `card_order`

## Stack tecnológico

- Python 3.10+
- FastAPI
- Uvicorn
- SQLAlchemy
- PyMySQL
- Python-dotenv
- PySide6
- Requests
- MySQL

## Estructura del proyecto

```text
Ejercicio04_Kanban/
├─ backend/
│  ├─ app/
│  │  ├─ core/
│  │  │  └─ security.py
│  │  ├─ models/
│  │  │  └─ entities.py
│  │  ├─ routers/
│  │  │  ├─ columns.py
│  │  │  └─ cards.py
│  │  ├─ schemas/
│  │  │  ├─ column.py
│  │  │  └─ card.py
│  │  ├─ database.py
│  │  └─ main.py
│  ├─ .env.example
│  ├─ requirements.txt
│  └─ schema.sql
├─ qt_client/
│  ├─ main.py
│  ├─ config.example.json
│  ├─ config.json
│  └─ requirements.txt
├─ run_backend.ps1
└─ run_client.ps1
```

## Requisitos previos

- Python 3.10 o superior.
- MySQL 8+.
- PowerShell (si usas scripts de arranque en Windows).

## Instalación y ejecución local

### 1) Clonar o copiar el proyecto

```bash
git clone <URL_DEL_REPO>
cd Ejercicio04_Kanban
```

### 2) Configurar base de datos

Crear las tablas ejecutando el script:

```sql
SOURCE C:/Ejercicio04_Kanban/backend/schema.sql;
```

### 3) Configurar variables de entorno del backend

Copiar y editar:

```bash
cp backend/.env.example backend/.env
```

Ejemplo de configuración:

```env
DB_HOST=127.0.0.1
DB_PORT=3306
DB_USER=root
DB_PASSWORD=tu_password
DB_NAME=kanban_db

API_TOKEN=1234567890
VPS_USER=admin
VPS_PASSWORD=admin

POLLING_INTERVAL_SECONDS=4
```

### 4) Levantar backend

#### Opción A: Script PowerShell (Windows)

```powershell
Set-ExecutionPolicy -Scope Process Bypass
C:/Ejercicio04_Kanban/run_backend.ps1
```

#### Opción B: Manual

```bash
cd backend
python -m venv .venv
# Windows
.venv\Scripts\activate
# Linux/macOS
source .venv/bin/activate
pip install -r requirements.txt
uvicorn app.main:app --host 0.0.0.0 --port 8000 --reload
```

### 5) Levantar cliente Qt

#### Opción A: Script PowerShell (Windows)

```powershell
Set-ExecutionPolicy -Scope Process Bypass
C:/Ejercicio04_Kanban/run_client.ps1
```

#### Opción B: Manual

```bash
cd qt_client
python -m venv .venv
# Windows
.venv\Scripts\activate
# Linux/macOS
source .venv/bin/activate
pip install -r requirements.txt
python main.py
```

## Configuración del cliente Qt

Archivo: `qt_client/config.json`

```json
{
  "api_url": "http://173.212.234.190:8000",
  "api_token": "1234567890",
  "polling_seconds": 4
}
```

Notas:
- `api_url`: URL base del backend.
- `api_token`: debe coincidir con `API_TOKEN` del backend.
- `polling_seconds`: recomendado entre 3 y 5 segundos.

## Endpoints principales

### Salud
- `GET /health`

### Columnas
- `GET /columns` - Listar columnas con sus tarjetas.
- `POST /columns` - Crear columna.
- `PUT /columns/{column_id}` - Editar columna.
- `DELETE /columns/{column_id}` - Eliminar columna.

### Tarjetas
- `GET /cards` - Listar tarjetas.
- `POST /cards` - Crear tarjeta.
- `PUT /cards/{card_id}` - Editar tarjeta.
- `DELETE /cards/{card_id}` - Eliminar tarjeta.
- `POST /cards/{card_id}/move` - Mover tarjeta entre columnas.
- `POST /cards/reorder/{column_id}` - Reordenar tarjetas en una columna.

## Modelo de datos

### columns
- `id` (PK)
- `title`
- `created_at`
- `updated_at`

### cards
- `id` (PK)
- `title`
- `description`
- `column_id` (FK -> columns.id)
- `created_at`
- `updated_at`

### card_order
- `card_id` (PK, FK -> cards.id)
- `column_id` (FK -> columns.id)
- `position`
- Restricción única: `(column_id, position)`

## Autenticación

La API acepta:

1. Bearer Token

```http
Authorization: Bearer 1234567890
```

2. Basic Auth
- Usuario y contraseña configurables en `.env`.

## Ejecución en VPS

Puntos clave para producción:

- Levantar FastAPI con:

```bash
uvicorn app.main:app --host 0.0.0.0 --port 8000
```

- Abrir puerto `8000` en firewall/security group.
- Configurar correctamente `.env` en el VPS.
- Verificar conectividad:

```bash
curl http://173.212.234.190:8000/health
```

## Troubleshooting

### Error 401 Unauthorized
- Verifica que `api_token` del cliente coincida con `API_TOKEN` en el backend.

### Error de conexión al crear columnas/tarjetas
- Confirmar que el backend está activo en la IP pública.
- Revisar firewall y puerto 8000.
- Confirmar que la URL en el cliente sea la correcta.

### Error de base de datos
- Verificar credenciales MySQL en `.env`.
- Confirmar existencia de `kanban_db` y tablas.

### La app se desconecta al fallar una petición
- Revisar logs del backend para identificar excepción SQL o validación.

## Roadmap

- Drag-and-drop real de tarjetas en Qt.
- WebSocket para actualización en tiempo real sin polling.
- Tests automatizados (unitarios e integración).
- Docker Compose para despliegue simplificado.
- Gestión de usuarios/roles.

## Autor

Proyecto desarrollado como ejercicio académico: **Ejercicio 04 - Tablero Kanban colaborativo (Qt + API)**.

Si te sirve este repositorio, puedes dejar una estrella en GitHub.
