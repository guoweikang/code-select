/** 
	* @file 		platform.c
	* @brief		
	* @details		
	* @author		郭伟康
	* @date 	2018/7/12
	* @version	V1.0 
	* @par Copyright (c):  
	*	   中移物联网有限公司 
	* @par History: 		 
	*	V1.0: 郭伟康,  2018/7/12 , 第一次创建\n 
*/	

#include <liblwm2m.h>
#include "plat_stdlib.h"
#include "plat_string.h"
#include "plat_stdio.h"

#include <stdarg.h>


#ifndef LWM2M_MEMORY_TRACE

void * lwm2m_malloc(SIZE_T s)
{
    return plat_malloc(s);
}

void lwm2m_free(void * p)
{
    return plat_free(p);
}

char * lwm2m_strdup(const char * str)
{
    return strdup(str);
}

#endif

int lwm2m_strncmp(const char * s1,
                     const char * s2,
                     SIZE_T n)
{
    return strncmp(s1, s2, n);
}

TIME_T lwm2m_gettime(void)
{
    struct plat_time_value  tv;

    if (0 !=  plat_gettimeofday(&tv, NULL))
    {
        return -1;
    }

    return tv.sec;
}

void lwm2m_printf(const char * format, ...)
{
	char logbuf[1024];
    va_list ap;

    va_start(ap, format);

    vsprintf(logbuf, format, ap);

    va_end(ap);
	plat_fprintf(PLAT_STDERR,logbuf);
}

