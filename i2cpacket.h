#ifndef I2CPACKET_H
#define I2CPACKET_H

#define I2C_REG_RX_SIZE 0 // size of bytes to be ready in receive uffer
#define I2C_REG_RX_DATA 1 // register to pick data bytes from rx buffer
#define I2C_REG_TX_SIZE 2 // size of bytes lasts in tx buffer and ready to send to uart
#define I2C_REG_TX_DATA 3 // register to add bytes to tx buffer

int devHasData(int devId);
int devGetPacket(int devId, unsigned char *bufptr, int size);
int devSendPacket(int devId, unsigned char *buf, int size);

void i2c_poll(void);

#endif // I2CPACKET_H
