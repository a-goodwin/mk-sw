#ifndef I2CPACKET_H
#define I2CPACKET_H

#ifdef __cplusplus
extern "C" {
#endif

int devGetPacket(int devId, unsigned char *bufptr);
int devSendPacket(int devId, unsigned char *buf, int size);

void i2c_init(void);
void i2c_poll(void);
void _i2c_read(int devId, unsigned char *buffer, int reqBytes);

#ifdef __cplusplus
}
#endif

#endif // I2CPACKET_H

