#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

#include "globals.h"
#include "cmdpacket.h"
#include "rs485test/rs485.h"
#include "kpn_sock.h"
#include "ctime.h"

static const char* ver=VER_STR;

// program control vars
//static const char *defPath = "/var";
static const char* logFName = KPN_LOG_FNAME;

static int dMode = 1;
static bool eStatus = 0;


//static time_t ticks;

// UART port
static const char* c_kpn_uart_name="/dev/ttyS1";
extern tDevInst uart;
tDevInst uart;


void term(int signum);


int main(int argc, char **argv, char **envp)
{

    struct sigaction sact;

    int ret;
    int opt;
    int uart_size;
    unsigned char* uart_buf;

    printf(CLKHD " kpn control process ver %s \r\n", getms(), ver);
    printf("kpn uart port %s\r\n", c_kpn_uart_name);
    // check console args
    while((opt=getopt(argc, argv, "nh")) != -1) {
        switch (opt) {
        case 'n':
            dMode = false;
            break;
        case 'h':
            printf("TCP to uart packet router for kpn device\nUsage: %s [-n] [-h]\n-n\t- no daemonize and log to console\n\t Otherwise daemonize and log to our log file\n", argv[0]);
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

    // DONE: init tcp sockets
    kpn_sock_init();
    // serial device init
    ret = openDev(c_kpn_uart_name, &uart);
    if (ret<0) {
        printf("tty %s open error\r\n", c_kpn_uart_name);
    }


    /////////// main loop ///////////////
    printf("Starting main cycle!\r\n");
    while(!eStatus) {
        // poll tcp sockets
        kpn_sock_poll();

        // poll kpn uart
        ret = receiveFSM(&uart);
        if (ret>0) { //has packet!
            getPacket(&uart, &uart_size, &uart_buf);
            printf(CLKHD " rs485 pkt ", getms());
            printhex("", uart_buf, uart_size);
            kpn_sock_send(uart_buf, uart_size);
        }
        //sleep(1);
    }
    /////////////////////////////////////


    printf("Closing sockets\r\n");
    kpn_sock_done();
    closeDev(&uart);

    printf(CLKHD " goodbye!\r\n", getms());
    return 0;
}

void term(int signum)
{
   printf("SIGNAL Caught! sig %i \r\n", signum);
   eStatus = 1;
}


