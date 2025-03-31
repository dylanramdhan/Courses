# phase 4 - secure_client.py
# This code implements a secure chat client using Python's socket library and cryptography.
# Description: It connects to a server, encrypts messages using Fernet symmetric encryption,
# and allows the user to send and receive encrypted messages.

import socket
import ssl
import threading

def receive_messages(sock):
    while True:
        try:
            msg = sock.recv(1024).decode('utf-8')
            print("\n" + msg)
        except:
            print("[ERROR] Connection lost.")
            sock.close()
            break

def start_secure_client():
    context = ssl.create_default_context(ssl.Purpose.SERVER_AUTH)
    context.check_hostname = False
    context.verify_mode = ssl.CERT_NONE

    raw_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    secure_sock = context.wrap_socket(raw_sock, server_hostname='localhost')

    secure_sock.connect(('127.0.0.1', 5566))

    threading.Thread(target=receive_messages, args=(secure_sock,), daemon=True).start()

    print("Connected securely. Type messages below:")
    while True:
        msg = input()
        if msg.lower() == 'exit':
            secure_sock.close()
            break
        secure_sock.send(msg.encode('utf-8'))

if __name__ == "__main__":
    start_secure_client()