#CC = gcc -DDEBUG 
CC = gcc

INCLUDES = 
LIBS = 
FLAGS = -lm

OBJS = tree.o cddb.o ui.o data.o

.c.o:
	$(CC) $(INCLUDES) $(LIBS) -c $<


tree-test: $(OBJS)
	$(CC) -o cddb $(OBJS) $(INCLUDES) $(LIBS) $(FLAGS)

ui.o: ui.c

tree.o: tree.c

cddb.o: cddb.c

data.o: data.c

clean:
	rm -f *~ *.exe *.o *.stackdump *.core cddb

