#CFLAGS:=-I./
select_serv:select_serv.c
	gcc -o select_serv select_serv.c -lpthread -I./
client:client.c
	gcc -o client client.c -I./
all:select_serv client

.DEFAULT_GOAL:=all

clean:
	rm select_serv client
	find ./ -type s -exec rm {} \;
