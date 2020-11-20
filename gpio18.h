#ifndef GPIO18_H
#define GPIO18_H
#include <ugpio/ugpio.h>
extern int rq;
// HAL pins and i2c control vars

int  gpio_init(void);
void gpio_done(void);

int gpio_poll(void);
int gpio_value(void);
#endif // GPIO18_H
