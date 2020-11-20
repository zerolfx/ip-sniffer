import socket
import os
import time

sock = socket.socket(socket.AF_NETLINK, socket.SOCK_DGRAM, socket.NETLINK_USERSOCK)

sock.bind((0, 0))

# 270 is SOL_NETLINK and 1 is NETLINK_ADD_MEMBERSHIP
sock.setsockopt(270, 1, 31)

while True:
    try:
        msg, _ = sock.recvfrom(1024)
        print(msg[16:].decode('utf8'))
    except socket.error as e:
        print(e)
