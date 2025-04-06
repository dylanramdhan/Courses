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

def check_and_handle_conflict(conn: sqlite3.Connection, table_name: str) -> str:
    cursor = conn.cursor()
    cursor.execute("SELECT name FROM sqlite_master WHERE type='table' AND name=?", (table_name,))
    exists = cursor.fetchone()

    if exists:
        action = prompt_on_conflict(table_name)
        if action == "overwrite":
            cursor.execute(f"DROP TABLE IF EXISTS {table_name}")
            conn.commit()
            return table_name
        elif action == "rename":
            new_name = input("Enter new table name: ").strip()
            return new_name
        elif action == "skip":
            return None
    return table_name
