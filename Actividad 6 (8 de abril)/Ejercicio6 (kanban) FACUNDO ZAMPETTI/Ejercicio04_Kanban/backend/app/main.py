from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware

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


@app.get("/health")
def health_check():
    return {"status": "ok"}
