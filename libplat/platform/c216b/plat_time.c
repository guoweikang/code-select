
/** 
	* @file 		plat_time.c 
	* @brief		系统库包装层 包装 time
	* @details		
	* @author		郭伟康
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   中移物联网有限公司 
	* @par History: 		 
	*	V1.0: 郭伟康,  2018/7/10 , 第一次创建\n 
*/	

#include "plat_time.h"
#include "tm.h"

/*!\brief 平台接口函数 获取当前系统时间
	* 
	* 平台统一标准接口，各个平台内部各自实现
	* @param[out]   val: 存储系统时间
	* @param[in]    timezone:时区 保留不使用 传NULL
	* @return  随机数
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/
int  plat_gettimeofday(struct plat_time_value *val,void *timezone)
{
	UINT32 ret;

	hal_HstSendEvent(0xffffffff);
	hal_HstSendEvent(0x40000001);
	hal_HstSendEvent(0xffffffff);

	if (val == NULL)
	{
		return -1;
	}

	ret = TM_GetTime();

	if (ret != 0)
	{
		val->sec = (long)ret;
		val->u_sec = (long)(ret * 1000);
		return 0;
	}
	else
	{
		return -1;
	}
}

static struct plat_tm gm_tm;
/*!\brief 平台接口函数 获取当前系统时间
	* 
	* 平台统一标准接口，各个平台内部各自实现
	* @param[out]   val: 存储系统时间
	* @param[in]    timezone:时区 保留不使用 传NULL
	* @return  随机数
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/
struct plat_tm *plat_gmtime(const TIME_T *timeptr)
{
	TM_FILETIME temp;

	TM_SYSTEMTIME Local_time;

	temp.DateTime = *timeptr;
	
	plat_printf(">>>>>plat_gmtime temp.DateTime:%d<<<<<\n",temp.DateTime);

	TM_FileTimeToSystemTime(temp, &Local_time);

	plat_printf(">>>>>plat_gmtime Local_time:Y%02d,M%02d,D%02d,H%d,M%02d,S%02d,W%02d<<<<<\n",Local_time.uYear,Local_time.uMonth,Local_time.uDay,Local_time.uHour,Local_time.uMinute,Local_time.uSecond,Local_time.uDayOfWeek);

	gm_tm.tm_sec = Local_time.uSecond;
	gm_tm.tm_min = Local_time.uMinute;
	gm_tm.tm_hour = Local_time.uHour;
	gm_tm.tm_mday= Local_time.uDay;
	gm_tm.tm_mon= Local_time.uMonth;
	gm_tm.tm_year= Local_time.uYear;
	gm_tm.tm_wday= Local_time.uDayOfWeek;
	gm_tm.tm_yday= 0;
	gm_tm.tm_isdst= 0;

	return (&gm_tm);
}

/*!\brief 平台接口函数 获取当前系统时间
	* 
	* C 库函数 struct tm *localtime(const time_t *timer) 使用 timer 的值来填充 tm 结构。timer 的值被分解为 tm 结构，并用本地时区表示
	* @param[in]  timer -- 这是指向表示日历时间的 time_t 值的指针。
	* @return 该函数返回指向 tm 结构的指针，该结构带有被填充的时间信息。下面是 tm 结构的细节：
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/
struct plat_tm *plat_localtime(const TIME_T *timer)
{
	return plat_gmtime(timer);
}

/*!\brief 平台接口函数 获取当前系统时间
	* 
	* C 库函数 time_t time(time_t *seconds) 返回自纪元 Epoch（1970-01-01 00:00:00 UTC）起经过的时间，以秒为单位。如果 seconds 不为空，则返回值也存储在变量 seconds 中。
	* @param[in]   timeptr:  这是指向类型为 time_t 的对象的指针，用来存储 seconds 的值。
	* @return  time_t 对象返回当前日历时间。
	* @par 保留   
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/
TIME_T plat_time( TIME_T *timeptr)
{
	struct plat_time_value timep;

	plat_gettimeofday(&timep, NULL);

	*timeptr = timep.sec;

	return timep.sec;
}

/*!\brief 平台接口函数 获取当前系统时间
	* 
	* 平台统一标准接口，各个平台内部各自实现
	* @param[out]  str  这是指向目标数组的指针，用来复制产生的 C 字符串
	* @param[in] max -- 这是被复制到 str 的最大字符数。
	* @param[in] format -- 这是 C 字符串，包含了普通字符和特殊格式说明符的任何组合。这些格式说明符由函数替换为表示 tm 中所指定时间的相对应值。格式说明符是：
	* @param[in]timeptr -- 这是指向 tm 结构的指针，该结构包含了一个呗分解为以下各部分的日历时间
	* @return  如果产生的 C 字符串小于 size 个字符（包括空结束字符），则会返回复制到 str 中的字符总数（不包括空结束字符），否则返回零。
	* @par 保留   
	*	    
	* @par 其它 
	*	   无 
	* @par 修改日志 
	*
	* @see 
*/
SIZE_T plat_strftime( char *str, SIZE_T max, const char *format, const struct plat_tm *tmptr)
{







}

