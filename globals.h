#ifndef GLOBALS_H
#define GLOBALS_H


#ifndef __cplusplus
# define false 0
# define true 1
typedef unsigned char bool;
#endif

#define VER_STR "0.06t alpha ip gpio i2c"
#define C_KT_COUNT 1
#define LOG_FNAME "/var/log/ip2c.log"

extern int connfd[C_KT_COUNT];


#endif // GLOBALS_H
