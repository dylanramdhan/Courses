import unittest
import json
from app import app

class APITest(unittest.TestCase):
    def setUp(self):
        self.app = app.test_client()
    
    def test_register_user(self):
        response = self.app.post("/users/register", json={
            "name": "John Doe",
            "email": "john@example.com"
        })
        self.assertEqual(response.status_code, 201)

    def test_create_house(self):
        response = self.app.post("/houses", json={
            "owner_id": "1",
            "address": "123 Main St"
        })
        self.assertEqual(response.status_code, 201)

if __name__ == "__main__":
    unittest.main()
