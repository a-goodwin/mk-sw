#ifndef QUEUE_H
#define QUEUE_H

#define QUEUE_SIZE 2048
static unsigned char queue_buf[QUEUE_SIZE];

static int wptr=0;
static int rptr=0;

void queue_add(unsigned char* ptr, unsigned short int size);
unsigned short int queue_size(void);
unsigned short queue_get(unsigned short size, unsigned char** bptr);




#endif // QUEUE_H
