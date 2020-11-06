#ifndef I2CPACKET_H
#define I2CPACKET_H

void printPacket(unsigned char* buf);
int devGetPacket(int devnum, int devId, unsigned char *bufptr);
int devSendPacket(int devId, unsigned char *buf, int size);
void printhex(char* str, unsigned char* buf, int sz);

void i2c_init(void);
void i2c_poll(void);
void _i2c_read(int devId, unsigned char *buffer, int reqBytes);
#endif // I2CPACKET_H
