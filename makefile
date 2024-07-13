CC = gcc
CFLAGS = -Wall -Werror -fpic
CCLINK = $(CC)
LDFLAGS = -shared -L.
LIBS = -llinked-list 

all: libgrades.so

libgrades.so: grades.o
	$(CCLINK) $(LDFLAGS) -o $@ $^ $(LIBS)

grades.o: grades.c
	$(CC) $(CFLAGS) -c grades.c

clean:
	rm -f *.o libgrades.so