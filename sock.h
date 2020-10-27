#ifndef SOCK_H
#define SOCK_H
#include <sys/types.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <errno.h>
#include "globals.h"

// tcp control vars


void sock_init(void);
void sock_send(int i, unsigned char *buf, size_t size);
void sock_poll(void);



#endif // SOCK_H
