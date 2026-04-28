from typing import List

from pydantic import BaseModel, ConfigDict


class ColumnBase(BaseModel):
    title: str


class ColumnCreate(ColumnBase):
    pass


class ColumnUpdate(BaseModel):
    title: str


class CardInColumn(BaseModel):
    model_config = ConfigDict(from_attributes=True)

    id: int
    title: str
    description: str
    column_id: int
    position: int


class ColumnOut(ColumnBase):
    model_config = ConfigDict(from_attributes=True)

    id: int
    cards: List[CardInColumn] = []
