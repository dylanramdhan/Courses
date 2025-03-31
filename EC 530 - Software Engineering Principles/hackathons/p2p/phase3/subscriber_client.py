# phase 3 - subscriber_client.py
# This code implements a simple subscriber client using Python's socket library.
# Description: It connects to a publish-subscribe server and subscribes to topics.

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
            print(f"[DISCONNECTED] {addr}")
            conn.close()
            break

def start_subscriber(api_server: str, listen_port: int, topic: str):
    # Register with the API server
    response = requests.post(f"http://{api_server}/register")
    print(response.json().get("message", "[ERROR] Could not register."))

    # Subscribe to a topic
    response = requests.post(f"http://{api_server}/subscribe?topic={topic}")
    print(response.json().get("message", "[ERROR] Could not subscribe."))

    # Start listening for incoming messages
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(('0.0.0.0', listen_port))
    server.listen()
    print(f"Listening for messages on topic '{topic}' at port {listen_port}...")

    threading.Thread(target=accept_connections, args=(server,), daemon=True).start()

    # Keep the subscriber running
    try:
        while True:
            pass
    except KeyboardInterrupt:
        print("[SHUTDOWN] Subscriber stopping...")

if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(description="Start a subscriber client.")
    parser.add_argument('--api', type=str, required=True, help="API server address (e.g. 127.0.0.1:8000)")
    parser.add_argument('--port', type=int, default=6000, help="Port to listen for messages")
    parser.add_argument('--topic', type=str, default="default", help="Topic to subscribe to")

    args = parser.parse_args()
    start_subscriber(args.api, args.port, args.topic)
