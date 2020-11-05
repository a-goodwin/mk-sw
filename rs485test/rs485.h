#ifndef RS485_H
#define RS485_H

#include <termios.h>




#ifdef __cplusplus
extern "C"
{
#endif

typedef enum {sClosed, sOpened} devState;
typedef struct {
    int fd;
    unsigned char pbuf[256]; // packet buf
    unsigned char rcvbuf[256];
    int ppos;
    int psize;
    int psize_latch;
    devState state;
} tDevInst;


void set_blocking (int fd, int should_block);
int set_interface_attribs (int fd, speed_t speed, int parity);

int openDev(const char* name, tDevInst *dev);
int closeDev(tDevInst *dev);

int sendPacket(tDevInst *dev, unsigned char *data, unsigned int size);
int receiveFSM(tDevInst *dev);
void getPacket(tDevInst *dev, int *size, unsigned char** buf);

#ifdef __cplusplus
}
#endif

#endif // RS485_H
