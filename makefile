SOURCES = main.c display.c helper.c bot.c
OBJECTS = $(SOURCES:.c=.o)
CC = gcc
CFLAGS = -Wall -c 
OUTPUT = app

$(OUTPUT): $(OBJECTS)
	$(CC) -o $@ $^

main.o: main.c config.h display.h helper.h
	$(CC) $(CFLAGS) $<

display.o: display.c config.h display.h
	$(CC) $(CFLAGS) $<

helper.o: helper.c config.h helper.h
	$(CC) $(CFLAGS) $<

bot.o: bot.c bot.h helper.h
	$(CC) $(CFLAGS) $<

.PHONY: clean
clean:
	@rm -rf *.o $(OUTPUT)
run: $(OUTPUT)
	./$(OUTPUT)