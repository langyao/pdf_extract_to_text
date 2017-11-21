#ifndef _ERROR_HANDLE_
#define _ERROR_HANDLE_

extern void err_ret(const char *fmt,...);
extern void err_sys(const char *fmt,...);
extern void err_abort(const char *fmt,...);
extern void err_msg(const char *fmt,...);
extern void err_quit(const char *fmt,...);

#endif
