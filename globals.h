#ifndef GLOBALS_H
#define GLOBALS_H


#ifndef __cplusplus
# define false 0
# define true 1
typedef unsigned char bool;
#endif

#define VER_STR "0.10t alpha <-ip-> gpio <-i2c->"

#define C_KT_COUNT 1
#define LOG_FNAME "/var/log/ip2c.log"

#define C_PKT_HDR_SZ 6
#define C_UART_NAME "/dev/ttyUSB0"
#define C_GPIO_SIGNAL_VALUE 0

#endif // GLOBALS_H
