#include "pdf.h"

#define ISNUMBER \
	'+':case'-':case'.':case'0':case'1':case'2':case'3':\
	case'4':case'5':case'6':case'7':case'8':case'9'
#define ISWHITE \
	'\000':case'\011':case'\012':case'\014':case'\015':case'\040'
#define ISHEX \
	'0':case'1':case'2':case'3':case'4':case'5':case'6':\
	case'7':case'8':case'9':case'A':case'B':case'C':\
	case'D':case'E':case'F':case'a':case'b':case'c':\
	case'd':case'e':case'f'
#define ISDELIM \
	'(':case')':case'<':case'>':case'[':case']':case'{':\
	case'}':case'/':case'%'

#define RANGE_0_9 \
	'0':case'1':case'2':case'3':case'4':case'5':\
	case'6':case'7':case'8':case'9'
#define RANGE_a_f \
	'a':case'b':case'c':case'd':case'e':case'f'
#define RANGE_A_F \
	'A':case'B':case'C':case'D':case'E':case'F'

static inline void lexwhite(pdf_stream *f)
{
    int c;
    do{
        c = read_byte(f);
    }while(c <= 32 && iswhite(c));
    if(c != EOF)
        pdf_unreadbyte(f);

}

static void lexname(pdf_stream *f,char *s, int n)
{
    while(n > 1)
    {
        int c = read_byte(f);
        switch(c)
        {
            case ISWHITE:
            case ISDELIM:
                pdf_unreadbyte(f);
                goto end;
            default:
                *s++ = c;
                n--;
                break;
        }
    }
end:
    *s = '\0';
}


static int pdf_tokenfromkeyword(char *key)
{
    switch(*key)
    {
        case 't':
            if(!strcmp(key,"trailer")) return PDF_TTRAILER;
            break;
        case 'f':
            break;
        default:
            break;
    }
    return PDF_TKEYWORD;

}


static int  lexhexstring(pdf_stream *f,char *buf,int n)
{
    char *s = buf;
    char *e = buf + n;
    int a = 0,x = 0;
    int c;

    while(s < e)
    {
        c = read_byte(f);
        switch (c)
        {
            case ISWHITE:
                break;
            case ISHEX:
                if (x)
                {
                    //  *s++ = a*16 + fromhex(c);
                    x = !x;
                }
                else
                {
                    //   a = fromhex(c);
                    x = !x;
                }
                break;
            case '>':
            default:
                goto end;
        }
    }
end:
    return s - buf;

}

int pdf_lex(int *tok,pdf_stream *f,char *buf,int n,int *sl)
{
    while(1)
    {
        int c = read_byte(f);
        printf("c:%c\n",c);
        switch(c)
        {
            case EOF:
                *tok = PDF_TEOF;
                return 1;
            case ISWHITE:
                lexwhite(f);
                break;
            case '<':
                c = read_byte(f);
                printf("c:%c\n",c);
                if( c == '<')
                {
                    *tok = PDF_TODICT;
                }
                else
                {
                    pdf_unreadbyte(f);
                    *sl = lexhexstring(f,buf,n);
                    *tok = PDF_TSTRING;
                }
                return 1;

            default: /* isregular: !isdelim && !iswhite && c != EOF */
                pdf_unreadbyte(f);
                lexname(f, buf, n);
                *sl = strlen(buf);
                *tok = pdf_tokenfromkeyword(buf);
                return 1;
        }
    }
}
