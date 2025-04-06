"""
    This file will take a user's question and send it along with the schema to OpenAI,
        and should return a SQL statement that answers the user's intial question.
        
        Resources: https://github.com/openai/openai-python
"""

import openai
import os

openai.api_key = os.getenv("OPENAI_API_KEY")

def generate_sql_from_question(schema_str, user_question):
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

        reply = response['choices'][0]['message']['content']
        return reply.strip()
    except Exception as e:
        return f"[ERROR] Failed to generate SQL: {e}"
