from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from sqlalchemy import text

from app.database import SessionLocal, engine
from app.models import entities  # noqa: F401 - Importa modelos para metadata
from app.database import Base
from app.routers import cards, columns

app = FastAPI(title="Kanban API", version="1.0.0")

# Configurar CORS para permitir conexiones remotas
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # En producción, especificar dominios permitidos
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

app.include_router(columns.router)
app.include_router(cards.router)


@app.on_event("startup")
def startup():
    # Crea tablas si no existen para simplificar arranque en entornos nuevos.
    try:
        Base.metadata.create_all(bind=engine)
    except Exception as exc:
        # No abortar el servidor: permite diagnosticar por /health si hay problema de DB.
        print(f"Startup DB init warning: {exc}")


@app.get("/health")
def health_check():
    db_status = "ok"
    try:
        with SessionLocal() as db:
            db.execute(text("SELECT 1"))
    except Exception as exc:
        db_status = f"error: {exc.__class__.__name__}"

    return {"status": "ok" if db_status == "ok" else "degraded", "db": db_status}
