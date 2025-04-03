"""
Develop an equivalent to Excel or Google Sheet application where the interaction with the application is a chat interaction.

Using elements of SQLLite, Pandas, and OpenAI

Steps:
Step 1:  Load CSV Files into SQLLite
Step 2:  Create Tables Dynamically from CSV
Step 3:  Handle Schema Conflicts
Step 4:  Simulate AI using input (the input to be schemas)
Step 5:  Add AI to generate SQL

"""

import pandas as pd
import sqlite3


# Step 1: Loading CSV
df = pd.read_csv('./data/demographics.csv')  # Make sure this file exists in your working directory

# Step 2: Connect to SQLite DB (this creates it if it doesn't exist)
conn = sqlite3.connect('./data/my_database.db')

# Step 3: Push CSV data into a new table in the database
df.to_sql('people', conn, if_exists='replace', index=False)

# Step 4: Run basic SQL queries
cursor = conn.cursor()


# ex 1: Showing all rows
print("=== All People ===")
for row in cursor.execute("SELECT * FROM people"):
    print(row)

# ex 2: Filtering people OVER 30
print("\n=== People over 30 ===")
for row in cursor.execute("SELECT * FROM people WHERE age > 30"):
    print(row)

# ex 3: Limit result
print("\n=== First 5 rows ===")
for row in cursor.execute("SELECT * FROM people LIMIT 5"):
    print(row)



# closing connection
conn.close()