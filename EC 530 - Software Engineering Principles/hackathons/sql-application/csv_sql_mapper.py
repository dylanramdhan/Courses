"""
    This file contains a function to loading CSV files and mapping into SQLite database tables,
        by using the validator module to handle any potential schema conflicts.
"""

import pandas as pd
import sqlite3
import os
from validator import validate_or_prompt

def load_csv_and_map_to_sql(csv_path, db_path):
    table_name = os.path.splitext(os.path.basename(csv_path))[0]

    try:
        df = pd.read_csv(csv_path)
    except Exception as e:
        print(f"Failed to read CSV: {e}")
        return

    conn = sqlite3.connect(db_path)

    if not validate_or_prompt(conn, table_name):
        print(f"Skipping table creation for {table_name}.")
        conn.close()
        return

    try:
        df.to_sql(table_name, conn, if_exists='replace', index=False)
        print(f"Table '{table_name}' created and data inserted.")
    except Exception as e:
        with open("error_log.txt", "a") as log:
            log.write(f"Error inserting into table '{table_name}': {e}\n")
        print(f"Error inserting into table '{table_name}'. See error_log.txt for details.")
    finally:
        conn.close()
