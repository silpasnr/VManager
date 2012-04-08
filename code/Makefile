all:
	gcc -g -Wall main.c ui.c handler.c -o vmanager `pkg-config --cflags --libs gtk+-2.0` -lvirt -pthread 

clean:
	rm vmanager
