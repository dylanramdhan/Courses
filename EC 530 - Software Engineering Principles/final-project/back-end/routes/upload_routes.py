from flask import Blueprint, request
import sqlite3
import pandas as pd
import os

upload_blueprint = Blueprint("upload", __name__)
DB_PATH = "./data/professor.db"
os.makedirs("./data", exist_ok=True)

def get_db_connection():
    return sqlite3.connect(DB_PATH)

@upload_blueprint.route("/upload", methods=["POST"])
def upload_csv():
    if "file" not in request.files or "table_name" not in request.form:
        return {"error": "file and table_name required"}, 400

    file = request.files["file"]
    table_name = request.form["table_name"]

    try:
        df = pd.read_csv(file)
        conn = get_db_connection()
        df.to_sql(table_name, conn, if_exists="replace", index=False)
        conn.close()

        return {"message": f"Uploaded successfully as table '{table_name}' with {len(df)} rows."}
    except Exception as e:
        return {"error": str(e)}, 500