"""
    This file contains functions to:
        - validate the schema of a SQLite table
        - handle conflicts when loading data
        - log errors to a TXT file
"""

import sqlite3
import os
from typing import Literal

def validate_table_schema(conn: sqlite3.Connection, table_name: str) -> list:
    cursor = conn.cursor()
    cursor.execute(f"PRAGMA table_info({table_name})")
    schema = cursor.fetchall()
    return schema

def prompt_on_conflict(table_name: str) -> Literal["overwrite", "rename", "skip"]:
    print(f"Conflict detected: Table '{table_name}' already exists.")
    print("Options:")
    print("1. Overwrite the existing table")
    print("2. Rename the new table")
    print("3. Skip loading this table")

    while True:
        choice = input("Enter your choice (1/2/3): ").strip()
        if choice == "1":
            return "overwrite"
        elif choice == "2":
            return "rename"
        elif choice == "3":
            return "skip"
        else:
            print("Invalid choice. Please enter 1, 2, or 3.")

def log_error(message: str, filename: str = "error_log.txt"):
    with open(filename, "a") as log_file:
        log_file.write(message + "\n")

def check_table_conflict(table_name: str, conn: sqlite3.Connection) -> str:
    """
    Check if a table with the given name already exists in the database.
    If it does, prompt the user to choose: Overwrite, Rename, or Skip.
    Returns the final table name to use or None to skip.
    """
    cursor = conn.cursor()
    cursor.execute("SELECT name FROM sqlite_master WHERE type='table' AND name=?", (table_name,))
    existing = cursor.fetchone()

    if not existing:
        return table_name  # No conflict

    print(f"[WARNING] Table '{table_name}' already exists.")
    while True:
        user_choice = input("Do you want to (O)verwrite, (R)ename, or (S)kip? ").strip().lower()
        if user_choice == 'o':
            cursor.execute(f"DROP TABLE IF EXISTS {table_name}")
            conn.commit()
            print(f"[INFO] Table '{table_name}' overwritten.")
            return table_name
        elif user_choice == 'r':
            new_name = input("Enter new table name: ").strip()
            return new_name
        elif user_choice == 's':
            print(f"[INFO] Skipping table '{table_name}'.")
            return None
        else:
            print("Invalid input. Please enter O, R, or S.")
