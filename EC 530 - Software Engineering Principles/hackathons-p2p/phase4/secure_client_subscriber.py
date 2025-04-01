## TLS-Compatible Secure Client

import socket
import ssl

def start_secure_client(server_ip: str, port: int):
    context = ssl.create_default_context()

    with socket.create_connection((server_ip, port)) as sock:
        with context.wrap_socket(sock, server_hostname=server_ip) as ssock:
            print("[SECURE] Connected to server.")
            while True:
                msg = input("Enter message (or 'exit'): ")
                if msg.lower() == 'exit':
                    break
                ssock.send(msg.encode('utf-8'))

# TLS-Compatible Secure Subscriber
import threading

def secure_handle_client(connstream, addr):
    print(f"[SECURE CONNECTED] {addr}")
    try:
        while True:
            data = connstream.recv(1024)
            if not data:
                break
            print(f"[SECURE MESSAGE] {addr}: {data.decode('utf-8')}")
    except:
        print(f"[ERROR] Lost connection to {addr}")
    finally:
        connstream.close()

def start_secure_subscriber(listen_port: int, certfile: str, keyfile: str):
    context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
    context.load_cert_chain(certfile=certfile, keyfile=keyfile)

    bindsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    bindsocket.bind(('0.0.0.0', listen_port))
    bindsocket.listen(5)
    print(f"[SECURE SUBSCRIBER] Listening on port {listen_port}...")

    while True:
        newsocket, fromaddr = bindsocket.accept()
        connstream = context.wrap_socket(newsocket, server_side=True)
        threading.Thread(target=secure_handle_client, args=(connstream, fromaddr), daemon=True).start()
