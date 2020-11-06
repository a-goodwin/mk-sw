# main compiler
CC := gcc

all: main i2cread i2cwrite


i2cread: i2cread.o
	@echo "\r\n============== Linking i2cread =============="
	$(CC) $(CFLAGS) i2cread.o -o i2cread $(LDFLAGS) -l onioni2c -l oniondebug

i2cread.o:
	@echo "\r\n============== Compiling i2cread.c =============="
	$(CC) -c $(CFLAGS) i2cread.c -l onioni2c -l oniondebug

i2cwrite: i2cwrite.o
	@echo "\r\n============== Linking i2cwrite =============="
	$(CC) $(CFLAGS) i2cwrite.o -o i2cwrite $(LDFLAGS) -l onioni2c -l oniondebug

i2cwrite.o:
	@echo "\r\n============== Compiling i2cwrite.c =============="
	$(CC) -c $(CFLAGS) i2cwrite.c -l onioni2c -l oniondebug

main: main.o i2cpacket.o queue.o rs485.o sock.o cmdpacket.o
	@echo "\r\n============== Linking main =============="
	$(CC) $(CFLAGS) main.o cmdpacket.o i2cpacket.o queue.o rs485.o sock.o -o main $(LDFLAGS) -l ugpio -l onioni2c -l oniondebug

main.o: 
	@echo "\r\n============== Compiling main.c =============="
	$(CC) -c $(CFLAGS) main.c

cmdpacket.o:
	@echo "\r\n============== Compiling cmdpacket.c =============="
	$(CC) -c $(CFLAGS) cmdpacket.c

sock.o:
	@echo "\r\n============== Compiling sock.c =============="
	$(CC) -c $(CFLAGS) sock.c

rs485.o:
	@echo "\r\n============== Compiling rs485.c =============="
	$(CC) -c $(CFLAGS) rs485test/rs485.c

i2cpacket.o: 
	@echo "\r\n============== Compiling i2cpacket.c =============="
	$(CC) -c $(CFLAGS) i2cpacket.c -l onioni2c -l oniondebug

queue.o: 
	@echo "\r\n============== Compiling queue.c =============="
	$(CC) -c $(CFLAGS) queue.c

clean:
	@echo "\r\n============== CLEAN ALL =============="
	@rm -rf *.o main i2cread i2cwrite
