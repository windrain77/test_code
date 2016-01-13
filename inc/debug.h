#ifndef __DEBUG_H__
#define __DEBUG_H__


#ifdef _cplusplus
extern "C" {
#endif


#define _MINUX_DEBUG
#ifdef _MINUX_DEBUG
//	#undef __func__
//	#define __func__ __PRETTY_FUNCTION__

	#include <string.h>
	#define __file__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

	#include <stdio.h>
	#include <stdarg.h>
	inline static void dbg_print(const char *fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		vfprintf(stderr, fmt, args);
		va_end(args);
	}

	#define dp(fmt, ...) do{fprintf(stderr, fmt, ##__VA_ARGS__);}while(0)

	#define _ENABLE_TIME_STAMP 
	#ifdef _ENABLE_TIME_STAMP
		#include <time.h>
		#include <sys/time.h>
		#include <string.h>
		inline static char *TimeLog(void)
		{
			static char timeStrBuf[32]={0,};
			struct timeval tv;
			struct tm tt;
			
			gettimeofday(&tv, NULL);
			localtime_r(&tv.tv_sec, &tt);

			memset(timeStrBuf, 0x00, 32);
			strftime(timeStrBuf, 32, "%H:%M:%S", &tt);
			snprintf(timeStrBuf+8, 5, ".%03ld", tv.tv_usec / 1000);
			
			return timeStrBuf;
		}

		#define dlp(fmt, ...) \
				do{\
					dbg_print( "\e[33m> %s \e[0;45m[DEBUG]\e[0;34m[%s]\e[32m%5d:\e[36m[%s]\e[m "fmt, \
									TimeLog(), __file__, __LINE__, __func__, ##__VA_ARGS__);\
				}while(0)
				
		#define dlperr(fmt, ...) \
				do{\
					dbg_print( "\e[0;1;3;41m> %s [ERROR][%s]%05d:[%s]\e[m "fmt,\
									TimeLog(), __file__, __LINE__, __func__, ##__VA_ARGS__);\
				}while(0) 
	#else 
		#define dlp(fmt, ...) \
				do{\
					dbg_print( "\e[33m> \e[0;45m[DEBUG]\e[0;34m[%s]\e32m%5d:\e[36m[%s]:\e[m "fmt,\
									__file__, __LINE__, __func__, ##__VA_ARGS__);\
				}while(0)
				
		#define dlperr(fmt, ...) \
				do{\
					dbg_print( "\e[0;1;3;41m> [ERROR][%s]%05d:[%s]:\e[m "fmt,\
									__file__, __LINE__, __func__, ##__VA_ARGS__);\
				}while(0) 
	#endif

	#define D_START    {dlp("\e[3;4;35m[START]                                                  \e[m\n");}
	#define D_END      {dlp("\e[3;4;35m[E N D]                                                  \e[m\n");}
	#define D_LOCATION {\
							dp("\e[0;7m> %s [LOCATE] \e[3;4m%05d.\e[1m%s()\e[0;7m@%s \e[m\n", \
							TimeLog(), __LINE__,  __func__, __file__);\
					   }
	#define D_EXIT     {dlp("\e[1;41m >>>>> E X I T <<<<< \e[m\n"); exit(EXIT_FAILURE);}
#else // _MINUX_DEBUG
	#define dp(fmt, ...)
	#define dlp(fmt, ...)
	#define dlperr(fmt, ...)
	#define D_START
	#define D_END  
	#define D_LOCATION
	#define D_EXIT
#endif // _MINUX_DEBUG


#ifdef _cplusplus
}
#endif


#endif // __DEBUG_H__
