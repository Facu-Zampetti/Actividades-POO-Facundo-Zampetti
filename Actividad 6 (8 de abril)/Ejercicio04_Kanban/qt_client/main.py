import sys
import json
from pathlib import Path
from dataclasses import dataclass

import requests
from requests import HTTPError
from PySide6.QtCore import QTimer, Qt
from PySide6.QtWidgets import (
    QApplication,
    QDialog,
    QDialogButtonBox,
    QFormLayout,
    QFrame,
    QHBoxLayout,
    QInputDialog,
    QLabel,
    QLineEdit,
    QMainWindow,
    QMessageBox,
    QPlainTextEdit,
    QPushButton,
    QScrollArea,
    QSizePolicy,
    QVBoxLayout,
    QWidget,
)


CONFIG_FILE = Path(__file__).resolve().parent / "config.json"


APP_STYLESHEET = """
QMainWindow {
    background-color: #f2f5f9;
}

QWidget {
    font-family: "Segoe UI", "Trebuchet MS", sans-serif;
    color: #12263a;
    font-size: 12px;
}

QLabel#TopTitle {
    font-size: 18px;
    font-weight: 700;
    color: #0f2744;
}

QFrame#TopBar {
    background-color: #ffffff;
    border: 1px solid #d5deea;
    border-radius: 12px;
    padding: 6px;
}

QFrame#ColumnFrame {
    background-color: #f8fbff;
    border: 1px solid #d4e1ef;
    border-radius: 14px;
}

QLabel#ColumnTitle {
    font-size: 16px;
    font-weight: 700;
    color: #13324f;
}

QFrame#CardFrame {
    background-color: #ffffff;
    border: 1px solid #e3e8f1;
    border-radius: 10px;
}

QLabel#CardTitle {
    font-size: 13px;
    font-weight: 700;
    color: #19344f;
}

QLabel#CardDescription {
    font-size: 12px;
    color: #2f4964;
}

QLineEdit, QPlainTextEdit {
    background-color: #ffffff;
    border: 1px solid #c8d5e3;
    border-radius: 8px;
    padding: 6px;
}

QLineEdit:focus, QPlainTextEdit:focus {
    border: 1px solid #3d84d6;
}

QPushButton {
    background-color: #1f6fb2;
    color: white;
    border: none;
    border-radius: 8px;
    padding: 6px 10px;
    font-weight: 600;
}

QPushButton:hover {
    background-color: #195c94;
}

QPushButton[role="secondary"] {
    background-color: #e9f0f8;
    color: #18314a;
}

QPushButton[role="secondary"]:hover {
    background-color: #dce9f7;
}

QPushButton[role="danger"] {
    background-color: #d94b4b;
    color: white;
}

QPushButton[role="danger"]:hover {
    background-color: #bf3d3d;
}
"""


@dataclass
class ApiClient:
    base_url: str
    token: str

    def _headers(self):
        return {"Authorization": f"Bearer {self.token}"}

    def list_columns(self):
        response = requests.get(f"{self.base_url}/columns", headers=self._headers(), timeout=10)
        if response.status_code == 405:
            response = requests.get(f"{self.base_url}/columns/", headers=self._headers(), timeout=10)
        response.raise_for_status()
        return response.json()

    def create_column(self, title: str):
        response = requests.post(
            f"{self.base_url}/columns",
            json={"title": title, "name": title},
            headers=self._headers(),
            timeout=10,
        )
        response.raise_for_status()

    def update_column(self, column_id: int, title: str):
        response = requests.put(
            f"{self.base_url}/columns/{column_id}",
            json={"title": title, "name": title},
            headers=self._headers(),
            timeout=10,
        )
        response.raise_for_status()

    def delete_column(self, column_id: int):
        response = requests.delete(
            f"{self.base_url}/columns/{column_id}",
            headers=self._headers(),
            timeout=10,
        )
        response.raise_for_status()

    def create_card(self, title: str, description: str, column_id: int):
        response = requests.post(
            f"{self.base_url}/cards",
            json={"title": title, "description": description, "column_id": column_id},
            headers=self._headers(),
            timeout=10,
        )
        response.raise_for_status()

    def update_card(self, card_id: int, title: str, description: str):
        response = requests.put(
            f"{self.base_url}/cards/{card_id}",
            json={"title": title, "description": description},
            headers=self._headers(),
            timeout=10,
        )
        response.raise_for_status()

    def delete_card(self, card_id: int):
        response = requests.delete(
            f"{self.base_url}/cards/{card_id}",
            headers=self._headers(),
            timeout=10,
        )
        response.raise_for_status()

    def move_card(self, card_id: int, target_column_id: int, target_position: int):
        payload = {
            "target_column_id": target_column_id,
            "target_position": target_position,
            "column_id": target_column_id,
            "position": target_position,
        }
        response = requests.post(
            f"{self.base_url}/cards/{card_id}/move",
            json=payload,
            headers=self._headers(),
            timeout=10,
        )
        if response.status_code == 405:
            response = requests.put(
                f"{self.base_url}/cards/{card_id}/move",
                json=payload,
                headers=self._headers(),
                timeout=10,
            )
        response.raise_for_status()

    def reorder_cards(self, column_id: int, ordered_ids: list[int]):
        response = requests.post(
            f"{self.base_url}/cards/reorder/{column_id}",
            json={"ordered_card_ids": ordered_ids},
            headers=self._headers(),
            timeout=10,
        )
        response.raise_for_status()


@dataclass
class ClientConfig:
    api_url: str = "http://173.212.234.190:8000"
    api_token: str = "change_me"
    polling_seconds: int = 4


def load_config() -> ClientConfig:
    if not CONFIG_FILE.exists():
        return ClientConfig()

    try:
        data = json.loads(CONFIG_FILE.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError):
        return ClientConfig()

    return ClientConfig(
        api_url=data.get("api_url", "http://173.212.234.190:8000").rstrip("/"),
        api_token=data.get("api_token", "change_me"),
        polling_seconds=max(3, min(int(data.get("polling_seconds", 4)), 10)),
    )


def save_config(config: ClientConfig):
    payload = {
        "api_url": config.api_url,
        "api_token": config.api_token,
        "polling_seconds": config.polling_seconds,
    }
    CONFIG_FILE.write_text(json.dumps(payload, indent=2), encoding="utf-8")


class CardDialog(QDialog):
    def __init__(self, title="Nueva tarjeta", card_title="", card_description=""):
        super().__init__()
        self.setWindowTitle(title)
        self.setMinimumWidth(420)
        self.title_edit = QLineEdit(card_title)
        self.description_edit = QPlainTextEdit(card_description)
        self.description_edit.setMinimumHeight(90)

        form = QFormLayout()
        form.addRow("Titulo:", self.title_edit)
        form.addRow("Descripcion:", self.description_edit)

        buttons = QDialogButtonBox(QDialogButtonBox.Ok | QDialogButtonBox.Cancel)
        buttons.accepted.connect(self.accept)
        buttons.rejected.connect(self.reject)

        layout = QVBoxLayout()
        layout.addLayout(form)
        layout.addWidget(buttons)
        self.setLayout(layout)

    def values(self):
        return self.title_edit.text().strip(), self.description_edit.toPlainText().strip()


class KanbanWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Kanban Colaborativo (Qt + FastAPI)")
        self.resize(1200, 700)
        self.setStyleSheet(APP_STYLESHEET)

        self.client = None
        self.board_data = []
        self.config = load_config()

        self.base_url_edit = QLineEdit(self.config.api_url)
        self.base_url_edit.setPlaceholderText("http://ip-del-vps:8000")
        self.token_edit = QLineEdit(self.config.api_token)
        self.token_edit.setEchoMode(QLineEdit.Password)

        self.connect_btn = QPushButton("Conectar")
        self.connect_btn.clicked.connect(self.connect_api)

        add_column_btn = QPushButton("Nueva columna")
        add_column_btn.setProperty("role", "secondary")
        add_column_btn.clicked.connect(self.create_column)

        refresh_btn = QPushButton("Refrescar")
        refresh_btn.setProperty("role", "secondary")
        refresh_btn.clicked.connect(self.fetch_board)

        title_label = QLabel("Tablero Kanban")
        title_label.setObjectName("TopTitle")

        top_frame = QFrame()
        top_frame.setObjectName("TopBar")

        top = QHBoxLayout()
        top.addWidget(title_label)
        top.addWidget(QLabel("API:"))
        top.addWidget(self.base_url_edit, 2)
        top.addWidget(QLabel("Token:"))
        top.addWidget(self.token_edit, 1)
        top.addWidget(self.connect_btn)
        top.addWidget(add_column_btn)
        top.addWidget(refresh_btn)
        top_frame.setLayout(top)

        self.board_container = QWidget()
        self.board_layout = QHBoxLayout(self.board_container)
        self.board_layout.setSpacing(14)
        self.board_layout.setAlignment(Qt.AlignLeft | Qt.AlignTop)

        scroll = QScrollArea()
        scroll.setFrameShape(QFrame.NoFrame)
        scroll.setWidgetResizable(True)
        scroll.setWidget(self.board_container)

        wrapper = QWidget()
        root = QVBoxLayout(wrapper)
        root.addWidget(top_frame)
        root.addWidget(scroll)
        self.setCentralWidget(wrapper)

        self.statusBar().showMessage("Listo para conectar")

        self.timer = QTimer(self)
        self.timer.setInterval(self.config.polling_seconds * 1000)
        self.timer.timeout.connect(self.fetch_board)

    @staticmethod
    def _format_request_error(exc: requests.RequestException) -> str:
        if isinstance(exc, HTTPError) and exc.response is not None:
            status = exc.response.status_code
            body = exc.response.text.strip()
            if status == 401:
                return "401 Unauthorized: token/credenciales invalidas."
            if status == 405:
                return (
                    "405 Method Not Allowed: el backend en el VPS no coincide con esta version de la app "
                    "o hay un proxy bloqueando metodos HTTP."
                )
            if status == 422:
                return f"422 Unprocessable Entity: payload invalido para ese backend. Respuesta: {body}"
            return f"HTTP {status}: {body or 'sin detalle'}"

        text = str(exc)
        lowered = text.lower()
        if "failed to establish a new connection" in lowered or "winerror 10061" in lowered:
            return (
                "Conexion rechazada (WinError 10061): en 173.212.234.190:8000 no hay proceso escuchando "
                "o el firewall bloquea el puerto."
            )
        return text

    def show_error(self, message: str):
        QMessageBox.critical(self, "Error", message)

    def connect_api(self):
        base_url = self.base_url_edit.text().strip().rstrip("/")
        token = self.token_edit.text().strip()
        if not base_url or not token:
            self.show_error("Debes completar URL y token.")
            return

        candidate = ApiClient(base_url, token)
        try:
            self.board_data = candidate.list_columns()
            self.client = candidate
            self.config = ClientConfig(
                api_url=base_url,
                api_token=token,
                polling_seconds=max(3, min(self.timer.interval() // 1000, 10)),
            )
            save_config(self.config)
            self.connect_btn.setText("Conectado")
            self.statusBar().showMessage(f"Conectado a {base_url} | polling {self.config.polling_seconds}s")
            self.render_board()
            self.timer.start()
        except requests.RequestException as exc:
            self.client = None
            self.timer.stop()
            self.show_error(f"No se pudo conectar a la API: {self._format_request_error(exc)}")

    def clear_board(self):
        while self.board_layout.count():
            item = self.board_layout.takeAt(0)
            widget = item.widget()
            if widget:
                widget.deleteLater()

    def fetch_board(self):
        if not self.client:
            return
        try:
            self.board_data = self.client.list_columns()
            self.render_board()
        except requests.RequestException as exc:
            self.show_error(f"No se pudo obtener tablero: {self._format_request_error(exc)}")
            self.timer.stop()

    def render_board(self):
        self.clear_board()
        for index, column in enumerate(self.board_data):
            frame = QFrame()
            frame.setObjectName("ColumnFrame")
            frame.setMinimumWidth(300)
            frame.setMaximumWidth(360)
            frame.setSizePolicy(QSizePolicy.Preferred, QSizePolicy.Expanding)

            vbox = QVBoxLayout(frame)
            vbox.setContentsMargins(12, 12, 12, 12)
            vbox.setSpacing(8)

            title = QLabel(column["title"])
            title.setObjectName("ColumnTitle")
            vbox.addWidget(title)

            col_actions = QHBoxLayout()
            col_actions.setSpacing(6)
            edit_col_btn = QPushButton("Editar")
            edit_col_btn.setProperty("role", "secondary")
            edit_col_btn.clicked.connect(
                lambda _, cid=column["id"], ctitle=column["title"]: self.edit_column(cid, ctitle)
            )
            del_col_btn = QPushButton("Eliminar")
            del_col_btn.setProperty("role", "danger")
            del_col_btn.clicked.connect(lambda _, cid=column["id"]: self.delete_column(cid))
            add_card_btn = QPushButton("+ Tarjeta")
            add_card_btn.setProperty("role", "secondary")
            add_card_btn.clicked.connect(lambda _, cid=column["id"]: self.create_card(cid))
            col_actions.addWidget(edit_col_btn)
            col_actions.addWidget(del_col_btn)
            col_actions.addWidget(add_card_btn)
            vbox.addLayout(col_actions)

            for card_index, card in enumerate(column.get("cards", [])):
                card_box = QFrame()
                card_box.setObjectName("CardFrame")
                card_layout = QVBoxLayout(card_box)
                card_layout.setContentsMargins(10, 10, 10, 10)
                card_layout.setSpacing(8)

                card_title = QLabel(card["title"])
                card_title.setObjectName("CardTitle")
                card_desc = QLabel(card.get("description", ""))
                card_desc.setObjectName("CardDescription")
                card_desc.setWordWrap(True)

                card_layout.addWidget(card_title)
                card_layout.addWidget(card_desc)

                actions = QHBoxLayout()
                actions.setSpacing(4)
                edit_btn = QPushButton("Editar")
                edit_btn.setProperty("role", "secondary")
                edit_btn.clicked.connect(
                    lambda _, c=card: self.edit_card(c["id"], c["title"], c.get("description", ""))
                )
                del_btn = QPushButton("Eliminar")
                del_btn.setProperty("role", "danger")
                del_btn.clicked.connect(lambda _, cid=card["id"]: self.delete_card(cid))
                up_btn = QPushButton("Arriba")
                up_btn.setProperty("role", "secondary")
                up_btn.clicked.connect(
                    lambda _, col=column, idx=card_index: self.move_inside_column(col["id"], idx, -1)
                )
                down_btn = QPushButton("Abajo")
                down_btn.setProperty("role", "secondary")
                down_btn.clicked.connect(
                    lambda _, col=column, idx=card_index: self.move_inside_column(col["id"], idx, 1)
                )
                left_btn = QPushButton("<-")
                left_btn.setProperty("role", "secondary")
                left_btn.clicked.connect(
                    lambda _, col_idx=index, c=card: self.move_to_neighbor_column(col_idx, c["id"], -1)
                )
                right_btn = QPushButton("->")
                right_btn.setProperty("role", "secondary")
                right_btn.clicked.connect(
                    lambda _, col_idx=index, c=card: self.move_to_neighbor_column(col_idx, c["id"], 1)
                )

                actions.addWidget(edit_btn)
                actions.addWidget(del_btn)
                actions.addWidget(up_btn)
                actions.addWidget(down_btn)
                actions.addWidget(left_btn)
                actions.addWidget(right_btn)

                card_layout.addLayout(actions)
                vbox.addWidget(card_box)

            vbox.addStretch(1)
            self.board_layout.addWidget(frame)

        self.board_layout.addStretch(1)

    def create_column(self):
        if not self.client:
            self.show_error("Conectate primero a la API.")
            return

        title, ok = QInputDialog.getText(self, "Nueva columna", "Titulo de columna:")
        if ok and title.strip():
            try:
                self.client.create_column(title.strip())
                self.fetch_board()
            except requests.RequestException as exc:
                self.show_error(self._format_request_error(exc))

    def edit_column(self, column_id: int, current_title: str):
        title, ok = QInputDialog.getText(self, "Editar columna", "Titulo:", text=current_title)
        if ok and title.strip():
            try:
                self.client.update_column(column_id, title.strip())
                self.fetch_board()
            except requests.RequestException as exc:
                self.show_error(self._format_request_error(exc))

    def delete_column(self, column_id: int):
        if QMessageBox.question(self, "Confirmar", "Eliminar columna y sus tarjetas?") != QMessageBox.Yes:
            return
        try:
            self.client.delete_column(column_id)
            self.fetch_board()
        except requests.RequestException as exc:
            self.show_error(self._format_request_error(exc))

    def create_card(self, column_id: int):
        dialog = CardDialog()
        if dialog.exec() == QDialog.Accepted:
            title, description = dialog.values()
            if not title:
                self.show_error("El titulo no puede estar vacio")
                return
            try:
                self.client.create_card(title, description, column_id)
                self.fetch_board()
            except requests.RequestException as exc:
                self.show_error(self._format_request_error(exc))

    def edit_card(self, card_id: int, card_title: str, card_description: str):
        dialog = CardDialog("Editar tarjeta", card_title, card_description)
        if dialog.exec() == QDialog.Accepted:
            title, description = dialog.values()
            if not title:
                self.show_error("El titulo no puede estar vacio")
                return
            try:
                self.client.update_card(card_id, title, description)
                self.fetch_board()
            except requests.RequestException as exc:
                self.show_error(self._format_request_error(exc))

    def delete_card(self, card_id: int):
        if QMessageBox.question(self, "Confirmar", "Eliminar tarjeta?") != QMessageBox.Yes:
            return
        try:
            self.client.delete_card(card_id)
            self.fetch_board()
        except requests.RequestException as exc:
            self.show_error(self._format_request_error(exc))

    def move_inside_column(self, column_id: int, current_index: int, delta: int):
        cards = None
        for col in self.board_data:
            if col["id"] == column_id:
                cards = col.get("cards", [])
                break

        if cards is None:
            return

        target = current_index + delta
        if target < 0 or target >= len(cards):
            return

        ordered_ids = [card["id"] for card in cards]
        ordered_ids[current_index], ordered_ids[target] = ordered_ids[target], ordered_ids[current_index]

        try:
            self.client.reorder_cards(column_id, ordered_ids)
            self.fetch_board()
        except requests.RequestException as exc:
            self.show_error(self._format_request_error(exc))

    def move_to_neighbor_column(self, column_index: int, card_id: int, direction: int):
        target_index = column_index + direction
        if target_index < 0 or target_index >= len(self.board_data):
            return

        target_column = self.board_data[target_index]
        target_position = len(target_column.get("cards", []))

        try:
            self.client.move_card(card_id, target_column["id"], target_position)
            self.fetch_board()
        except requests.RequestException as exc:
            self.show_error(self._format_request_error(exc))


def main():
    app = QApplication(sys.argv)
    win = KanbanWindow()
    win.show()
    sys.exit(app.exec())


if __name__ == "__main__":
    main()
