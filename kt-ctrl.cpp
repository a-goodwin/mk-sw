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
#include "queue.h"
#include "i2cpacket.h"
//#include "rs485test/rs485.h"
#include "sock.h"
#include "gpio18.h"
#include "ctime.h"
#include "eventprocessor.h"





static const char* ver=VER_STR;

// program control vars
//static const char *defPath = "/var";
static const char* logFName = I2C_LOG_FNAME;

static int dMode = 1;
static bool eStatus = 0;

static CEventProcessor *processor;

//static time_t ticks;

void term(int signum);

int main(int argc, char **argv, char **envp)
{
    ///
        //int i;
        //int rv;
        //int gpio;
    ///

    int value=0, ov=0;

    struct sigaction sact;
    processor = new CEventProcessor();
    int ret;
    int opt;
    int nkt = C_KT_COUNT;
    int uart_size;
    unsigned char* uart_buf;

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
    i2c_init();
    // init i2c_queue
    // DONE: init tcp sockets
    sock_init();

    /////////// main loop ///////////////
    printf("Starting main cycle!\r\n");
    while(!eStatus) {
        // poll tcp sockets and send commands directly to kt
        sock_poll();

        // poll event-processor timers and do event-action chains
        processor->eventloop();

        // check i2c if gpio pin active receive commands and send it to PC and to event processor
        i2c_poll();
        usleep(1000);
    }
    /////////////////////////////////////


    printf("Closing sockets\r\n");
    sock_done();
    gpio_done();
    printf("goodbye!\r\n");
    return 0;
}

void term(int signum)
{
   printf("SIGNAL Caught! sig %i \r\n", signum);
   eStatus = 1;
}


