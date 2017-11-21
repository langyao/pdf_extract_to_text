/**
 * Error Deal
 *
 * @param
 */
#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<syslog.h>
#include <string.h>
#include <errno.h>

#define MAXLINE 1024

int daemon_proc  = 0;  //若该全局变量不为0，则出错消息按指定的日志级别传给syslog,否则打在标准输出上


static void err_doit(int,int, const char *,va_list);


void
err_ret(const char *fmt,...)
{
	va_list ap;
	va_start(ap,fmt);
	err_doit(1,LOG_ERR,fmt,ap);
	va_end(ap);
	return;
}

void
err_sys(const char *fmt,...)
{
    va_list ap;
    va_start(ap,fmt);
    err_doit(1,LOG_ERR,fmt,ap);
    va_end(ap);
    exit(1);
}

/*fatal error related to system call
 * print message,dump core, and terminate */
void
err_abort(const char *fmt,...)
{
	va_list ap;
	va_start(ap,fmt);
	err_doit(1,LOG_ERR,fmt,ap);
	va_end(ap);
	abort();
	exit(1);
}

/*not fatal error unrelated to system call
 * print message and return
 * not strerror*/

void
err_msg(const char *fmt,...)
{
	va_list ap;
	va_start(ap,fmt);
	err_doit(0,LOG_INFO,fmt,ap);
	va_end(ap);
	return;
}


//no strerror and quit
void err_quit(const char *fmt,...)
{
    va_list ap;
    va_start(ap,fmt);
    err_doit(0,LOG_ERR,fmt,ap);
    va_end(ap);
    exit(1);
}


//syslog是LINUX系统默认的守护进程。默认配置文件/etc/syslog.conf
//程序,守护进程或内核可以调用syslog让日志程序生成信息
static void err_doit(int errnoflag,int level,const char *fmt,va_list ap)
{
    int errno_save,n;
    char buf[MAXLINE+1];
    errno_save = errno;
#if HAVE_VSNPRINTF
	//将内容往Buf打 而不是stdout 这是和printf区别
    vsnprintf(buf,MAXLINE,fmt,ap);/*safe*/
	printf("test #ifdef\n");
#else
    vsprintf(buf,fmt,ap);
#endif
    n = strlen(buf);
    if(errnoflag)
        snprintf(buf+n,MAXLINE-n,":%s",strerror(errno_save));
    strcat(buf,"\n");

    if(daemon_proc)
    {
        syslog(level,buf);
    }
    else
    {
        fflush(stdout);
        fputs(buf,stderr);
        fflush(stderr);
    }
    return;

}
