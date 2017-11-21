#include "pdf.h"
#include "err_handle.h"
#include "comm.h"


int main(void)
{
    int fd;
    fd = open("pdftest.pdf",O_RDONLY,0666);
    if(fd < 0)
        err_sys("open file failed");

    pdf_xref *pdfxref=NULL;

    pdf_stream *stm = pdf_openfile(fd);

    int error = pdf_openxrefwithstream(pdfxref,stm);



    return 0;

}
