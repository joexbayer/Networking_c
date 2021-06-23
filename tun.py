import os, sys
from select import select
from scapy.all import IP

f = os.open("/dev/tun12", os.O_RDWR)
while 1:
    r = select([f],[],[])[0][0]
    if r == f:
        packet = os.read(f, 4000)
        # print len(packet), packet
        ip = IP(packet)
        ip.show()
