from flask import Flask, jsonify, request
from datetime import datetime, timezone
import hashlib

app = Flask(__name__)

@app.get("/")
def index():
    return (
        "<!doctype html><html><head><title>University Result Portal</title></head>"
        "<body><h1>University Result Portal</h1>"
        "<p>Result service is running.</p>"
        "<a href='/health'>Health</a></body></html>",
        200,
        {"Content-Type": "text/html; charset=utf-8"},
    )

@app.get("/health")
def health():
    return jsonify(
        status="ok",
        service="result-portal",
        timestamp=datetime.now(timezone.utc).isoformat()
    ), 200

@app.get("/work")
def work():
    """Keep the B3 CPU-work endpoint so B4 capacity uses comparable B3 measurements."""
    try:
        iterations = int(request.args.get("n", "10000"))
    except ValueError:
        return jsonify(error="n must be an integer"), 400

    if iterations < 1 or iterations > 100000:
        return jsonify(error="n must be between 1 and 100000"), 400

    digest = b"result-portal-b3"
    for _ in range(iterations):
        digest = hashlib.sha256(digest).digest()

    return jsonify(status="ok", iterations=iterations, digest=digest.hex()), 200
