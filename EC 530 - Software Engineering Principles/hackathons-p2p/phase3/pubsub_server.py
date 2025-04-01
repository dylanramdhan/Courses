## phase 3 - pubsub_server.py
# This code implements a simple publish-subscribe server using Python's socket library.
# Description: It allows clients to subscribe to topics and publish messages to those topics.

from fastapi import FastAPI, Request
from pydantic import BaseModel
import socket

app = FastAPI()

class Message(BaseModel):
    topic: str
    content: str

# In-memory structures
peers = {}
subscriptions = {}

@app.post("/register")
async def register_peer(req: Request):
    peer_ip = req.client.host
    peers[peer_ip] = True
    return {"message": f"Peer {peer_ip} registered."}

@app.post("/subscribe")
async def subscribe(req: Request, topic: str):
    peer_ip = req.client.host
    if topic not in subscriptions:
        subscriptions[topic] = set()
    subscriptions[topic].add(peer_ip)
    return {"message": f"Peer {peer_ip} subscribed to {topic}."}

@app.post("/publish")
async def publish_message(msg: Message):
    if msg.topic not in subscriptions:
        return {"status": "failed", "reason": "No subscribers for this topic."}
    failures = []
    for subscriber in subscriptions[msg.topic]:
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.connect((subscriber, 5555))
                s.send(f"[{msg.topic}] {msg.content}".encode('utf-8'))
        except Exception as e:
            failures.append({"subscriber": subscriber, "error": str(e)})
    return {"status": "published", "failures": failures}
