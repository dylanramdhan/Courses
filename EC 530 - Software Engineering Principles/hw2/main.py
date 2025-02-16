"""
    Assignment #2, due date: 2/16/2025
    Engineer: Dylan Ramdhan 
     
    Assignment:
        Design APIs for all entities:
            ⇢ User
            ⇢ House
            ⇢ Rooms
            ⇢ Devices
        
        Develop Stub builds for the APIs
        Develop unit tests for the stub functions
        Utilize GitHub actions and automate the cycle
        Add error checking into APIs
"""

from flask import Flask, request, jsonify

app = Flask(__name__)

users = {}
houses = {}
rooms = {}
devices = {}

@app.route("/")
def home():
    return jsonify({"message": "Welcome to the API!"})



# user endpoints
@app.route("/users/register", methods=["POST"])
def register_user():
    data = request.json

    # checking if required fields are present
    if not data or "name" not in data or "email" not in data:
        return jsonify({"error": "Missing 'name' or 'email' field"}), 400
    
    # checking valid email format
    if "@" not in data["email"]:
        return jsonify({"error": "Invalid email format"}), 400
    
    user_id = str(len(users) + 1)
    users[user_id] = {
        "user_id": user_id,
        "name": data["name"],
        "email": data["email"],
        "password": "hashed_password"
    }
    
    return jsonify({"message": "User registered", "user_id": user_id}), 201


@app.route("/users/<user_id>", methods=["GET"])
def get_user(user_id):
    if user_id not in users:
        return jsonify({"error": "User not found"}), 404
    return jsonify(users[user_id])




# house endpoints
@app.route("/houses", methods=["POST"])
def create_house():
    data = request.json

    if not data or "owner_id" not in data or "address" not in data:
        return jsonify({"error": "Missing 'owner_id' or 'address' field"}), 400
    
    if data["owner_id"] not in users:
        return jsonify({"error": "Owner ID does not exist"}), 400

    house_id = str(len(houses) + 1)
    houses[house_id] = {
        "house_id": house_id,
        "owner_id": data["owner_id"],
        "address": data["address"],
        "rooms": []
    }
    
    return jsonify({"message": "House created", "house_id": house_id}), 201


@app.route("/houses/<house_id>", methods=["GET"])
def get_house(house_id):
    if house_id not in houses:
        return jsonify({"error": "House not found"}), 404
    return jsonify(houses[house_id])





# room endpoints
@app.route("/houses/<house_id>/rooms", methods=["POST"])
def add_room(house_id):
    if house_id not in houses:
        return jsonify({"error": "House not found"}), 404
    
    data = request.json

    if not data or "name" not in data:
        return jsonify({"error": "Missing 'name' field"}), 400

    room_id = str(len(rooms) + 1)
    rooms[room_id] = {
        "room_id": room_id,
        "house_id": house_id,
        "name": data["name"],
        "devices": []
    }
    
    houses[house_id]["rooms"].append(room_id)
    
    return jsonify({"message": "Room added", "room_id": room_id}), 201


@app.route("/rooms/<room_id>", methods=["GET"])
def get_room(room_id):
    if room_id not in rooms:
        return jsonify({"error": "Room not found"}), 404
    return jsonify(rooms[room_id])




# device endpoints
@app.route("/rooms/<room_id>/devices", methods=["POST"])
def add_device(room_id):
    if room_id not in rooms:
        return jsonify({"error": "Room not found"}), 404
    
    data = request.json

    if not data or "type" not in data:
        return jsonify({"error": "Missing 'type' field"}), 400

    device_id = str(len(devices) + 1)
    devices[device_id] = {
        "device_id": device_id,
        "room_id": room_id,
        "type": data["type"],
        "status": "OFF",
        "settings": data.get("settings", {})
    }
    
    rooms[room_id]["devices"].append(device_id)
    
    return jsonify({"message": "Device added", "device_id": device_id}), 201


@app.route("/devices/<device_id>", methods=["GET"])
def get_device(device_id):
    if device_id not in devices:
        return jsonify({"error": "Device not found"}), 404
    return jsonify(devices[device_id])




# running the app
if __name__ == "__main__":
    app.run(debug=True)
