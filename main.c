#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
//#include <iostream>

#include <fcntl.h>
#include <sys/types.h>
#include <string.h>

#include <errno.h>


#include <ugpio/ugpio.h>
//#include <time.h>
#include "globals.h"
#include "queue.h"
#include "i2cpacket.h"
#include "rs485test/rs485.h"
#include "sock.h"

static const char* ver=VER_STR;

// program control vars
//static const char *defPath = "/var";
static const char* logFName = LOG_FNAME;
static int rq;

static int dMode = 1;
static bool eStatus = 0;


//static time_t ticks;

// UART port
static const char* c_kpn_uart_name="/dev/ttyS1";
extern tDevInst uart;
tDevInst uart;

// HAL pins and i2c control vars
static const int irqPin = 18;

void term(int signum);

int  gpio_init(void);
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
    // init i2c_queue
    // DONE: init tcp sockets
    sock_init();
    // serial device init
    ret = openDev(c_kpn_uart_name, &uart);
    if (ret<0) {
        printf("tty open error\r\n");
    }


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
        if (value==C_GPIO_ACTIVE_VALUE) {
            i2c_poll();
        }

        // poll uart
        ret = receiveFSM(&uart);
        if (ret>0) { //has packet!
            getPacket(&uart, &uart_size, &uart_buf);
            printf("uart has packet\r\n");
            printPacket(uart_buf);
            sock_send(0, uart_buf, uart_size);
        }
        //sleep(1);
    }
    /////////////////////////////////////


    printf("Closing sockets\r\n");
    sock_done();

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

