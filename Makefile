
CC=gcc
CFLAGS=-Wall -Werror -lX11 -lasound
dwmstatus: dwmstatus.c
	$(CC) $(CFLAGS) -o dwmstatus dwmstatus.c

install:
	cp dwmstatus /bin

clean:
	rm dwmstatus
	rm /bin/dwmstatus
