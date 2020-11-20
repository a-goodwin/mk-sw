#include "gpio18.h"
#include "globals.h"

#include <stdio.h>
int rq=0;
static const int irqPin = 18;
static int ov, value;

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

void gpio_done(void)
{
    // unexport the gpio
    if (!rq) {
        printf("> unexporting gpio\r\n");
        if (gpio_free(irqPin) < 0)
        {
            perror("gpio_free");
        }
    }
}

inline int gpio_poll(void)
{
    // poll gpio and i2c
    value = gpio_get_value(irqPin);
    if (value != ov) {
        //printf("GPIO %i -> %i\r\n", irqPin, value);
        ov = value;
    }
    if (value == C_GPIO_ACTIVE_VALUE) return 1; else return 0;
}

inline int gpio_value(void)
{
    return value;
}


