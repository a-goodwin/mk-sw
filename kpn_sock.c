#include "kpn_sock.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <errno.h>
#include "globals.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "cmdpacket.h"
#include "rs485test/rs485.h"
#include "ctime.h"

extern tDevInst uart;

static char sendBuf[1025];
static char recvBuf[1025];

//static char packetBuf[256];
//static int packetSz=0;

static const unsigned short sockPortBase = SOCK_PORT_BASE;

static struct sockaddr_in serv_addr;
static int listenfd;
static int connfd;

void kpn_sock_init(void)
{
    int fd;
    int ret;
    int tr = 1;
    printf(CLKHD "sock_init()\r\n", getms());
    memset(&serv_addr, '\0', sizeof(serv_addr));
    memset(sendBuf, '\0', sizeof(sendBuf));
    connfd = -1;

    fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK , 0);
    listenfd=-1;
    if (fd<0) {
        printf("socket creating error\r\n");
        return;
    }
    listenfd = fd;

    // set socket to be completely closed immediately after use
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &tr, sizeof(int));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(sockPortBase);
    ret = bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (ret<0) printf("error in bind socket %i\r\n", 0);
    ret = listen(listenfd, 1);
    if (ret<0) printf("error in turn socket to listen state %i\r\n", 0);
}

void kpn_sock_done(void)
{
    // close all data socket
    if (connfd>0) { // poll connected sockets for data
        close(connfd);
    }
    // close all listen socket
    if (listenfd>0) {
        close(listenfd);
    }
}

void kpn_sock_send(unsigned char* buf, size_t size)
{
    if (connfd>0)
    {
        write(connfd, buf, size);
        //printf("socksend %i bytes to fd %i\r\n", size, connfd[i]);
    }
}

void kpn_sock_poll(void)
{
    int fd;
    int n=0;
    //printf(".\r\n");
    fd = accept4(listenfd, (struct sockaddr*) NULL, NULL, SOCK_NONBLOCK);
    if (connfd!=fd) {
        if (fd>0) {
            if (connfd>0) close(connfd); // the newest connection accepted always, old one disconnects!
            //                printf("accpt: %i: %i ->%i\r\n", i, connfd[i], fd);
            printf("New conn @%i, fd=%i\r\n", sockPortBase, fd);
            connfd = fd;
        }
    }

    // poll for data
    //printf("-\r\n");
    if (connfd>0) { // poll connected sockets for data
        //printf("?\r\n");
        if (recv(connfd,NULL,1, MSG_PEEK | MSG_DONTWAIT) != 0) {
            n = read(connfd, recvBuf, sizeof(recvBuf)-1);
            //printf("!\r\n");
            if (n>0) { // data received!
                printf(CLKHD " @%i: packet(%i) ", getms(), sockPortBase, n);
                printPacket((unsigned char*)recvBuf);
                // send packet to KPN via uart
                sendPacket(&uart, (unsigned char*)recvBuf, n);
            }
        } else { // socket is gone
            printf(CLKHD " remote %i gone, clearing\r\n", getms(), connfd);
            close(connfd);
            connfd=-1;
        }
    }
}
