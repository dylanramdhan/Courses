"""
    This file contains a function to dynamically generate a SQLite CREATE TABLE statement from a CSV's inferred schema.
"""

import pandas as pd
import sqlite3

def infer_sqlite_schema(df: pd.DataFrame, table_name: str) -> str:
    """
    Generate a SQLite CREATE TABLE statement by inferring schema from the DataFrame.
    """
    dtype_mapping = {
        'object': 'TEXT',
        'int64': 'INTEGER',
        'float64': 'REAL',
        'bool': 'INTEGER',
        'datetime64[ns]': 'TEXT'
    }

    columns = []
    for col in df.columns:
        dtype = str(df[col].dtype)
        sql_type = dtype_mapping.get(dtype, 'TEXT')
        columns.append(f"{col} {sql_type}")

    columns_str = ",\n    ".join(columns)
    create_stmt = f"""
    CREATE TABLE IF NOT EXISTS {table_name} (
        {columns_str}
    );
    """
    return create_stmt.strip()


def infer_and_create_table(conn: sqlite3.Connection, df: pd.DataFrame, table_name: str, if_exists: str = 'replace') -> None:
    """
    Infer the schema and execute a CREATE TABLE statement using the connection provided.
    Then, insert the data from the DataFrame into the created table.
    """
    create_stmt = infer_sqlite_schema(df, table_name)
    print(f"[INFO] Executing SQL:\n{create_stmt}\n")
    cursor = conn.cursor()
    cursor.execute(f"DROP TABLE IF EXISTS {table_name}") if if_exists == 'replace' else None
    cursor.execute(create_stmt)
    conn.commit()

    # Insert data from DataFrame into table
    df.to_sql(table_name, conn, if_exists='append', index=False)
