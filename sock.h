#ifndef SOCK_H
#define SOCK_H

#ifdef __cplusplus
extern "C" {
#endif

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
void sock_done(void);

#ifdef __cplusplus
}
#endif

#endif // SOCK_H
