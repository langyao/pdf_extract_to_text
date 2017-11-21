#include "pdf.h"
static int readfile(pdf_stream *stm,unsigned char *buf,int len)
{
    int n = read(*(int*)stm->state,buf,len);
    if(n < 0)
    {
        err_ret("read error");
        return -1;
    }
    return n;
}

static void seekfile(pdf_stream *stm, int offset, int whence)
{
    int n = lseek(*(int *)stm->state,offset,whence);
    if(n < 0)
    {
        err_ret("can not lseek");
    }
    stm->pos = n;
    stm->rp = stm->bp;
    stm->wp = stm->bp;
}

static void closefile(pdf_stream *stm)
{
    int n = close(*(int*)stm->state);
    if(n < 0)
        err_ret("close error");
    free(stm->state);
}


pdf_stream *pdf_openfile(int fd)
{
    pdf_stream *stm = MALLOC(1,pdf_stream);

    int *state;
    state = MALLOC(1,int);

    *state = fd;
    stm->state = (void*)state;

    stm->refs =1;
    stm->pos = 0;

    stm->read = readfile;
    stm->seek = seekfile;
    stm->close = closefile;


    stm->bp = stm->buf;
    stm->rp = stm->bp;
    stm->wp = stm->bp;
    stm->ep = stm->buf + sizeof(stm->buf);

    return stm;
}

