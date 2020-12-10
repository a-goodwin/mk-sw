# main compiler
CC := g++

all: KT-CTRL I2CREAD I2CWRITE KPN-CTRL


KPN-CTRL: kpn-ctrl.o ctime.o rs485.o kpn_sock.o cmdpacket.o
	@echo "\r\n============== Linking kpn-ctrl =============="
	$(CC) $(CFLAGS) kpn-ctrl.o ethpacket.o ctime.o cmdpacket.o rs485.o kpn_sock.o -o kpn-ctrl $(LDFLAGS) -l stdc++

kpn-ctrl.o: 
	@echo "\r\n============== Compiling kpn-ctrl.cpp =============="
	$(CC) -c $(CFLAGS) kpn-ctrl.cpp




KT-CTRL: kt-ctrl.o camlist.o ctime.o gpio18.o i2cpacket.o sock.o cmdpacket.o cktcontainer.o ckt.o ethpacket.o
	@echo "\r\n============== Linking kt-ctrl =============="
	$(CC) $(CFLAGS) kt-ctrl.o cktcontainer.o ckt.o ethpacket.o camlist.o ctime.o gpio18.o cmdpacket.o i2cpacket.o sock.o -o kt-ctrl \
	$(LDFLAGS) -l ugpio -l onioni2c -l oniondebug -l stdc++

kt-ctrl.o: 
	@echo "\r\n============== Compiling kt-ctrl.cpp =============="
	$(CC) -c $(CFLAGS) kt-ctrl.cpp



ckt.o: 
	@echo "\r\n============== Compiling ckt.cpp =============="
	$(CC) -c $(CFLAGS) ckt.cpp

cktcontainer.o: ckt.o
	@echo "\r\n============== Compiling cktcontainer.cpp =============="
	$(CC) -c $(CFLAGS) cktcontainer.cpp

ethpacket.o: 
	@echo "\r\n============== Compiling ethpacket.cpp =============="
	$(CC) -c $(CFLAGS) ethpacket.cpp

camlist.o:
	@echo "\r\n============== Compiling camlist.cpp =============="
	$(CC) -c $(CFLAGS) camlist.cpp

ctime.o:
	@echo "\r\n============== Compiling ctime.cpp ============="
	$(CC) -c $(CFLAGS) ctime.cpp

gpio18.o:
	@echo "\r\n============== Compiling gpio18.c ============"
	$(CC) -c $(CFLAGS) gpio18.c -l ugpio

cmdpacket.o:
	@echo "\r\n============== Compiling cmdpacket.c =============="
	$(CC) -c $(CFLAGS) cmdpacket.c

sock.o:
	@echo "\r\n============== Compiling sock.cpp =============="
	$(CC) -c $(CFLAGS) sock.cpp

kpn_sock.o:
	@echo "\r\n============== Compiling kpn_sock.cpp =============="
	$(CC) -c $(CFLAGS) kpn_sock.cpp

rs485.o:
	@echo "\r\n============== Compiling rs485.c =============="
	$(CC) -c $(CFLAGS) rs485test/rs485.c

i2cpacket.o:
	@echo "\r\n============== Compiling i2cpacket.cpp =============="
	$(CC) -c $(CFLAGS) i2cpacket.cpp -l onioni2c -l oniondebug

clean:
	@echo "\r\n============== CLEAN ALL =============="
	@rm -rf *.o main i2cread i2cwrite



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

