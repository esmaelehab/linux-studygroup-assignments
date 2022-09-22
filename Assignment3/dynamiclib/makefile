CC = gcc
OBJS = myfact.o myrand.o myfib.o

libA3command.so: $(OBJS)
	$(CC) -shared $^ -o $@

%.o: %.c
	$(CC) -c -fPIC $<
