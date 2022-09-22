CC = gcc
OBJS = myrand.o myfact.o myfib.o

libA3command.a: $(OBJS)
	ar -rs $@ $^

%.o: %.c
	$(CC) -c $<
