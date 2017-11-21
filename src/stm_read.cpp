#include "pdf.h"

void pdf_fillbuffer(pdf_stream *stm)
{
    int n;

    assert(stm->rp == stm->wp);

    n = stm->read(stm,stm->bp,stm->ep - stm->bp);
    if( n > 0)
    {
        stm->rp = stm->bp;
        stm->wp = stm->bp + n;
        stm->pos += n;
    }
    else
        err_ret("read error");
}


void pdf_readline(pdf_stream *stm,char *mem, int n)
{
    char *s = mem;
    char c;
    while(n > 1)
    {
        c = read_byte(stm);
        if(c == EOF)
            break;
        if(c == '\r')
        {
            c = read_peerbyte(stm);
            if(c == '\n')
            {
                  read_byte(stm);
            }
            break;
        }
        if(c == '\n')
            break;
        *s++ = c;
        n--;
    }
    if(n)
        *s = '\0';

}

int pdf_tell(pdf_stream *stm)
{
    return stm->pos - (stm->wp - stm->rp);
}

void pdf_seek(pdf_stream *stm,int offset, int whence)
{
    if(stm->seek)
    {
        if(whence == 1)
        {
            offset = pdf_tell(stm) + offset;
            whence = 0;
        }
        stm->seek(stm,offset,whence);
    }
}

