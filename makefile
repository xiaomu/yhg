#/*****************************************************************************
#*  Copyright          :  2012 All Rights Reserved.
#*
#*  Date               :  2012-04-30 21:49:25
#*  Author             :  Dengzhaoqun
# *****************************************************************************/
all:client con_mng con_server game_mng game_server db_server

CC = gcc
INCLUDE = .
CFLAGS = -g -Wall

MYLIB = yhg.a

client: client_entry.o $(MYLIB)
	$(CC) -o client client_entry.o $(MYLIB)
con_mng: con_mng_entry.o $(MYLIB)
	$(CC) -o con_mng con_mng_entry.o $(MYLIB)
con_server: con_server_entry.o $(MYLIB)
	$(CC) -o con_server con_server_entry.o $(MYLIB)
game_mng: game_mng_entry.o $(MYLIB)
	$(CC) -o game_mng game_mng_entry.o $(MYLIB)
game_server: game_server_entry.o $(MYLIB)
	$(CC) -o game_server game_server_entry.o $(MYLIB)
db_server: db_server_entry.o $(MYLIB)
	$(CC) -o db_server db_server_entry.o $(MYLIB)

$(MYLIB): $(MYLIB)(cmd.o) $(MYLIB)(cmd_handler.o) $(MYLIB)(error.o) $(MYLIB)(log.o) $(MYLIB)(utils.o) $(MYLIB)(client.o) $(MYLIB)(con_mng.o) $(MYLIB)(con_server.o) $(MYLIB)(game_mng.o) $(MYLIB)(game_server.o) $(MYLIB)(db_server.o) 

client.o: client.c con_server.h log.h error.h cmd.h utils.h defines.h client.h
cmd.o: cmd.c cmd.h cmd_handler.h log.h
cmd_handler.o: cmd_handler.c cmd_handler.h con_mng.h cmd.h log.h
con_mng.o: con_mng.c con_mng.h error.h log.h cmd.h utils.h defines.h cmd_handler.h
con_server.o: con_server.c con_server.h log.h error.h cmd.h utils.h  defines.h
db_server.o: db_server.c con_server.h log.h error.h cmd.h utils.h defines.h db_server.h
error.o: error.c error.h
game_mng.o: game_mng.c con_server.h log.h error.h cmd.h utils.h defines.h
game_server.o: game_server.c con_server.h log.h error.h cmd.h utils.h defines.h game_server.h
log.o: log.c utils.h defines.h error.h
utils.o: utils.c utils.h defines.h
client_entry.o: client_entry.c con_server.h log.h error.h cmd.h utils.h defines.h client.h cmd_handler.h
con_mng_entry.o: con_mng_entry.c con_mng.h error.h log.h cmd.h utils.h defines.h cmd_handler.h
con_server_entry.o: con_server_entry.c con_server.h log.h error.h cmd.h  utils.h defines.h
db_server_entry.o: db_server_entry.c
game_mng_entry.o: game_mng_entry.c con_server.h log.h error.h cmd.h utils.h defines.h
game_server_entry.o: game_server_entry.c con_server.h log.h error.h cmd.h  utils.h defines.h game_server.h

clean:
	rm -f *.o

