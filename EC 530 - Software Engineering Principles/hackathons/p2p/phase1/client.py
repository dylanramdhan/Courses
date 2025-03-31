## phase 1 - client.py
# This code implements a simple chat client using Python's socket library.
# Description: It connects to a server and allows the user to send and receive messages.

import socket
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

def start_client():
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect(('127.0.0.1', 5555))

    threading.Thread(target=receive_messages, args=(client,), daemon=True).start()

    print("Connected to chat. Type messages below:")
    while True:
        msg = input()
        if msg.lower() == 'exit':
            client.close()
            break
        client.send(msg.encode('utf-8'))

if __name__ == "__main__":
    start_client()
