
/** 
	* @file 		plat_time.c 
	* @brief		ϵͳ���װ�� ��װ time
	* @details		
	* @author		��ΰ��
	* @date 	2018/7/10
	* @version	V1.0 
	* @par Copyright (c):  
	*	   �������������޹�˾ 
	* @par History: 		 
	*	V1.0: ��ΰ��,  2018/7/10 , ��һ�δ���\n 
*/	

#include "plat_time.h"
#include "tm.h"

/*!\brief ƽ̨�ӿں��� ��ȡ��ǰϵͳʱ��
	* 
	* ƽ̨ͳһ��׼�ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[out]   val: �洢ϵͳʱ��
	* @param[in]    timezone:ʱ�� ������ʹ�� ��NULL
	* @return  �����
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
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
/*!\brief ƽ̨�ӿں��� ��ȡ��ǰϵͳʱ��
	* 
	* ƽ̨ͳһ��׼�ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[out]   val: �洢ϵͳʱ��
	* @param[in]    timezone:ʱ�� ������ʹ�� ��NULL
	* @return  �����
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
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

/*!\brief ƽ̨�ӿں��� ��ȡ��ǰϵͳʱ��
	* 
	* C �⺯�� struct tm *localtime(const time_t *timer) ʹ�� timer ��ֵ����� tm �ṹ��timer ��ֵ���ֽ�Ϊ tm �ṹ�����ñ���ʱ����ʾ
	* @param[in]  timer -- ����ָ���ʾ����ʱ��� time_t ֵ��ָ�롣
	* @return �ú�������ָ�� tm �ṹ��ָ�룬�ýṹ���б�����ʱ����Ϣ�������� tm �ṹ��ϸ�ڣ�
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see 
*/
struct plat_tm *plat_localtime(const TIME_T *timer)
{
	return plat_gmtime(timer);
}

/*!\brief ƽ̨�ӿں��� ��ȡ��ǰϵͳʱ��
	* 
	* C �⺯�� time_t time(time_t *seconds) �����Լ�Ԫ Epoch��1970-01-01 00:00:00 UTC���𾭹���ʱ�䣬����Ϊ��λ����� seconds ��Ϊ�գ��򷵻�ֵҲ�洢�ڱ��� seconds �С�
	* @param[in]   timeptr:  ����ָ������Ϊ time_t �Ķ����ָ�룬�����洢 seconds ��ֵ��
	* @return  time_t ���󷵻ص�ǰ����ʱ�䡣
	* @par ����   
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
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

/*!\brief ƽ̨�ӿں��� ��ȡ��ǰϵͳʱ��
	* 
	* ƽ̨ͳһ��׼�ӿڣ�����ƽ̨�ڲ�����ʵ��
	* @param[out]  str  ����ָ��Ŀ�������ָ�룬�������Ʋ����� C �ַ���
	* @param[in] max -- ���Ǳ����Ƶ� str ������ַ�����
	* @param[in] format -- ���� C �ַ�������������ͨ�ַ��������ʽ˵�������κ���ϡ���Щ��ʽ˵�����ɺ����滻Ϊ��ʾ tm ����ָ��ʱ������Ӧֵ����ʽ˵�����ǣ�
	* @param[in]timeptr -- ����ָ�� tm �ṹ��ָ�룬�ýṹ������һ���·ֽ�Ϊ���¸����ֵ�����ʱ��
	* @return  ��������� C �ַ���С�� size ���ַ��������ս����ַ�������᷵�ظ��Ƶ� str �е��ַ��������������ս����ַ��������򷵻��㡣
	* @par ����   
	*	    
	* @par ���� 
	*	   �� 
	* @par �޸���־ 
	*
	* @see 
*/
SIZE_T plat_strftime( char *str, SIZE_T max, const char *format, const struct plat_tm *tmptr)
{







}

