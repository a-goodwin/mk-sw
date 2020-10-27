# main compiler
CC := gcc

all: main i2cread i2cwrite


i2cread: i2cread.o
	@echo "============== Linking i2cread =============="
	$(CC) $(CFLAGS) i2cread.o -o i2cread $(LDFLAGS) -l onioni2c -l oniondebug

i2cread.o:
	@echo "============== Compiling i2cread =============="
	$(CC) -c $(CFLAGS) i2cread.c -l onioni2c -l oniondebug

i2cwrite: i2cwrite.o
	@echo "============== Linking i2cwrite =============="
	$(CC) $(CFLAGS) i2cwrite.o -o i2cwrite $(LDFLAGS) -l onioni2c -l oniondebug

i2cwrite.o:
	@echo "============== Compiling i2cwrite =============="
	$(CC) -c $(CFLAGS) i2cwrite.c -l onioni2c -l oniondebug

main: main.o i2cpacket.o queue.o rs485.o sock.o
	@echo "============== Linking main =============="
	$(CC) $(CFLAGS) main.o i2cpacket.o queue.o rs485.o sock.o -o main $(LDFLAGS) -l ugpio -l onioni2c -l oniondebug

main.o: 
	@echo "============== Compiling main =============="
	$(CC) -c $(CFLAGS) main.c

sock.o:
	@echo "============== Compiling sock module =============="
	$(CC) -c $(CFLAGS) sock.c

rs485.o:
	@echo "============== Compiling rs485usb =============="
	$(CC) -c $(CFLAGS) rs485test/rs485.c

i2cpacket.o: 
	@echo "============== Compiling i2c packet module =============="
	$(CC) -c $(CFLAGS) i2cpacket.c -l onioni2c -l oniondebug

queue.o: 
	@echo "============== Compiling queue module =============="
	$(CC) -c $(CFLAGS) queue.c

clean:
	@rm -rf *.o main
