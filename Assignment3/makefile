CC = gcc 
STATIC = NO
DYNAMIC = NO
OBJS = Assignment3.o 
LIB_NAME = A3command

ifeq ($(STATIC), YES)

LIB = ./staticlib/libA3command.a
PATH  = ./staticlib/
EXE = A3staticexe
HEADER = ./staticlib/mycommand.h

else

LIB = ./dynamiclib/libA3command.so
PATH = ./dynamiclib/
EXE = A3dynamicexe
HEADER = ./dynamiclib/mycommand.h

endif

all: A3exe

A3exe: $(OBJS) $(LIB)
	$(CC) -o $(EXE) $< -l$(LIB_NAME) -L$(PATH)

%.o: %.c $(HEADER)
	$(MAKE) -C $(PATH)
	$(CC) -c $< -I$(PATH)

clean:
	rm -f *.o
	rm -f $(PATH)/*.o
	rm -f $(PATH)/*.a
	rm -f $(PATH)/*.so
	rm -f $(EXE)
