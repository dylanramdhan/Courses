## phase 2 - peer_client.py
# This code implements a simple peer-to-peer client using Python's socket library.
# Description: It connects to a peer and allows the user to send and receive messages.

import requests
import socket
import threading

def receive_messages(sock):
    while True:
        try:
            msg = sock.recv(1024).decode('utf-8')
            print(f"\nReceived: {msg}")
        except:
            print("[ERROR] Connection lost.")
            sock.close()
            break

def start_client(api_server: str, listen_port: int):
    # Register with the API server
    response = requests.post(f"http://{api_server}/register")
    print(response.json()["message"])

    # Start listening for incoming messages
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(('0.0.0.0', listen_port))
    server.listen()
    print(f"Listening for incoming messages on port {listen_port}...")

    threading.Thread(target=accept_connections, args=(server,), daemon=True).start()

    # Send messages to peers
    while True:
        peer_ip = input("Enter peer IP to send message: ")
        message = input("Enter message: ")
        response = requests.post(f"http://{api_server}/send", json={"to": peer_ip, "content": message})
        print(response.json()["status"])

def accept_connections(server):
    while True:
        conn, addr = server.accept()
        threading.Thread(target=handle_client, args=(conn, addr), daemon=True).start()

def handle_client(conn, addr):
    print(f"Connected by {addr}")
    while True:
        try:
            msg = conn.recv(1024).decode('utf-8')
            print(f"\nMessage from {addr}: {msg}")
        except:
            print(f"Connection with {addr} lost.")
            conn.close()
            break

if __name__ == "__main__":
    api_server = input("Enter API server address (e.g., 127.0.0.1:8000): ")
    listen_port = int(input("Enter port to listen for incoming messages: "))
    start_client(api_server, listen_port)
