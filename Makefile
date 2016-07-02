
CC=gcc
CFLAGS=-Wall -Werror -lX11
dwmstatus: dwmstatus.c
	$(CC) $(CFLAGS) -o dwmstatus dwmstatus.c

install:
	cp dwmstatus /bin

clean:
	rm /bin/dwmstatus
