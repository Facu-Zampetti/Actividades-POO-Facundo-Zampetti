from fastapi import APIRouter, Depends, HTTPException, status
from sqlalchemy import asc
from sqlalchemy.orm import Session

from app.core.security import require_auth
from app.database import get_db
from app.models import Card, CardOrder, Column
from app.schemas.column import ColumnCreate, ColumnOut, ColumnUpdate

router = APIRouter(prefix="/columns", tags=["columns"], dependencies=[Depends(require_auth)])


@router.get("", response_model=list[ColumnOut])
def list_columns(db: Session = Depends(get_db)):
    try:
        columns = db.query(Column).order_by(Column.id.asc()).all()
        result = []

        for column in columns:
            cards = (
                db.query(Card, CardOrder)
                .join(CardOrder, Card.id == CardOrder.card_id)
                .filter(Card.column_id == column.id)
                .order_by(asc(CardOrder.position))
                .all()
            )

            column_payload = {
                "id": column.id,
                "title": column.title,
                "cards": [
                    {
                        "id": card.id,
                        "title": card.title,
                        "description": card.description or "",
                        "column_id": card.column_id,
                        "position": order.position,
                    }
                    for card, order in cards
                ],
            }
            result.append(column_payload)

        return result
    except Exception as e:
        print(f"Error in list_columns: {e}")
        raise


@router.post("", response_model=ColumnOut, status_code=status.HTTP_201_CREATED)
def create_column(payload: ColumnCreate, db: Session = Depends(get_db)):
    column = Column(title=payload.title)
    db.add(column)
    db.commit()
    db.refresh(column)
    return {"id": column.id, "title": column.title, "cards": []}


@router.put("/{column_id}", response_model=ColumnOut)
def update_column(column_id: int, payload: ColumnUpdate, db: Session = Depends(get_db)):
    column = db.query(Column).filter(Column.id == column_id).first()
    if not column:
        raise HTTPException(status_code=404, detail="Columna no encontrada")

    column.title = payload.title
    db.commit()
    db.refresh(column)

    cards = (
        db.query(Card, CardOrder)
        .join(CardOrder, Card.id == CardOrder.card_id)
        .filter(Card.column_id == column.id)
        .order_by(asc(CardOrder.position))
        .all()
    )

    return {
        "id": column.id,
        "title": column.title,
        "cards": [
            {
                "id": card.id,
                "title": card.title,
                "description": card.description or "",
                "column_id": card.column_id,
                "position": order.position,
            }
            for card, order in cards
        ],
    }


@router.delete("/{column_id}", status_code=status.HTTP_204_NO_CONTENT)
def delete_column(column_id: int, db: Session = Depends(get_db)):
    column = db.query(Column).filter(Column.id == column_id).first()
    if not column:
        raise HTTPException(status_code=404, detail="Columna no encontrada")

    db.delete(column)
    db.commit()
