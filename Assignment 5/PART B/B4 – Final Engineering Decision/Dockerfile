FROM python:3.12-slim
WORKDIR /app
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt \
    && groupadd --system portal \
    && useradd --system --gid portal --no-create-home \
       --home-dir /nonexistent --shell /usr/sbin/nologin portal
COPY --chown=portal:portal app.py /app/app.py
USER portal
EXPOSE 8000
CMD ["gunicorn", "--workers", "2", "--bind", "0.0.0.0:8000", "app:app"]
