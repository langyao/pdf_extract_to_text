#include "pdf.h"


int pdf_loadVersion(pdf_xref *pdfxref)
{
    char buf[20];

    pdf_readline(pdfxref->file,buf,sizeof(buf));
    if(memcmp(buf,"%PDF-",5) != 0)
    {
        err_ret("can not recognize pdf version");
        return -1;
    }
    pdfxref->version = atof(buf + 5)*10;

    printf("pdf version is:%d.%d\n", pdfxref->version / 10, pdfxref->version % 10);
    return 1;

}



int pdf_readstartxref(pdf_xref *pdfxref)
{
    unsigned char  buf[1024];
    pdfxref->file->seek(pdfxref->file,0,2);

    pdfxref->filesize = pdf_tell(pdfxref->file);
    printf("filesize:%d\n",pdfxref->filesize);


    size_t t = MAX(0,pdfxref->filesize - sizeof(buf));

    pdfxref->file->seek(pdfxref->file,t,0);

    unsigned long long  n = pdf_tell(pdfxref->file);

    printf("倒叙读出字节大小为:%llu\n",n);


    n = pdfxref->file->read(pdfxref->file,buf,sizeof(buf));
    if( n < 0)
    {
        err_ret("can not read fro file");
        return -1;
    }
    //find the startxref in buf
    unsigned int i = 0;
    for(i = n - 9; i >= 0; --i)
    {
        if(memcmp(buf+i,"startxref",9) == 0)
        {
            i += 9;
            while(iswhite(buf[i]) && i < n)
                i++;
            pdfxref->startxref = atoi((char*)(buf+i));
            printf("startxref:%d\n",pdfxref->startxref);
            break;
        }
    }
    if(i == 0)
    {
        err_msg("can not find startxref");
        return -1;

    }
    return 1;

}

void pdf_strsep(char *s,char *delim,char *p[64])
{
    int i = 0;
    while((p[i] = strtok(s,delim)) != NULL)
    {
        i++;
        s = NULL;
    }
}


int pdf_readtrailer(pdf_xref *pdfxref,char *buf,int bufSize)
{
    int c;
    char *p[64];

    pdfxref->file->seek(pdfxref->file,pdfxref->startxref,0);

    while(iswhite(read_peerbyte(pdfxref->file)))
        read_byte(pdfxref->file);

    c = read_peerbyte(pdfxref->file);
    if( c == 'x')
    {
        pdf_readline(pdfxref->file,buf,bufSize);
        if(strncmp(buf, "xref", 4)!=0)
        {
            err_quit("can not read xref marker");
        }
    }
    bzero(buf,bufSize);
    while(1)
    {
        c = read_peerbyte(pdfxref->file);
        printf("c:%c\n",c);

        if(!(c>= '0' && c <= '9'))
            break;

        pdf_readline(pdfxref->file,buf,bufSize);
        printf("buf = %s\n",buf);


        pdf_strsep(buf," ",p);
        if(p[0] == NULL)
        {
            err_quit("xref read failed");
        }

        int len =atoi(p[1]);


        int  t = pdf_tell(pdfxref->file);
        pdf_seek(pdfxref->file,t+20*len,0);

    }

   // bzero(buf,bufSize);
   // pdf_readline(pdfxref->file,buf,bufSize);
   //  printf("buf = %s\n",buf);

    int tok,n;
    int error = pdf_lex(&tok,pdfxref->file,buf,bufSize,&n);
    if(tok != PDF_TTRAILER )
    {
        err_ret("expected trailer marker");
    }

    printf("buf = %s\n",buf);
    error = pdf_lex(&tok,pdfxref->file,buf,bufSize,&n);
    if(tok != PDF_TODICT)
    {
        err_ret("expected trailer dictionary");
    }


    return 1;

}

int pdf_openxrefwithstream(pdf_xref *pdfxref,pdf_stream *file)
{
    file->seek(file,0,0);

    pdfxref = MALLOC(1,pdf_xref);
    memset(pdfxref,0,sizeof(pdf_xref));

    pdfxref->file = file;


    int error;
    error = pdf_loadVersion(pdfxref);
    if(error < 0)
        err_ret("can not read pdf version");

    pdf_readstartxref(pdfxref);


    pdf_readtrailer(pdfxref,pdfxref->scratch,sizeof(pdfxref->scratch));




    //find traier


    return 1;
}

