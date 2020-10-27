#ifndef RS48USB_H
#define RS48USB_H
#include <iostream>
#include <vector>

#ifdef __cplusplus
extern "C"
{
#endif

namespace rs485usb {
    enum devState {sClosed, sOpened};
    typedef struct {
        int fd;
        unsigned char pbuf[64];
        int ppos;
        devState state;
    } tDevInst;
    typedef std::vector<std::string> tDevList;
    typedef tDevList* pDevList;

    static unsigned char header[4]={0xAA, 0xBB, 0x57, 0x00};

    void set_blocking (int fd, int should_block);
    int set_interface_attribs (int fd, int speed, int parity);

    // enumerate and find device, list properties
    int devsInit();
    pDevList getDevs();
    // open/close device
    int openDev(std::string name, tDevInst *dev);
    int closeDev(tDevInst *dev);
    // send packet
    int sendPacket(tDevInst *dev, unsigned char *data, unsigned int size);
    //get received packet
    int receivePacket(tDevInst *dev, unsigned char* buf, unsigned int e_size);
    // packet callback


    static tDevList list;
    static const std::string cDevMask = "/dev/ttyUSB";

}

#ifdef __cplusplus
}
#endif

#endif // RS48USB_H
