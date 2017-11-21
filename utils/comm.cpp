#include <stdio.h>
#include "err_handle.h"
#include "comm.h"

#undef malloc


void *alloc(size_t size)
{
    void *new_mem;
    new_mem = malloc(size);
    if(new_mem == NULL)
    {
        err_sys("out of memory!\n");
    }
    return new_mem;
}

bool iswhite(int ch)
{
    return
            ch == '\000' || ch == '\011' || ch == '\012' ||
            ch == '\014' || ch == '\015' || ch == '\040';
}

