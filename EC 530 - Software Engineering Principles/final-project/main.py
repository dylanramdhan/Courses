"""
   Final Project for EC530
   Engineer: Dylan Ramdhan

   Welcome to Grade Mate!

   This program is designed to help professors manage their grades and assignments.
   It allows professors to create assignments, add students, and calculate final grades.
"""

from flask import Flask, request, jsonify
import sqlite3
import os
import pandas as pd
from flask_cors import CORS

app = Flask(__name__)
CORS(app)  # Allow cross-origin requests (for frontend access)

DB_PATH = "./data/professor_tool.db"
os.makedirs("./data", exist_ok=True)

def get_conn():
    return sqlite3.connect(DB_PATH)

@app.route("/", methods=["GET"])
def home():
    return jsonify({"message": "Professor Assistant Flask API is running."})

@app.route("/upload_csv", methods=["POST"])
def upload_csv():
    file = request.files.get("file")
    table_name = request.form.get("table_name")
    if not file or not table_name:
        return jsonify({"error": "File or table name missing"}), 400
    try:
        df = pd.read_csv(file)
        conn = get_conn()
        df.to_sql(table_name, conn, if_exists='replace', index=False)
        conn.close()
        return jsonify({"message": f"CSV uploaded and table '{table_name}' created with {len(df)} rows."})
    except Exception as e:
        return jsonify({"error": str(e)}), 500

@app.route("/list_tables", methods=["GET"])
def list_tables():
    try:
        conn = get_conn()
        cursor = conn.cursor()
        cursor.execute("SELECT name FROM sqlite_master WHERE type='table';")
        tables = [row[0] for row in cursor.fetchall()]
        conn.close()
        return jsonify({"tables": tables})
    except Exception as e:
        return jsonify({"error": str(e)}), 500

@app.route("/preview_table/<table_name>", methods=["GET"])
def preview_table(table_name):
    try:
        conn = get_conn()
        df = pd.read_sql_query(f"SELECT * FROM {table_name} LIMIT 10", conn)
        conn.close()
        return jsonify(df.to_dict(orient='records'))
    except Exception as e:
        return jsonify({"error": str(e)}), 500

@app.route("/ask", methods=["POST"])
def ask_question():
    from llm_sql_generator import ask_llm
    data = request.get_json()
    user_question = data.get("question")
    if not user_question:
        return jsonify({"error": "Missing question"}), 400
    try:
        conn = get_conn()
        cursor = conn.cursor()
        cursor.execute("SELECT name FROM sqlite_master WHERE type='table';")
        tables = [row[0] for row in cursor.fetchall()]
        schema = ""
        for table in tables:
            cursor.execute(f"PRAGMA table_info({table})")
            schema += f"Table {table}: {[col[1] + ' ' + col[2] for col in cursor.fetchall()]}
"
        conn.close()

        query = ask_llm(schema, user_question)
        conn = get_conn()
        df = pd.read_sql_query(query, conn)
        conn.close()
        return jsonify({"generated_sql": query, "results": df.to_dict(orient='records')})
    except Exception as e:
        return jsonify({"error": str(e)}), 500

if __name__ == "__main__":
    app.run(debug=True, port=5000)
