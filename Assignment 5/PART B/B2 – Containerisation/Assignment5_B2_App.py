import os
from datetime import datetime, timezone

from flask import Flask, jsonify
import psycopg

app = Flask(__name__)

def database_status():
    """Check database connectivity without exposing credentials."""
    db_host = os.getenv("DATABASE_HOST")
    if not db_host:
        return "not-configured"

    try:
        with psycopg.connect(
            host=db_host,
            port=os.getenv("DATABASE_PORT", "5432"),
            dbname=os.getenv("DATABASE_NAME", "result_portal"),
            user=os.getenv("DATABASE_USER", "portal"),
            password=os.getenv("DATABASE_PASSWORD", ""),
            connect_timeout=3,
        ) as conn:
            with conn.cursor() as cur:
                cur.execute("SELECT 1")
                cur.fetchone()
        return "connected"
    except Exception:
        app.logger.exception("Database connectivity check failed")
        return "unavailable"

@app.get("/")
def index():
    db = database_status()
    return (
        "<!doctype html><html><head><title>University Result Portal</title></head>"
        "<body><h1>University Result Portal</h1>"
        "<p>Sample Flask portal is running inside a Docker container.</p>"
        f"<p>Database status: <strong>{db}</strong></p>"
        "<p><a href='/healthz'>Application health</a></p></body></html>",
        200 if db in ("connected", "not-configured") else 503,
    )

@app.get("/healthz")
def health():
    return jsonify(
        status="ok",
        service="result-portal",
        timestamp=datetime.now(timezone.utc).isoformat(),
    ), 200

@app.get("/readyz")
def ready():
    db = database_status()
    ready_state = db in ("connected", "not-configured")
    return jsonify(status="ready" if ready_state else "not-ready",
                   database=db), 200 if ready_state else 503
