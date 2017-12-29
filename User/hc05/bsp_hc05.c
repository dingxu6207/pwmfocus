#include "Bsp_hc05.h"
#include "Bsp_usart_blt.h"
#include <string.h>
#include <stdio.h>


void HC05_Init(void)
{
	BLT_USART_Config();		
}

 /**
  * @brief  ��stream�л�ȡһ���ַ�����line��
	* @param  line,�洢����е��ַ�������
	* @param  stream��ԭ�ַ���������
	* @param 	max_size��stream�Ĵ�С 
  * @retval line�ĳ��ȣ���stream��û�С�\0����'\r'��'\n'���򷵻�0
  */
int get_line(char* line, char* stream ,int max_size)  
{  
		char *p;	
        int len = 0;  
		
		p=stream;
        while( *p != '\0' && len < max_size )
		{  
        	line[len++] = *p;  
			p++;
			
        	if('\n' == *p || '\r'==*p)  
            break;  
        }
	
		if(*p != '\0' && *p != '\n' && *p != '\r')
			return 0;
  
      
    	line[len] = '\0';  
    	return len;  
} 

void HC05_SendString(char* str)
{
	
	Usart_SendString(USART2,(uint8_t *)str);

}


