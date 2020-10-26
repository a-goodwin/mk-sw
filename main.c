#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
//#include <iostream>

#include <fcntl.h>
#include <sys/types.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <errno.h>


#include <ugpio/ugpio.h>
//#include <time.h>
#include "globals.h"
#include "queue.h"
#include "i2cpacket.h"


static const char* ver=VER_STR;

// program control vars
//static const char *defPath = "/var";
static const char* logFName = LOG_FNAME;
static int rq;

static int dMode = 1;
static bool eStatus = 0;

// tcp control vars
static const unsigned short sockPortBase = 7000;
static const int sockCount = C_KT_COUNT+1;

static struct sockaddr_in serv_addr[C_KT_COUNT+1];
static int listenfd[C_KT_COUNT+1];

int connfd[C_KT_COUNT+1];

static char sendBuf[1025];
static char recvBuf[1025];
//static time_t ticks;

// HAL pins and i2c control vars
static const int irqPin = 18;

void term(int signum);

int gpio_init(void);

void sock_init(void);
void sock_poll(void);



int main(int argc, char **argv, char **envp)
{
    ///
        //int i;
        //int rv;
        //int gpio;
    ///

    int value=0, ov=0;

    struct sigaction sact;

    int ret;
    int opt;
    int nkt = C_KT_COUNT;
    printf("IP2C process ver %s \r\n", ver);
    printf("%i KT ports\r\n", nkt);
    // check console args
    while((opt=getopt(argc, argv, "nh")) != -1) {
        switch (opt) {
        case 'n':
            dMode = false;
            break;
        case 'h':
            fprintf(stderr, "TCP to i2c packet router\nUsage: %s [-n] [-h]\n-n\t- no daemonize and log to console\n\t Otherwise daemonize and log to our log file\n", argv[0]);
            return 0;
            break;
        default:
            break;
        }
    }

    if (dMode) {
        printf("Daemonize...\n");
        // daemonize
        if (daemon(1,0)<0) {
            printf("Error during daemonize\n");
            return(-3);
        }
        // redirect stdout and stderr
        freopen(logFName, "a", stdout);
        freopen(logFName, "a", stderr);
        setbuf(stdout, NULL);
        setbuf(stderr, NULL);
    }

    // sigterm handler set
    memset(&sact, 0, sizeof(sact));
    sact.sa_handler = term;
    sigaction(SIGTERM, &sact, NULL);
    sigaction(SIGINT, &sact, NULL);

    // export irq pin from file system
    ret = gpio_init();
    if (ret<0) {
        perror("error in GPIO_INIT");
        return EXIT_FAILURE;
    }
    // DONE init i2c
    // init i2c_queue
    // DONE: init tcp sockets
    sock_init();


/////////// main loop ///////////////
    printf("Starting main cycle!\r\n");
    while(!eStatus) {
        // poll socket
        sock_poll();

        // poll gpio and i2c
        value = gpio_get_value(irqPin);
        if (value != ov) {
            printf("GPIO %i -> %i\r\n", irqPin, value);
            ov = value;
        }
        if (value==C_GPIO_SIGNAL_VALUE) {
            i2c_poll();
        }
        //sleep(1);
    }
/////////////////////////////////////



    // unexport the gpio
    if (!rq) {
        printf("> unexporting gpio\r\n");
        if (gpio_free(irqPin) < 0)
        {
            perror("gpio_free");
        }
    }

    printf("goodbye!\r\n");
    return 0;
}


void term(int signum)
{
   printf("SIGNAL Caught! sig %i \r\n", signum);
   eStatus = 1;
}

int gpio_init(void)
{
    int rv;
    // check if gpio is already exported
    if ((rq = gpio_is_requested(irqPin)) < 0)
    {
        perror("gpio_is_requested");
        return -1;
    }
    // export the gpio
    if (!rq) {
        printf("> exporting gpio\n");
        if ((rv = gpio_request(irqPin, NULL)) < 0)
        {
            perror("gpio_request");
            return -2;
        }
    }

    // set to input direction
    printf("> setting to input\n");
    if ((rv = gpio_direction_input(irqPin)) < 0)
    {
        perror("gpio_direction_input");
        //return EXIT_FAILURE;
    }
    return 0;
}

void sock_init(void)
{
    unsigned short i=0;
    int fd;
    int ret;

    printf("sock_init()\r\n");
    memset(&serv_addr, '\0', sizeof(serv_addr));
    memset(sendBuf, '\0', sizeof(sendBuf));
    for (i=0; i<sockCount; i++) {
        connfd[i] = -1;

        fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
        if (fd<0) {
            printf("socket creating error %i\r\n", i);
        }
        listenfd[i] = fd;
        serv_addr[i].sin_family = AF_INET;
        serv_addr[i].sin_addr.s_addr = htonl(INADDR_ANY);
        serv_addr[i].sin_port = htons(sockPortBase + i);
        ret = bind(listenfd[i], (struct sockaddr*)&(serv_addr[i]), sizeof(serv_addr));
        if (ret<0) printf("error in  bind socket %i\r\n", i);
        ret = listen(listenfd[i], 1);
        if (ret<0) printf("error in turn socket to listen state %i\r\n", i);
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
                    printf("@%i: packet(%i)\r\n", i+sockPortBase, n);
                    printPacket((unsigned char*)recvBuf);
                    // send to i2c
                    if (i>0) devSendPacket(i-1, (unsigned char*)recvBuf, n);
                    else {
                        // TODO: send packet po KPN via uart
                    }
                    //n = write(connfd[i], "OK\r\n", 4);
                }
            } else { // socket is gone
                printf("%i: %i gone, clearing\r\n", i, connfd[i]);
                close(connfd[i]);
                connfd[i]=-1;
            }
        }
    }
}

