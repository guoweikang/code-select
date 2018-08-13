
/** 
	* @file 		plat_stdio.c 
	* @brief		系统库包装层 包装 标准io
	* @details		
	* @author		郭伟康
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   中移物联网有限公司 
	* @par History: 		 
	*	V1.0: 郭伟康,  2018/7/10 , 第一次创建\n 
*/	











#if defined(LINUX_PLAT) 

#include "plat_stdio.h"
#include <ctype.h>

#include <stdarg.h>


int plat_printf(const char* format,...)
{

	int size = 0 ;
	va_list ap;
	
	va_start(ap, format);
	
	size = vprintf(format, ap);
	
	va_end(ap);

	return size;

}




int plat_fprintf(PLAT_FILE *stream,const char* format,...)
{

	int size = 0 ;

	va_list ap;
	
	va_start(ap, format);
	
	size = vfprintf(stream,format, ap);
	
	va_end(ap);
	
	return size;
}



int plat_sprintf(char *str,const char* format,...)
{
	int size = 0 ;

	va_list ap;
	
	va_start(ap, format);
	
	size = vsprintf(str,format, ap);
	
	va_end(ap);
	return size;

}





int plat_snprintf(char *str,SIZE_T size,const char* format,...)
{
		int nsize = 0 ;
	
		va_list ap;
		
		va_start(ap, format);
		
		nsize = vsnprintf(str,size,format, ap);
		
		va_end(ap);
		return nsize;

}



int plat_sscanf(const char  *str,const char* format,...)
{
	int size = 0 ;
	
	va_list ap;
	
	va_start(ap, format);
	
	size = vsscanf(str,format, ap);
	
	va_end(ap);
	return size;

	
}




int plat_isprint(int c )
{
	return isprint(c);
}



int plat_isspace(int c )
{
	return isspace(c);
}



int plat_isalnum(int c )
{
	return isalnum(c);
}


int plat_toupper(int c )
{
	return toupper(c);

}

int plat_tolower(int c )
{
	
	return tolower(c);
}


PLAT_FILE *plat_fopen(const char *path,const char *mode)
{
	return fopen(path,mode);
}




int plat_fclose(PLAT_FILE *stream)
{
	return fclose(stream);
}




int plat_feof(PLAT_FILE *stream)
{
	return feof(stream);
}



SIZE_T plat_fread(void *ptr,SIZE_T size,SIZE_T nmemb, PLAT_FILE *stream)
{
	return fread(ptr,size,nmemb,stream);
}



#endif 


