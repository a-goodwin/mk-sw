#include "rs485usb.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/serial.h>
#include <sys/ioctl.h>
#include <iostream>

#include <errno.h>
#include <string.h>
#include <termios.h>

using namespace rs485usb;
using namespace std;

int rs485usb::set_interface_attribs (int fd, int speed, int parity)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                cerr << "error %d from tcgetattr" << errno;
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
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= parity;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
        {
                cerr << "error %d from tcsetattr " << errno;
                return -1;
        }
        return 0;
}

void rs485usb::set_blocking (int fd, int should_block)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                cerr << "error %d from tggetattr" << errno;
                return;
        }

        tty.c_cc[VMIN]  = should_block ? 1 : 0;
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
                cerr << "error %d setting term attributes" << errno;
}

int rs485usb::devsInit()
{
    std::string path;
    int i;
    int fd;
    for (i=0;i<256; i++) {
        path.clear();
        path.append(cDevMask);
        path.append(std::to_string(i));
        fd=open(path.c_str(), O_RDWR | O_NOCTTY);
        if (fd != -1) {
            serial_struct info;
            ioctl(fd, TIOCGSERIAL, &info);
            cout << "Port " << path << " type is " << info.type << "\n";
            list.push_back(path);
            close(fd);
        }
    }

    return 0;

}

pDevList rs485usb::getDevs()
{
    return &list;
}

int rs485usb::openDev(string name, tDevInst *dev)
{
    int Status;
    dev->state = sClosed;
    int fd = open(name.c_str(), O_RDWR | O_NOCTTY);
    if (fd<=0) {
        return -1;
    }
    dev->fd = fd;
    dev->ppos=0;
    Status = set_interface_attribs(dev->fd, B115200, 0);
    if (Status < 0) return -2;

    set_blocking(dev->fd, 0);
    dev->state = sOpened;
    return 0;
}

int rs485usb::sendPacket(tDevInst *dev, unsigned char *data, unsigned int size)
{
    unsigned char crc = 0;
    unsigned int i;
    // check device
    if (dev->state!=sOpened) return -1;
    if (size>sizeof(dev->pbuf)) return -2;
    // send header
    header[3]=(unsigned char)size;

    i=4;
    write(dev->fd, header, i);
    // send data
    write(dev->fd, data, size);
    // send crc
    for(i=0; i<sizeof(header); i++) crc ^=header[i];
    for(i=0; i<size; i++) crc ^= data[i];
    write(dev->fd, &crc, 1);
    return (int)size;
}

int rs485usb::closeDev(tDevInst *dev)
{
    if (dev->state!=sOpened) return -1;
    close(dev->fd);
    return 0;
}

int rs485usb::receivePacket(tDevInst *dev, unsigned char *buf, unsigned int e_size)
{
    int Status;
    int sz = e_size+5;
    unsigned char crc=0;// header, data, crc
    if (dev->state!=sOpened) return -1;
    if (sz>sizeof(dev->pbuf)) return -2;
    Status = read(dev->fd, dev->pbuf+dev->ppos, sz-dev->ppos);
    if (Status>0) dev->ppos += Status; else
        if (Status == 0) return 0;
        else return -3;
    //cout << "Received " << Status << "bytes\n";
    if (dev->ppos < sz) return 0;
    // verify packet
    dev->ppos=0;
    if (dev->pbuf[0]!=0xBB) return 0;
    if (dev->pbuf[1]!=0xAA) return 0;
    if (dev->pbuf[2]!=0x57) return 0;
    if (dev->pbuf[3]!=e_size) return 0;
    for(int i=0; i<sz; i++) crc ^=dev->pbuf[i];
    if (crc!=0) return 0;
    memcpy(buf, dev->pbuf+sizeof(header), e_size);
    return e_size;
}
