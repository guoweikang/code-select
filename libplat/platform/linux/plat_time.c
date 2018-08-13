
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


#if defined(LINUX_PLAT) 

#include "plat_time.h"
#include "time.h"


int  plat_gettimeofday(struct plat_time_value *val,void *timezone)
{
	return  gettimeofday((struct timeval*)val,timezone);
}



/*!\brief ƽ̨�ӿں��� ��ȡ��ǰϵͳʱ��
	* 
	* C �⺯�� struct tm *gmtime(const time_t *timer) ʹ�� timer ��ֵ����� tm �ṹ������Э������ʱ��UTC��Ҳ����Ϊ�������α�׼ʱ�䣨GMT����ʾ��
	* @param[in]   timeptr -- ����ָ���ʾ����ʱ��� time_t ֵ��ָ�롣
	* @return �ú�������ָ�� tm �ṹ��ָ�룬�ýṹ���б�����ʱ����Ϣ��
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
	return (struct plat_tm*)gmtime(timeptr);
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
struct plat_tm *plat_localtime(const TIME_T *timer){
	
return (struct plat_tm*)localtime(timer);

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
	return time(timeptr);
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
SIZE_T plat_strftime( char *str, SIZE_T max, const char *format, const struct plat_tm *tmptr){
	return strftime(str,max,format,(struct tm*)tmptr);
}




#endif  

