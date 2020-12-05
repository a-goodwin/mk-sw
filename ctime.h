#ifndef CTIME_H
#define CTIME_H
#ifdef __cplusplus
extern "C" {
#endif
typedef unsigned long long t_ctime;

t_ctime getms(void);
unsigned long getms1m(void);
#ifdef __cplusplus
}
#endif

#endif // CTIME_H
