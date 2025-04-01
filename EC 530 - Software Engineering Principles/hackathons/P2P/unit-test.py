## Unit Tests for P2P Chat Application
# This script tests the functionality of a P2P chat application.

# Description: It includes tests for basic chat functionality, registration, sending messages,
#       subscription, and authentication. The tests are designed to ensure that the 
#       application behaves as expected and handles various scenarios correctly.

import pytest
import socket
import threading
import time
import requests

# Configurations
SERVER_HOST = "127.0.0.1"
SERVER_PORT = 5555
API_URL = "http://127.0.0.1:8000"

# Helper to start a basic client
def start_test_client(send_msgs=None, received_msgs=None):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((SERVER_HOST, SERVER_PORT))

    def recv():
        while True:
            try:
                msg = sock.recv(1024).decode('utf-8')
                if received_msgs is not None:
                    received_msgs.append(msg)
            except:
                break

    threading.Thread(target=recv, daemon=True).start()

    if send_msgs:
        for msg in send_msgs:
            sock.send(msg.encode('utf-8'))
            time.sleep(0.1)

    return sock

def test_phase1_basic_chat():
    received = []
    client1 = start_test_client(send_msgs=["Hello from client1"], received_msgs=received)
    client2 = start_test_client(received_msgs=received)
    time.sleep(1)
    assert any("Hello from client1" in msg for msg in received)
    client1.close()
    client2.close()

def test_phase2_register_and_send():
    r = requests.post(f"{API_URL}/register")
    assert r.status_code == 200
    assert "registered" in r.json().get("message", "")

    # Try sending to self
    r2 = requests.post(f"{API_URL}/send", json={"to": "127.0.0.1", "content": "API test message"})
    assert r2.status_code == 200

def test_phase3_subscription():
    r = requests.post(f"{API_URL}/subscribe")
    assert r.status_code == 200

    r2 = requests.post(f"{API_URL}/notify_subscribers", json={"to": "127.0.0.1", "content": "Important update"})
    assert r2.status_code == 200
    assert "notified" in r2.json().get("status", "")

def test_phase4_auth():
    headers = {"Authorization": "Bearer SECRET123"}
    r = requests.post(f"{API_URL}/subscribe", headers=headers)
    assert r.status_code == 200

    r2 = requests.post(f"{API_URL}/notify_subscribers", json={"to": "127.0.0.1", "content": "Secure update"}, headers=headers)
    assert r2.status_code == 200

def test_phase4_auth_fail():
    headers = {"Authorization": "Bearer WRONG"}
    r = requests.post(f"{API_URL}/subscribe", headers=headers)
    assert r.status_code == 401
    assert "error" in r.json()
