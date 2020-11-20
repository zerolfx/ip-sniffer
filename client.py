import socket
import os
import time

sock = socket.socket(socket.AF_NETLINK, socket.SOCK_DGRAM, socket.NETLINK_USERSOCK)

sock.bind((0, 0))

# 270 is SOL_NETLINK and 1 is NETLINK_ADD_MEMBERSHIP
sock.setsockopt(270, 1, 31)

with open("ip.log", 'ab') as f:
    while True:
        try:
            msg, _ = sock.recvfrom(1024)
            msg_data = msg[16:]
            f.write(msg[16:])
            f.write(b'\n')
        except socket.error as e:
            print(e)
            exit(-1)
