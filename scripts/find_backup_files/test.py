import httplib

conn = httplib.HTTPConnection("www.turundustugi.ee")
conn.request("GET", "/")
r1 = conn.getresponse()
print r1.status
