from fastapi import FastAPI, HTTPException
import mysql.connector
from jose import jwt
from datetime import datetime, timedelta

app = FastAPI()

# Configuración de la base de datos
db_config = {
    "host": "mysql",
    "user": "root",
    "password": "root",
    "database": "empresa_db"
}

# Clave secreta para el token
SECRET_KEY = "mi_clave_secreta"
ALGORITHM = "HS256"

def get_user(username, password):
    conn = mysql.connector.connect(**db_config)
    cursor = conn.cursor(dictionary=True)

    query = "SELECT * FROM usuarios WHERE username=%s AND password=%s"
    cursor.execute(query, (username, password))
    user = cursor.fetchone()

    cursor.close()
    conn.close()

    return user

@app.post("/login")
def login(data: dict):
    username = data.get("username")
    password = data.get("password")

    user = get_user(username, password)

    if not user:
        raise HTTPException(status_code=401, detail="Credenciales incorrectas")

    # Crear token
    payload = {
        "sub": username,
        "exp": datetime.utcnow() + timedelta(hours=1)
    }

    token = jwt.encode(payload, SECRET_KEY, algorithm=ALGORITHM)

    return {"access_token": token}