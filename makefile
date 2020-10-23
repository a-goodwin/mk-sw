# main compiler
CC := gcc

TARGET1 := main
TARGET2 := i2cpacket
TARGET3 := queue

all: main i2cread

i2cread: i2cread.o i2cpacket.o
	@echo linking i2cread
	$(CC) $(CFLAGS) i2cread.o i2cpacket.o -o i2cread $(LDFLAGS) -l onioni2c -l oniondebug

i2cread.o:
	@echo "compiling i2cread"
	$(CC) -c $(CFLAGS) i2cread.c

main: main.o i2cpacket.o queue.o
	@echo "Linking main"
	$(CC) $(CFLAGS) main.o i2cpacket.o queue.o -o main $(LDFLAGS) -l ugpio -l onioni2c -l oniondebug

main.o: 
	@echo "Compiling main"
	$(CC) -c $(CFLAGS) main.c

i2cpacket.o: 
	@echo "Compiling i2c packet module"
	$(CC) -c $(CFLAGS) i2cpacket.c -l onioni2c -l oniondebug

queue.o: 
	@echo "Compiling queue module"
	$(CC) -c $(CFLAGS) queue.c
#	$(CC) $(CFLAGS) $(TARGET3).c -o $(TARGET3) $(LDFLAGS) -l ugpio

clean:
	@rm -rf *.o main
