# Networking code for macOS with Tun/Tap

## Using TAP as a network interface to read and write packets.

Currently working:

<p>Parsing Ethernett Packets.</p>
<p>Accepting and parsing IPv4 packets.</p>
<p>Caculate checksum based on <a href="https://datatracker.ietf.org/doc/html/rfc1071#section-4.1">RFC 1071 #4.1.</a></p>
<p>Using a sk_buff.</p>
<p>Configurable net device with IP and MAC.</p>
<p>Parsing ICMP with ping.</p>

![alt text](https://github.com/joexbayer/Networking_c/blob/main/pictures/icmp_cli.png?raw=true)

![alt text](https://github.com/joexbayer/Networking_c/blob/main/pictures/icmp_wireshark.png?raw=true)


For TAP on MacOS: <a href="http://tuntaposx.sourceforge.net/">TunTap</a>

Inspired by: <a href="https://www.saminiir.com/lets-code-tcp-ip-stack-1-ethernet-arp/">Let's code a TCP/IP stack, 1: Ethernet & ARP</a>
