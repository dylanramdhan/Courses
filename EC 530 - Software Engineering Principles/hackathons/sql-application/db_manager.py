"""
    This script contains a function to load a CSV file into a SQLite database.
"""


import pandas as pd
import sqlite3
import os

def load_csv_to_sqlite(csv_path, db_path, table_name):
    if not os.path.exists(csv_path):
        raise FileNotFoundError(f"CSV file not found: {csv_path}")

    df = pd.read_csv(csv_path)
    conn = sqlite3.connect(db_path)
    df.to_sql(table_name, conn, if_exists='replace', index=False)
    conn.close()
    print(f"Loaded {table_name} into SQLite DB.")
    

# Example usage:
# load_csv_to_sqlite("data/sales.csv", "data/my_database.db", "sales")
