"""
    This script contains all the functions to load a CSV file into a SQLite database
        and perform the different features for the assingment to have LLM querying on 
        any given SQL things on any give uploaded CSV file.
"""

import sqlite3
from schema_inferrer import infer_and_create_table
from csv_loader import load_csv_to_df
from validator import check_table_conflict
from llm_sql_generator import ask_llm
import os

DB_PATH = "./data/my_database.db"


# listing tables
def list_tables(conn):
    cursor = conn.cursor()
    cursor.execute("SELECT name FROM sqlite_master WHERE type='table';")
    tables = cursor.fetchall()
    if not tables:
        print("No tables found.")
    else:
        print("Tables:")
        for table in tables:
            print(f"- {table[0]}")


# running sql query
def run_sql_query(conn):
    query = input("Enter your SQL query:\n> ")
    try:
        cursor = conn.cursor()
        cursor.execute(query)
        rows = cursor.fetchall()
        print("\nResults:")
        for row in rows:
            print(row)
    except Exception as e:
        print(f"Error executing query: {e}")


# loading csv file
def load_csv_interactive(conn):
    csv_path = input("Enter CSV file path:\n> ")
    if not os.path.exists(csv_path):
        print("File not found.")
        return
    try:
        df = load_csv_to_df(csv_path)
        table_name = os.path.splitext(os.path.basename(csv_path))[0]

        action = check_table_conflict(conn, table_name)
        if action == "skip":
            return

        infer_and_create_table(conn, df, table_name, if_exists=action)
        print(f"CSV loaded into table '{table_name}' successfully.")

    except Exception as e:
        print(f"Failed to load CSV: {e}")


# llm query
def ask_llm_query(conn):
    user_question = input("Ask your question:\n> ")
    query = ask_llm(user_question, conn)
    print(f"\nGenerated SQL:\n{query}\n")
    try:
        cursor = conn.cursor()
        cursor.execute(query)
        rows = cursor.fetchall()
        print("Results:")
        for row in rows:
            print(row)
    except Exception as e:
        print(f"Error executing query: {e}")


## main menu
def main():
    print("\nWelcome to ChatSheet!")
    print("What can I help you with today?")
    conn = sqlite3.connect(DB_PATH)

    while True:
        print("\n[1] Load CSV file")
        print("[2] List tables")
        print("[3] Run SQL query")
        print("[4] Ask a question (LLM)")
        print("[5] Exit")

        choice = input("Choose an option: ")

        if choice == "1":
            load_csv_interactive(conn)
        elif choice == "2":
            list_tables(conn)
        elif choice == "3":
            run_sql_query(conn)
        elif choice == "4":
            ask_llm_query(conn)
        elif choice == "5":
            print("Goodbye!\n")
            break
        else:
            print("Sorry! Invalid option. Please try again.")

    conn.close()


if __name__ == "__main__":
    main() # main menu
