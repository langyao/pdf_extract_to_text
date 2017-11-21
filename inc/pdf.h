#ifndef _PDF_H_HEADER
#define _PDF_H_HEADER

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>

#include <unistd.h>
#include <assert.h>
#include <string.h>

#include "err_handle.h"
#include "comm.h"


enum
{
	PDF_TERROR, PDF_TEOF,
	PDF_TOARRAY, PDF_TCARRAY,
	PDF_TODICT, PDF_TCDICT,
	PDF_TOBRACE, PDF_TCBRACE,
	PDF_TNAME, PDF_TINT, PDF_TREAL, PDF_TSTRING, PDF_TKEYWORD,
	PDF_TR, PDF_TTRUE, PDF_TFALSE, PDF_TNULL,
	PDF_TOBJ, PDF_TENDOBJ,
	PDF_TSTREAM, PDF_TENDSTREAM,
	PDF_TXREF, PDF_TTRAILER, PDF_TSTARTXREF,
	PDF_NTOKENS
};

typedef struct pdf_stream_s pdf_stream;
typedef struct pdf_stream_s
{
    int refs;
    int pos;
    unsigned char *bp,*rp,*wp,*ep;
    void *state;
    int  fd;
    int (*read)(pdf_stream *stm,unsigned char *buf,int len);
    void (*close)(pdf_stream *stm);
    void (*seek)(pdf_stream *stm, int offset, int whence);
    unsigned char buf[4096];
}pdf_stream;

struct pdf_obj
{

};

typedef struct pdf_xref_s pdf_xref;
struct pdf_xref_s
{
    pdf_stream *file;
    int version;
    int startxref;
    int filesize;
    pdf_obj *trailer;

    int len;
  //  pdf_xrefentry *table;

    pdf_obj **pageobjs;
    pdf_obj **pagerefs;

    char scratch[65536];

};

typedef struct pdf_s
{
    char *doctile;
    pdf_xref *xref;
    int pagecount;
}pdf_s;


//stm_read
extern void pdf_fillbuffer(pdf_stream *stm);
extern void pdf_readline(pdf_stream *stm,char *mem, int n);
extern int pdf_tell(pdf_stream *stm);
extern void pdf_seek(pdf_stream *stm,int offset, int whence);

//pdf_open
extern pdf_stream *pdf_openfile(int fd);


static inline int read_peerbyte(pdf_stream *stm)
{
    if(stm->rp == stm->wp)
    {
        pdf_fillbuffer(stm);
        return stm->rp < stm->wp? *stm->rp : EOF;
    }
    return *stm->rp;
}

static inline int read_byte(pdf_stream *stm)
{
    if(stm->rp == stm->wp)
    {
        pdf_fillbuffer(stm);
        return stm->rp < stm->wp? *stm->rp++ : EOF;
    }
    return *stm->rp++;
}


static inline void pdf_unreadbyte(pdf_stream *stm)
{
	if (stm->rp > stm->bp)
		stm->rp--;
}
//pdf_xref

extern int pdf_loadVersion(pdf_xref *pdfxref);
extern int pdf_readstartxref(pdf_xref *pdfxref);
extern int pdf_readtrailer(pdf_xref *pdfxref,char *buf,int bufSize);
extern int pdf_openxrefwithstream(pdf_xref *pdfxref,pdf_stream *file);


//pdf_lex

extern int pdf_lex(int *tok,pdf_stream *f,char *buf,int n,int *sl);
#endif
