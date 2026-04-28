from datetime import datetime, timezone
from flask import Flask, jsonify
import psutil

app = Flask(__name__)


def compute_status(memory_used: float, disk_used: float, load_avg: float) -> str:
    if memory_used >= 95 or disk_used >= 98:
        return "down"
    if memory_used >= 85 or disk_used >= 90 or load_avg >= 2.5:
        return "alert"
    return "ok"


@app.get("/health")
def health():
    uptime_seconds = datetime.now(timezone.utc).timestamp() - psutil.boot_time()

    try:
        load_1m = psutil.getloadavg()[0]
    except (AttributeError, OSError):
        load_1m = 0.0

    memory_used_percent = psutil.virtual_memory().percent
    disk_used_percent = psutil.disk_usage("/").percent

    payload = {
        "status": compute_status(memory_used_percent, disk_used_percent, load_1m),
        "uptime_seconds": round(uptime_seconds, 2),
        "load_avg": round(load_1m, 2),
        "memory_used_percent": round(memory_used_percent, 2),
        "disk_used_percent": round(disk_used_percent, 2),
        "timestamp": datetime.now(timezone.utc).isoformat(),
    }
    return jsonify(payload)


if __name__ == "__main__":
    # Expose endpoint for external monitors.
    app.run(host="0.0.0.0", port=8080)
