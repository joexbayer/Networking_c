# Networking code for macOS with Tun/Tap

## Using TAP as a network interface to read and write packets.

### UDP
![alt text](https://github.com/joexbayer/Networking_c/blob/main/pictures/udp.png?raw=true)

### ARP
![alt text](https://github.com/joexbayer/Networking_c/blob/main/pictures/arp.png?raw=true)

![alt text](https://github.com/joexbayer/Networking_c/blob/main/pictures/arp_table.png?raw=true)

### ICMP
![alt text](https://github.com/joexbayer/Networking_c/blob/main/pictures/icmp_cli.png?raw=true)

![alt text](https://github.com/joexbayer/Networking_c/blob/main/pictures/icmp_wireshark.png?raw=true)

Currently working:

<p>Parsing Ethernett Packets.</p>
<p>Accepting and parsing IPv4 packets.</p>
<p>Calculate checksum based on <a href="https://datatracker.ietf.org/doc/html/rfc1071#section-4.1">RFC 1071 #4.1.</a></p>
<p>Using a sk_buff.</p>
<p>Configurable net device with IP and MAC.</p>
<p>Parsing ICMP and responding ping.</p>
<p>Printing UDP messages from netcat.</p>
<p>Parsing ARP, sending ARP reply and saving ARP cache</p>


For TAP on MacOS: <a href="http://tuntaposx.sourceforge.net/">TunTap</a>

Inspired by: 
<a href="https://github.com/torvalds/linux/tree/master/net">Linux Network Stack</a> and 
<a href="https://people.cs.clemson.edu/~westall/853/notes/netinit.pdf">Networking History</a>
