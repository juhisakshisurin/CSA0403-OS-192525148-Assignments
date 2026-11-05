from flask import Flask, jsonify
from datetime import datetime, timezone

app = Flask(__name__)

@app.get("/")
def index():
    return (
        "<!doctype html><html><head><title>University LMS and Exam Service</title></head>"
        "<body><h1>University LMS and Online Examination Service</h1>"
        "<p>Second B4 service is running with independent resource limits.</p>"
        "<a href='/health'>Health</a></body></html>",
        200,
        {"Content-Type": "text/html; charset=utf-8"},
    )

@app.get("/health")
def health():
    return jsonify(
        status="ok",
        service="lms-exam",
        timestamp=datetime.now(timezone.utc).isoformat()
    ), 200

@app.get("/exam/status")
def exam_status():
    return jsonify(
        service="lms-exam",
        exam_status="sample-available",
        note="Demonstration endpoint only; no real exam data is stored."
    ), 200
