# === backend/app.py ===
from flask import Flask
from routes.upload import upload_blueprint
from routes.query import query_blueprint
from routes.grades import grades_blueprint

app = Flask(__name__)

# Register blueprints
app.register_blueprint(upload_blueprint)
app.register_blueprint(query_blueprint)
app.register_blueprint(grades_blueprint)

@app.route("/")
def home():
    return {"message": "Professor Assistant API is running!"}

if __name__ == "__main__":
    app.run(debug=True)
