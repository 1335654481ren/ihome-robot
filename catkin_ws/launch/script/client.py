# -*- coding: UTF-8 -*-
import socket  

HOST='127.0.0.1'  
PORT=10001 
BUFFER=4096  
sock=socket.socket(socket.AF_INET,socket.SOCK_STREAM)  
sock.connect((HOST,PORT))  
sock.send("任晓亮")  
recv=sock.recv(BUFFER)  
print('[tcpServer said]: %s' % recv)  
sock.close() 