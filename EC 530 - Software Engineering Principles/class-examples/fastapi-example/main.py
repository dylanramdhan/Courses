# Creating a branches
# Creating tests for the REST APIs
# Requirement file

from fastapi import FastAPI, HTTPException
from typing import List
from models import Book

app = FastAPI()

# In-memory "database"
books_db = []

# Get all books
@app.get("/books", response_model=List[Book])
def get_books():
    return books_db

# Get a book by ID
@app.get("/books/{book_id}", response_model=Book)
def get_book(book_id: int):
    for book in books_db:
        if book.id == book_id:
            return book
    raise HTTPException(status_code=404, detail="Book not found")

# Add a new book
@app.post("/books", response_model=Book)
def add_book(book: Book):
    book.id = len(books_db) + 1
    books_db.append(book)
    return book

# Update an existing book
@app.put("/books/{book_id}", response_model=Book)
def update_book(book_id: int, updated_book: Book):
    for index, book in enumerate(books_db):
        if book.id == book_id:
            books_db[index] = updated_book
            updated_book.id = book_id
            return updated_book
    raise HTTPException(status_code=404, detail="Book not found")

# Delete a book
@app.delete("/books/{book_id}")
def delete_book(book_id: int):
    for index, book in enumerate(books_db):
        if book.id == book_id:
            books_db.pop(index)
            return {"message": "Book deleted successfully"}
    raise HTTPException(status_code=404, detail="Book not found")
