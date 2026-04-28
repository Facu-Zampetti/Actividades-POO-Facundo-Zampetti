# Ejercicio 04 - Tablero Kanban colaborativo (Qt + API)

### Desarrollar una aplicación de escritorio en Qt que permita gestionar tareas tipo Kanban, usando el backend FastAPI del VPS y persistencia en MySQL.
Consolidar CRUD, orden y movimiento entre columnas. Incorporar actualización colaborativa.

### Requisitos:
Backend (FastAPI):
. CRUD de columnas y tarjetas.

. Endpoint para mover tarjeta entre columnas.

. Endpoint para reordenar tarjetas en una columna.

. Autenticación básica (reutilizar la del VPS).

### Base de datos (MySQL):
. Tablas: columns, cards, card_order.

### App Qt:
. Vista Kanban con columnas y tarjetas.

. Crear/editar/eliminar tarjetas y columnas.

. Mover tarjetas (drag-and-drop o botones "mover").

. Actualización en tiempo real (polling cada 3-5s o WebSocket).

### Persistencia:
. Al reiniciar, el tablero queda igual.

--- 

### Stack tecnológico:

- Python 3.10+
- FastAPI
- Uvicorn
- SQLAlchemy
- PyMySQL
- MySQL
- PySide6 (Qt)
- Requests

### Arquitectura:
### Backend:
. API REST en FastAPI con autenticación por Bearer token (y soporte Basic Auth).

### Base de datos:
. MySQL con tablas columns, cards y card_order.

### Cliente:
. App de escritorio en Qt que consume la API y refresca automáticamente el estado del tablero.

### Funcionalidades:
### Columnas:
- Crear
- Editar
- Eliminar
- Listar con sus tarjetas

### Tarjetas:
- Crear
- Editar
- Eliminar
- Mover entre columnas
- Reordenar dentro de una columna

### Colaboración:
- Actualización automática por polling

### Modelo de datos:
### columns:
- id (PK)
- title
- created_at
- updated_at

### cards:
- id (PK)
- title
- description
- column_id (FK -> columns.id)
- created_at
- updated_at

### card_order:
- card_id (PK, FK -> cards.id)
- column_id (FK -> columns.id)
- position
- unique(column_id, position)

### Endpoints principales:
### Salud:
- GET /health

### Columnas:
- GET /columns
- POST /columns
- PUT /columns/{column_id}
- DELETE /columns/{column_id}

### Tarjetas:
- GET /cards
- POST /cards
- PUT /cards/{card_id}
- DELETE /cards/{card_id}
- POST /cards/{card_id}/move
- POST /cards/reorder/{column_id}

### Autenticación
### Bearer token:
. Authorization: Bearer 1234567890

### Basic auth:
. Usuario y contraseña definidos en variables de entorno.

### Configuración local (Windows):
### 1) Backend:
Set-ExecutionPolicy -Scope Process Bypass
cd C:\Ejercicio04_kanban\backend
py -m venv .venv
.\.venv\Scripts\Activate.ps1
python -m pip install --upgrade pip
pip install -r requirements.txt
uvicorn app.main:app --host 0.0.0.0 --port 8000

### 2) Cliente Qt (otra terminal):
Set-ExecutionPolicy -Scope Process Bypass
cd C:\Ejercicio04_kanban\qt_client
py -m venv .venv
.\.venv\Scripts\Activate.ps1
python -m pip install --upgrade pip
pip install -r requirements.txt
python main.py

### 3) Configuración del cliente:
api_url: http://173.212.234.190:8000
api_token: 1234567890
polling_seconds: 4

### Despliegue en VPS (Ubuntu):
### 1) Instalar dependencias base:
sudo apt update
sudo apt install -y python3 python3-venv python3-pip mysql-server

### 2) Preparar backend:
cd /home/facundo/Ejercicio04_kanban/backend
python3 -m venv .venv
source .venv/bin/activate
pip install --upgrade pip
pip install -r requirements.txt

### 3) Configurar variables de entorno:
DB_HOST=127.0.0.1
DB_PORT=3306
DB_USER=kanban_user
DB_PASSWORD=1234
DB_NAME=kanban_db
API_TOKEN=1234567890
VPS_USER=admin
VPS_PASSWORD=admin

### 4) Crear base de datos y usuario:
sudo mysql -e "CREATE DATABASE IF NOT EXISTS kanban_db CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;"
sudo mysql -e "CREATE USER IF NOT EXISTS 'kanban_user'@'localhost' IDENTIFIED BY '1234';"
sudo mysql -e "GRANT ALL PRIVILEGES ON kanban_db.* TO 'kanban_user'@'localhost'; FLUSH PRIVILEGES;"
sudo mysql kanban_db < /home/facundo/Ejercicio04_kanban/backend/schema.sql

### 5) Probar arranque manual:
cd /home/facundo/Ejercicio04_kanban/backend
source .venv/bin/activate
uvicorn app.main:app --host 0.0.0.0 --port 8000

### 6) Verificar API:
curl -i http://127.0.0.1:8000/health
curl -i -H "Authorization: Bearer 1234567890" http://127.0.0.1:8000/columns

### Servicio persistente (systemd):
### Crear servicio:
sudo tee /etc/systemd/system/kanban-api.service > /dev/null << 'EOF'
[Unit]
Description=Kanban FastAPI
After=network.target

[Service]
User=facundo
Group=facundo
WorkingDirectory=/home/facundo/Ejercicio04_kanban/backend
EnvironmentFile=/home/facundo/Ejercicio04_kanban/backend/.env
ExecStart=/home/facundo/Ejercicio04_kanban/backend/.venv/bin/uvicorn app.main:app --host 0.0.0.0 --port 8000
Restart=always
RestartSec=3

[Install]
WantedBy=multi-user.target
EOF

### Activar servicio:
sudo systemctl daemon-reload
sudo systemctl enable --now kanban-api
sudo systemctl status kanban-api --no-pager

### Verificar puerto:
sudo ss -ltnp | grep 8000

### Acceso desde otra PC:
1) Instalar y ejecutar cliente Qt en la otra PC.
2) Configurar:
- URL API: http://173.212.234.190:8000
- Token: 1234567890
3) Conectar.

### Pruebas rápidas de red desde otra PC:
curl -i http://173.212.234.190:8000/health
curl -i -H "Authorization: Bearer 1234567890" http://173.212.234.190:8000/columns
