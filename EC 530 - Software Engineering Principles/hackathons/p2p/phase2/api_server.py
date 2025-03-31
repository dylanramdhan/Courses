# phase 2 - api_server.py
# This code implements a simple API server using FastAPI.
# Description: It allows peers to register and send messages to each other.

from fastapi import FastAPI, Request
from pydantic import BaseModel
import socket

app = FastAPI()

class Message(BaseModel):
    to: str
    content: str

# In-memory peer directory
peers = {}

@app.post("/register")
async def register_peer(req: Request):
    peer_ip = req.client.host
    peers[peer_ip] = True
    return {"message": f"Peer {peer_ip} registered."}

@app.post("/send")
async def send_message(msg: Message):
    if msg.to not in peers:
        return {"status": "failed", "reason": "Peer not registered."}
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((msg.to, 5555))
            s.send(msg.content.encode('utf-8'))
        return {"status": "sent"}
    except Exception as e:
        return {"status": "failed", "reason": str(e)}
