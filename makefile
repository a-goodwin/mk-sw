# main compiler
CC := g++

all: KT-CTRL I2CREAD I2CWRITE KPN-CTRL


I2CREAD: i2cread.o
	@echo "\r\n============== Linking i2cread =============="
	$(CC) $(CFLAGS) i2cread.o -o i2cread $(LDFLAGS) -l onioni2c -l oniondebug

i2cread.o:
	@echo "\r\n============== Compiling i2cread.c =============="
	$(CC) -c $(CFLAGS) i2cread.c -l onioni2c -l oniondebug

I2CWRITE: i2cwrite.o
	@echo "\r\n============== Linking i2cwrite =============="
	$(CC) $(CFLAGS) i2cwrite.o -o i2cwrite $(LDFLAGS) -l onioni2c -l oniondebug

i2cwrite.o:
	@echo "\r\n============== Compiling i2cwrite.c =============="
	$(CC) -c $(CFLAGS) i2cwrite.c -l onioni2c -l oniondebug

KT-CTRL: kt-ctrl.o ctime.o gpio18.o i2cpacket.o sock.o cmdpacket.o
	@echo "\r\n============== Linking kt-ctrl =============="
	$(CC) $(CFLAGS) kt-ctrl.o ctime.o gpio18.o cmdpacket.o i2cpacket.o sock.o -o kt-ctrl $(LDFLAGS) -l ugpio -l onioni2c -l oniondebug

KPN-CTRL: kpn-ctrl.o ctime.o rs485.o kpn_sock.o cmdpacket.o
	@echo "\r\n============== Linking kpn-ctrl =============="
	$(CC) $(CFLAGS) kpn-ctrl.o ctime.o cmdpacket.o rs485.o kpn_sock.o -o kpn-ctrl $(LDFLAGS)

kt-ctrl.o: 
	@echo "\r\n============== Compiling kt-ctrl.c =============="
	$(CC) -c $(CFLAGS) kt-ctrl.c

kpn-ctrl.o: 
	@echo "\r\n============== Compiling kpn-ctrl.c =============="
	$(CC) -c $(CFLAGS) kpn-ctrl.c

ctime.o:
	@echo "\r\n============== Compiling ctime.c ============="
	$(CC) -c $(CFLAGS) ctime.c

gpio18.o:
	@echo "\r\n============== Compiling gpio18.c ============"
	$(CC) -c $(CFLAGS) gpio18.c -l ugpio

cmdpacket.o:
	@echo "\r\n============== Compiling cmdpacket.c =============="
	$(CC) -c $(CFLAGS) cmdpacket.c

sock.o:
	@echo "\r\n============== Compiling sock.c =============="
	$(CC) -c $(CFLAGS) sock.c

kpn_sock.o:
	@echo "\r\n============== Compiling kpn_sock.c =============="
	$(CC) -c $(CFLAGS) kpn_sock.c

rs485.o:
	@echo "\r\n============== Compiling rs485.c =============="
	$(CC) -c $(CFLAGS) rs485test/rs485.c

i2cpacket.o:
	@echo "\r\n============== Compiling i2cpacket.c =============="
	$(CC) -c $(CFLAGS) i2cpacket.c -l onioni2c -l oniondebug

#queue.o: 
#	@echo "\r\n============== Compiling queue.c =============="
#	$(CC) -c $(CFLAGS) queue.c

clean:
	@echo "\r\n============== CLEAN ALL =============="
	@rm -rf *.o main i2cread i2cwrite
