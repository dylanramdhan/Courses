"""
    llm_sql_generator.py
    This module takes a natural language question and database schema,
    then uses OpenAI's API to return an SQL query.
"""

import os
import openai

# Ensure this is set in your environment: export OPENAI_API_KEY="your-api-key"
openai.api_key = os.getenv("OPENAI_API_KEY")

def ask_llm(schema_str: str, user_question: str) -> str:
    """
    Generates a SQL query from a natural language question and database schema.
    """
    prompt = f"""
You are an AI assistant tasked with converting user queries into SQL statements.
The database uses SQLite and contains the following schema:

{schema_str}

User Query: "{user_question}"

Your task is to:
1. Generate a SQL query that accurately answers the user's question.
2. Ensure the SQL is compatible with SQLite syntax.
3. Provide a short comment explaining what the query does.

Output Format:
- SQL Query
- Explanation
"""

    try:
        response = openai.ChatCompletion.create(
            model="gpt-3.5-turbo",
            messages=[
                {"role": "system", "content": "You are a helpful assistant for SQL generation."},
                {"role": "user", "content": prompt}
            ],
            temperature=0.2,
            max_tokens=300,
        )

        return response['choices'][0]['message']['content'].strip()

    except Exception as e:
        return f"[ERROR] Failed to generate SQL: {e}"
