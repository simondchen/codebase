import socket
s=socket.socket()
host=socket.gethostname()
port=7777
s.connect((host,port))
print s.recv(1024)
