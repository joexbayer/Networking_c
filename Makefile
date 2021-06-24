VFLAGS = --track-origins=yes --leak-check=full --show-leak-kinds=all
CFLAGS = -std=gnu11 -g -Wall -Wextra

all: tunc

tunc: tun.c utils.c ipv4.c icmp.c
	gcc tun.c utils.c ipv4.c icmp.c $(CFLAGS) -o server && rm -r server.dSYM && sudo ./server

tunpy: tun.py
	sudo python3 tun.py && sudo ifconfig tun12 inet 10.0.0.1 10.0.0.2 up
