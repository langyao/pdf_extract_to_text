#ifndef _COMM_H_
#define _COMM_H_

#include <stdlib.h>

#define malloc 不要直接调用malloc!
#define MALLOC(num,type) (type*)alloc(num*sizeof(type))

extern void *alloc(size_t size);
extern  bool iswhite(int ch);


#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

#endif
