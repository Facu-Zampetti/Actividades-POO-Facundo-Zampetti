from pydantic import BaseModel, ConfigDict


class CardBase(BaseModel):
    title: str
    description: str = ""


class CardCreate(CardBase):
    column_id: int


class CardUpdate(BaseModel):
    title: str
    description: str


class CardMove(BaseModel):
    target_column_id: int
    target_position: int


class ReorderRequest(BaseModel):
    ordered_card_ids: list[int]


class CardOut(CardBase):
    model_config = ConfigDict(from_attributes=True)

    id: int
    column_id: int
    position: int
