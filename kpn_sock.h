#ifndef SOCK_H
#define SOCK_H
#include <sys/types.h>

// tcp control vars


void kpn_sock_init(void);
void kpn_sock_send(unsigned char *buf, size_t size);
void kpn_sock_poll(void);
void kpn_sock_done(void);

#endif // SOCK_H
