## phase 4 - secure_client.py
# This code implements a secure chat client using Python's socket library and cryptography.
# Description: It connects to a server, encrypts messages using Fernet symmetric encryption,
# and allows the user to send and receive encrypted messages.

import socket
import ssl
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
    print(f"[SECURE CONNECTION] {addr} connected.")
    while True:
        try:
            msg = conn.recv(1024)
            if msg:
                broadcast(msg, conn)
        except:
            clients.remove(conn)
            conn.close()
            break

def start_secure_server():
    context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
    context.load_cert_chain(certfile="cert.pem", keyfile="key.pem")

    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(('0.0.0.0', 5566))
    server.listen()
    print("[SECURE SERVER] Listening on port 5566...")

    while True:
        conn, addr = server.accept()
        secure_conn = context.wrap_socket(conn, server_side=True)
        clients.append(secure_conn)
        thread = threading.Thread(target=handle_client, args=(secure_conn, addr))
        thread.start()

if __name__ == "__main__":
    start_secure_server()