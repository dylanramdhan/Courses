## phase 1 - server.py
# This code implements a simple chat server using Python's socket library.
# Description: It listens for incoming connections and allows multiple clients to connect

import socket
import threading

clients = []

def broadcast(message, conn):
    for client in clients:
        if client != conn:
            try:
                client.send(message)
            except:
                client.close()
                clients.remove(client)

def handle_client(conn, addr):
    print(f"[NEW CONNECTION] {addr} connected.")
    while True:
        try:
            msg = conn.recv(1024)
            if msg:
                broadcast(msg, conn)
        except:
            clients.remove(conn)
            conn.close()
            break

def start_server():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(('0.0.0.0', 5555))
    server.listen()
    print("[LISTENING] Server is listening on port 5555")

    while True:
        conn, addr = server.accept()
        clients.append(conn)
        thread = threading.Thread(target=handle_client, args=(conn, addr))
        thread.start()

if __name__ == "__main__":
    start_server()
