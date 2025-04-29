"""
   Final Project for EC530
   Engineer: Dylan Ramdhan

   Welcome to Grade Mate!
   
   This program is designed to help professors manage their grades and assignments. 
   It allows professors to create assignments, add students, and calculate final grades.

"""

# Project: Professor Assistant Tool with LLM, REST API, and Frontend (React Ready)

# Initial Backend Skeleton (FastAPI-based)
# File: backend/main.py

from fastapi import FastAPI, UploadFile, File, Form, Request
from fastapi.middleware.cors import CORSMiddleware
from fastapi.responses import JSONResponse
import sqlite3
import pandas as pd
import os
import io

app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

DB_PATH = "./data/professor_tool.db"
os.makedirs("./data", exist_ok=True)

# Utility: Connect to database
def get_conn():
    return sqlite3.connect(DB_PATH)

@app.get("/")
def root():
    return {"message": "Professor Assistant API is running"}

@app.post("/upload_csv")
def upload_csv(file: UploadFile = File(...), table_name: str = Form(...)):
    try:
        df = pd.read_csv(io.BytesIO(file.file.read()))
        conn = get_conn()
        df.to_sql(table_name, conn, if_exists='replace', index=False)
        conn.close()
        return {"message": f"CSV uploaded and table '{table_name}' created with {len(df)} rows."}
    except Exception as e:
        return JSONResponse(status_code=500, content={"error": str(e)})

@app.get("/list_tables")
def list_tables():
    conn = get_conn()
    cursor = conn.cursor()
    cursor.execute("SELECT name FROM sqlite_master WHERE type='table';")
    tables = [row[0] for row in cursor.fetchall()]
    conn.close()
    return {"tables": tables}

@app.get("/preview_table/{table_name}")
def preview_table(table_name: str):
    conn = get_conn()
    df = pd.read_sql_query(f"SELECT * FROM {table_name} LIMIT 10", conn)
    conn.close()
    return df.to_dict(orient='records')

@app.post("/ask")
def ask_question(request: Request):
    from llm_sql_generator import ask_llm  # Imported here to delay if OpenAI not needed yet
    body = await request.json()
    user_question = body.get("question")
    try:
        conn = get_conn()
        schema = ""
        cursor = conn.cursor()
        cursor.execute("SELECT name FROM sqlite_master WHERE type='table';")
        tables = [row[0] for row in cursor.fetchall()]
        for table in tables:
            cursor.execute(f"PRAGMA table_info({table})")
            schema += f"Table {table}: {[col[1] + ' ' + col[2] for col in cursor.fetchall()]}\n"
        conn.close()

        query = ask_llm(schema, user_question)

        conn = get_conn()
        df = pd.read_sql_query(query, conn)
        conn.close()
        return {"generated_sql": query, "results": df.to_dict(orient='records')}

    except Exception as e:
        return JSONResponse(status_code=500, content={"error": str(e)})
