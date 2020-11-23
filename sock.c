#include "sock.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "i2cpacket.h"
#include "rs485test/rs485.h"
#include "ctime.h"
#include "cmdpacket.h"

extern tDevInst uart;

static char sendBuf[1025];
static char recvBuf[1025];

//static char packetBuf[256];
//static int packetSz=0;

static const unsigned short sockPortBase = 1 + SOCK_PORT_BASE;
static const int sockCount = C_KT_COUNT;

static struct sockaddr_in serv_addr[C_KT_COUNT];
static int listenfd[C_KT_COUNT];

static int connfd[C_KT_COUNT];

void sock_init(void)
{
    unsigned short i=0;
    int fd;
    int ret;
    int tr = 1;

    printf(CLKHD " sock_init()\r\n", getms());
    memset(&serv_addr, '\0', sizeof(serv_addr));
    memset(sendBuf, '\0', sizeof(sendBuf));
    for (i=0; i<sockCount; i++) {
        connfd[i] = -1;

        fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
        listenfd[i]=-1;
        if (fd<0) {
            printf("socket creating error %i\r\n", i);
            continue;
        }
        listenfd[i] = fd;

        // set socket to be completely closed immediately after use
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &tr, sizeof(int));

        serv_addr[i].sin_family = AF_INET;
        serv_addr[i].sin_addr.s_addr = htonl(INADDR_ANY);
        serv_addr[i].sin_port = htons(sockPortBase + i);
        ret = bind(listenfd[i], (struct sockaddr*)&(serv_addr[i]), sizeof(serv_addr));
        if (ret<0) printf("error in bind socket %i\r\n", i);
        ret = listen(listenfd[i], 1);
        if (ret<0) printf("error in turn socket to listen state %i\r\n", i);
    }
}
void sock_done(void)
{
    int i;
    // close all data socket
    for (i=0; i<sockCount; i++) {
        if (connfd[i]>0) { // poll connected sockets for data
            close(connfd[i]);
        }

        if (listenfd[i]>0) {
            close(listenfd[i]);
        }
    }

    // close all listen socket

}

void sock_send(int i, unsigned char* buf, size_t size)
{
    if (connfd[i]>0)
    {
        write(connfd[i], buf, size);
        //printf("socksend %i bytes to fd %i\r\n", size, connfd[i]);
    }
}

void sock_poll(void)
{
    unsigned short i=0;
    int fd;
    int n=0;
    //printf(".\r\n");
    for (i=0; i<sockCount; i++) {
        fd = accept4(listenfd[i], (struct sockaddr*) NULL, NULL, SOCK_NONBLOCK);
        if (connfd[i]!=fd) {
            if (fd>0) {
                if (connfd[i]>0) close(connfd[i]); // the newest connection accepted always, old one disconnects!
//                printf("accpt: %i: %i ->%i\r\n", i, connfd[i], fd);
                printf("New conn @%i, fd=%i\r\n", i+sockPortBase, fd);
                connfd[i] = fd;
            }
        }
    }

    // poll for data
    for (i=0; i<sockCount; i++) {
        //printf("-\r\n");
        if (connfd[i]>0) { // poll connected sockets for data
            //printf("?\r\n");
            if (recv(connfd[i],NULL,1, MSG_PEEK | MSG_DONTWAIT) != 0) {
                n = read(connfd[i], recvBuf, sizeof(recvBuf)-1);
                //printf("!\r\n");
                if (n>0) { // data received!
                    printf(CLKHD " @%i: packet(%i) ", getms(), i+sockPortBase, n);
                    printPacket((unsigned char*)recvBuf);
                    // send to i2c
                    devSendPacket(i, (unsigned char*)recvBuf, n);
                    //n = write(connfd[i], "OK\r\n", 4);
                }
            } else { // socket is gone
                printf(CLKHD " %i: %i gone, clearing\r\n", getms(), i, connfd[i]);
                close(connfd[i]);
                connfd[i]=-1;
            }
        }
    }
}
