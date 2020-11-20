#include "rs485.h"


#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/serial.h>
#include <sys/ioctl.h>

#include <errno.h>
#include <string.h>
#include "../i2cpacket.h"
#include "../cmdpacket.h"
#include "../ctime.h"

int set_interface_attribs (int fd, speed_t speed, int parity)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                printf("error %d from tcgetattr", errno);
                return -1;
        }

        cfsetospeed (&tty, speed);
        cfsetispeed (&tty, speed);

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // disable break processing
        tty.c_lflag = 0;                // no signaling chars, no echo,
                                        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 1;            // 0.1 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= parity;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
        {
                printf("error %d from tcsetattr ", errno);
                return -1;
        }
        return 0;
}

void set_blocking (int fd, int should_block)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                printf("error %d from tggetattr",errno);
                return;
        }

        tty.c_cc[VMIN]  = should_block ? 1 : 0;
        tty.c_cc[VTIME] = 1;            // 0.5 seconds read timeout

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
                printf("error %d setting term attributes",errno);
}

int openDev(const char* name, tDevInst *dev)
{
    int Status;
    dev->state = sClosed;
    int fd = open(name, O_RDWR | O_NOCTTY);
    if (fd<=0) {
        return -1;
    }
    dev->fd = fd;
    dev->ppos=0;
    Status = set_interface_attribs(dev->fd, B9600, 0);
    if (Status < 0) return -2;

    set_blocking(dev->fd, 0);
    dev->state = sOpened;
    return 0;
}

int closeDev(tDevInst *dev)
{
    if (dev->state!=sOpened) return -1;
    close(dev->fd);
    return 0;
}


int sendPacket(tDevInst *dev, unsigned char *data, unsigned int size)
{
    ssize_t i;
    // check device
    if (dev->state!=sOpened) return -1;
    if (size>sizeof(dev->pbuf)) return -2;
    //printPacket(data);
    // send data
    //printf("uart_wBuf: %i\r\n", size);
    i = write(dev->fd, data, size);
    return (int)i;
}

int receiveFSM(tDevInst *dev)
{
    int Status;
    unsigned char* pcb; // *current byte
    if (dev->state!=sOpened) return 0;

    Status = read(dev->fd, dev->rcvbuf+dev->ppos, 1);
    if (Status<=0) return 0;
    // some data received, parse
    pcb = dev->rcvbuf+dev->ppos; // current byte received
    switch (dev->ppos) {
    case 0: // signature
        if (*pcb!=SLAVE_SIGNATURE) {
            printf("%06ul bad signature: 0x%02x\r\n", getms(), *pcb);
            dev->ppos = 0;
            dev->psize = 0;
            break;
        }

        if (*pcb==SLAVE_SIGNATURE) dev->psize = C_STD_PACKET_SZ;
        dev->ppos += 1;
        break;

    case 1: // addr hi or 2-nd signature
        if ((dev->psize == 2) && ((*pcb) == *(pcb-1))) { // small packet arrived ok
            memcpy(dev->pbuf, dev->rcvbuf, dev->psize);
            dev->psize_latch = dev->psize;
            dev->ppos = 0;
            dev->psize = 0;
            return dev->psize_latch;
        } else dev->ppos += 1;
        break;

    case 2: // addr lo
        dev->ppos += 1;
        break;

    case 3: // cmd id
        dev->ppos += 1;
        break;

    case 4: // size
        dev->psize = C_STD_PACKET_SZ+(*pcb)+1;
        dev->ppos += 1;
        break;

    case 5: // hcrc
        dev->ppos += 1;
        if (dev->psize==C_STD_PACKET_SZ) { // zadel for next version oof protocol
            memcpy(dev->pbuf, dev->rcvbuf, dev->psize);
            dev->psize_latch = dev->psize;
            dev->ppos = 0;
            dev->psize = 0;
            return dev->psize_latch;
        }
        break;

    default: // data+dcrc
        dev->ppos += 1;
        if (dev->ppos==dev->psize) {
            memcpy(dev->pbuf, dev->rcvbuf, dev->psize);
            dev->psize_latch = dev->psize;
            dev->ppos = 0;
            dev->psize = 0;
            return dev->psize_latch;
        }
        break;
    }

    return 0;
}

void getPacket(tDevInst *dev, int *size, unsigned char** buf)
{
    *size = dev->psize_latch;
    *buf = dev->pbuf;
}
