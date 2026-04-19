Set-Location C:\Ejercicio04_Kanban\qt_client

if (-not (Test-Path .venv)) {
    py -m venv .venv
}

.\.venv\Scripts\Activate.ps1
python -m pip install --upgrade pip
pip install -r requirements.txt
python main.py
