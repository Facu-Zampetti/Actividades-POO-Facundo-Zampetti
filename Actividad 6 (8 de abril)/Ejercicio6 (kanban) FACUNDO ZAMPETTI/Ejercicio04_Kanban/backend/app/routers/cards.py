from fastapi import APIRouter, Depends, HTTPException, status
from sqlalchemy import asc
from sqlalchemy.orm import Session

from app.core.security import require_auth
from app.database import get_db
from app.models import Card, CardOrder, Column
from app.schemas.card import CardCreate, CardMove, CardOut, CardUpdate, ReorderRequest

router = APIRouter(prefix="/cards", tags=["cards"], dependencies=[Depends(require_auth)])


def _compact_positions(db: Session, column_id: int):
    ordered = (
        db.query(CardOrder)
        .filter(CardOrder.column_id == column_id)
        .order_by(asc(CardOrder.position))
        .all()
    )
    for i, row in enumerate(ordered):
        row.position = i


@router.get("", response_model=list[CardOut])
def list_cards(db: Session = Depends(get_db)):
    cards = (
        db.query(Card, CardOrder)
        .join(CardOrder, Card.id == CardOrder.card_id)
        .order_by(Card.column_id.asc(), CardOrder.position.asc())
        .all()
    )
    return [
        {
            "id": card.id,
            "title": card.title,
            "description": card.description or "",
            "column_id": card.column_id,
            "position": order.position,
        }
        for card, order in cards
    ]


@router.post("", response_model=CardOut, status_code=status.HTTP_201_CREATED)
def create_card(payload: CardCreate, db: Session = Depends(get_db)):
    column = db.query(Column).filter(Column.id == payload.column_id).first()
    if not column:
        raise HTTPException(status_code=404, detail="Columna no encontrada")

    max_position = (
        db.query(CardOrder)
        .filter(CardOrder.column_id == payload.column_id)
        .order_by(CardOrder.position.desc())
        .first()
    )
    next_position = 0 if not max_position else max_position.position + 1

    card = Card(
        title=payload.title,
        description=payload.description,
        column_id=payload.column_id,
    )
    db.add(card)
    db.flush()

    order = CardOrder(card_id=card.id, column_id=payload.column_id, position=next_position)
    db.add(order)
    db.commit()
    db.refresh(card)

    return {
        "id": card.id,
        "title": card.title,
        "description": card.description or "",
        "column_id": card.column_id,
        "position": next_position,
    }


@router.put("/{card_id}", response_model=CardOut)
def update_card(card_id: int, payload: CardUpdate, db: Session = Depends(get_db)):
    card = db.query(Card).filter(Card.id == card_id).first()
    if not card:
        raise HTTPException(status_code=404, detail="Tarjeta no encontrada")

    order = db.query(CardOrder).filter(CardOrder.card_id == card_id).first()
    if not order:
        raise HTTPException(status_code=500, detail="Orden de tarjeta inconsistente")

    card.title = payload.title
    card.description = payload.description
    db.commit()
    db.refresh(card)

    return {
        "id": card.id,
        "title": card.title,
        "description": card.description or "",
        "column_id": card.column_id,
        "position": order.position,
    }


@router.post("/reorder/{column_id}", response_model=list[CardOut])
def reorder_cards(column_id: int, payload: ReorderRequest, db: Session = Depends(get_db)):
    try:
        column = db.query(Column).filter(Column.id == column_id).first()
        if not column:
            raise HTTPException(status_code=404, detail="Columna no encontrada")

        # Actualizar posiciones sin validación estricta
        order_map = {
            order.card_id: order
            for order in db.query(CardOrder).filter(CardOrder.column_id == column_id).all()
        }

        for position, card_id in enumerate(payload.ordered_card_ids):
            card_order = order_map.get(card_id)
            if card_order:
                card_order.position = -(position + 1)

        db.flush()

        for position, card_id in enumerate(payload.ordered_card_ids):
            card_order = order_map.get(card_id)
            if card_order:
                card_order.position = position

        db.commit()

        # Retornar las tarjetas reordenadas
        rows = (
            db.query(Card, CardOrder)
            .join(CardOrder, Card.id == CardOrder.card_id)
            .filter(Card.column_id == column_id)
            .order_by(CardOrder.position.asc())
            .all()
        )

        return [
            {
                "id": card.id,
                "title": card.title,
                "description": card.description or "",
                "column_id": card.column_id,
                "position": order.position,
            }
            for card, order in rows
        ]
    except Exception as e:
        print(f"Error in reorder_cards: {e}")
        import traceback
        traceback.print_exc()
        raise HTTPException(status_code=500, detail=f"Error: {str(e)}")


@router.delete("/{card_id}", status_code=status.HTTP_204_NO_CONTENT)
def delete_card(card_id: int, db: Session = Depends(get_db)):
    card = db.query(Card).filter(Card.id == card_id).first()
    if not card:
        raise HTTPException(status_code=404, detail="Tarjeta no encontrada")

    column_id = card.column_id
    db.delete(card)
    db.flush()
    _compact_positions(db, column_id)
    db.commit()


@router.post("/{card_id}/move", response_model=CardOut)
def move_card(card_id: int, payload: CardMove, db: Session = Depends(get_db)):
    card = db.query(Card).filter(Card.id == card_id).first()
    if not card:
        raise HTTPException(status_code=404, detail="Tarjeta no encontrada")

    target_column = db.query(Column).filter(Column.id == payload.target_column_id).first()
    if not target_column:
        raise HTTPException(status_code=404, detail="Columna destino no encontrada")

    order = db.query(CardOrder).filter(CardOrder.card_id == card_id).first()
    if not order:
        raise HTTPException(status_code=500, detail="Orden de tarjeta inconsistente")

    source_column_id = card.column_id

    source_orders = (
        db.query(CardOrder)
        .filter(CardOrder.column_id == source_column_id, CardOrder.position > order.position)
        .all()
    )
    for row in source_orders:
        row.position -= 1000

    target_orders = (
        db.query(CardOrder)
        .filter(CardOrder.column_id == payload.target_column_id, CardOrder.position >= payload.target_position)
        .all()
    )
    for row in target_orders:
        row.position += 1000

    target_count = db.query(CardOrder).filter(CardOrder.column_id == payload.target_column_id).count()
    target_position = max(0, min(payload.target_position, target_count))

    for row in source_orders:
        row.position += 999

    for row in target_orders:
        row.position -= 999

    card.column_id = payload.target_column_id
    order.column_id = payload.target_column_id
    order.position = target_position

    db.commit()
    db.refresh(card)

    return {
        "id": card.id,
        "title": card.title,
        "description": card.description or "",
        "column_id": card.column_id,
        "position": order.position,
    }
