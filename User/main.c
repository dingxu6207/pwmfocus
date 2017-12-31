/**
  ******************************************************************************
  * @file    main.c
  * @author  DINGXU
  * @version V1.0
  * @date    2017-11-20������
  * @brief   ������
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"
#include "bsp_hc05.h"
#include "bsp_usart_blt.h"
#include <stdio.h>
#include <stdlib.h>
#include "bsp_ds18b20.h"
#include <string.h>
#include "bsp_TiMbase.h"
#include "stdbool.h"

extern u8 flagrun;
extern u8 blurunflag;
bool brightleft = false;
char linebuff[1024];
//char *ReplyBuff = "S2Focuser#";
char ReplyBuff[256] = {0};
char *pEquipment = "S2Focuser#";
volatile u32 time = 0; // ms ��ʱ���� 
//u16 uSpeedCmd[10] = {512,256,128,64,32,16,8,4,2,1};
char temperature[10] = {0};   //�����¶�
char statecount[10] = {0};    //����״̬����
char stepcount[10] = {0};     //��������
bool bCount = false;
bool bCmdequ = false;

char buffer[10];    				//�豸��
u8 uSpeedIndex;     				//�ٶ�����
//u16 uSpeed = uSpeedCmd[9];		    //�ٶ�ֵ
u16 uSpeed = 256;		    //�ٶ�ֵ
bool bStateflag = true;	            //����״̬
bool bStateCount = true;            //״̬������־λ
int uStepCount = 0;                 //���в���
int uCmdstep = 0;                   //�����
int iStepCount = 0;
u8 Mode = 0;

//extern bool IoState;

void SetMode(int sMode);

void SetMode(int sMode)
{
	switch(sMode)
	{
		case 0:
		{
			/* M0=0 */
			GPIO_ResetBits(M0_GPIO_PORT, M0_GPIO_PIN);
			/* M1=0 */
			GPIO_ResetBits(M1_GPIO_PORT, M1_GPIO_PIN);
			/* M2=0 */
			GPIO_ResetBits(M2_GPIO_PORT, M2_GPIO_PIN);
		}
		break;
		
		case 1:
		{
			/* M0=1 */
			GPIO_SetBits(M0_GPIO_PORT, M0_GPIO_PIN);
			/* M1=0 */
			GPIO_ResetBits(M1_GPIO_PORT, M1_GPIO_PIN);
			/* M2=0 */
			GPIO_ResetBits(M2_GPIO_PORT, M2_GPIO_PIN);	
		}
		break;
		
		case 2:
		{
			/* M0=0 */
			GPIO_ResetBits(M0_GPIO_PORT, M0_GPIO_PIN);
			/* M1=1 */
			GPIO_SetBits(M1_GPIO_PORT, M1_GPIO_PIN);
			/* M2=0 */
			GPIO_ResetBits(M2_GPIO_PORT, M2_GPIO_PIN);	
		}
		break;
		
		case 3:
		{
			/* M0=1 */
			GPIO_SetBits(M0_GPIO_PORT, M0_GPIO_PIN);	   
			/* M1=1 */
			GPIO_SetBits(M1_GPIO_PORT, M1_GPIO_PIN);
			/* M2=0 */
			GPIO_ResetBits(M2_GPIO_PORT, M2_GPIO_PIN);	
		}
		break;
		
		case 4:
		{
			/* M0=0 */
			GPIO_ResetBits(M0_GPIO_PORT, M0_GPIO_PIN);	   
			/* M1=0 */
			GPIO_ResetBits(M1_GPIO_PORT, M1_GPIO_PIN);
			/* M2=1 */
			GPIO_SetBits(M2_GPIO_PORT, M2_GPIO_PIN);				
		}
		break;
		
		case 5:
		{
			/* M0=1 */
			GPIO_SetBits(M0_GPIO_PORT, M0_GPIO_PIN);   
			/* M1=1 */
			GPIO_SetBits(M1_GPIO_PORT, M1_GPIO_PIN);
			/* M2=1 */
			GPIO_SetBits(M2_GPIO_PORT, M2_GPIO_PIN);
		}
		break;
	}
}

void CmdProcess(unsigned char *RxBuffer);
void CmdProcess(unsigned char *RxBuffer)
{
    unsigned char ucTemp;
	ucTemp = *(RxBuffer+2);
	
	if (*(RxBuffer+0) == ':')
	{
			if (*(RxBuffer+1) == 'F')
			{
				switch (ucTemp)
				{
					case '?':
					        {
								sprintf(ReplyBuff, ":?%s\n", pEquipment);

								break;
					        }

					case 't':
						    {
								sprintf(ReplyBuff,":t%.2f#\n", DS18B20_Get_Temp());//����2λС��

								break;
					        }
					
					case 'B':
						    {
								sprintf(ReplyBuff,":B%d#\n", !bStateflag);

								break;
					        }
          case 'p':
				   			{
								if (uStepCount >= 0)
									sprintf(ReplyBuff,":p+%d*%d\n", uStepCount,!bStateflag);//ת��״̬Ҳ����λ������ĩβ����:F+10000*1#
								else
									sprintf(ReplyBuff,":p%d*%d#\n", uStepCount,!bStateflag);
								
								break;
				   			}
				   
				   case 'D':   				//:FD�*�*�*# 
							{
								strcpy(buffer, (char *)RxBuffer+3);
								pEquipment = buffer;						
								sprintf(ReplyBuff,":FD%s\n", pEquipment);
								
								break;
							}
				   
				   case '-': 				//:F-#  
				            {
								if (brightleft == false)
								{
									GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);   //dir�ź�
								}
								else
								{
									GPIO_ResetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);   //dir�ź�
								}
								ControlMotor(ENABLE);
								bStateflag = false;
								bStateCount = false;
								sprintf(ReplyBuff,":-#\n");

								break;
				            }
				   
				   case '+': 				//:F+#    
				            {
								if (brightleft == false)
								{
									GPIO_ResetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);   //dir�ź�
								}
								else
								{
									GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);   //dir�ź�
								}						
								ControlMotor(ENABLE);
								bStateflag = false;
								bStateCount = true;
								sprintf(ReplyBuff,":+#\n");

								break;
				            }
				   case 'Q': 				//:FQ#    
							{
								ControlMotor(DISABLE);
								bStateflag = true;
								bStateCount = true;
								sprintf(ReplyBuff,":Q#\n");

								break;
							}
				  case 'R': 				//:FR#    
							{
								ControlMotor(DISABLE);
								brightleft = !brightleft;                                  //�����ź�
								bStateflag = true;	
								bStateCount = true;
								sprintf(ReplyBuff,":R#\n");

							break;
							}
				  
				case 'S':       //:FS+01000#;
						{
							if (RxBuffer[3] == '+')
							{
								uStepCount = atoi((char const *)RxBuffer+4);
								ControlMotor(DISABLE);
								bStateflag = true;
								bStateCount = true;
								sprintf(ReplyBuff,":S#\n");
							}
							
								break;							
						}
							
				case 'M':     //:FM1#
						{
							Mode = atoi((char const *)RxBuffer+3);
							SetMode(Mode);
							sprintf(ReplyBuff,":M#\n");

							break;
						}
				
				case 'P':       //:FPsxxxx#
						{
							if (RxBuffer[3] == '+')							
							{
								uCmdstep = atoi((char const *)RxBuffer+4);
							}
							
							if (RxBuffer[3] == '-')
							{
								uCmdstep = atoi((char const *)RxBuffer+4);
								uCmdstep = -uCmdstep;
							}
							
							iStepCount = uCmdstep - uStepCount;
							if (iStepCount > 0)
							{
								ControlMotor(ENABLE);							
								if (brightleft == false)
									GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN); //DIR
								else
									GPIO_ResetBits(LED2_GPIO_PORT, LED2_GPIO_PIN); //DIR								
								bStateflag = false;
								bStateCount = false;							
							}
							else
							{
								ControlMotor(ENABLE);
								if (brightleft == false)
									GPIO_ResetBits(LED2_GPIO_PORT, LED2_GPIO_PIN); //DIR
								else
									GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN); //DIR							
								bStateflag = false;
								bStateCount = true;
							}
							bCmdequ = true;
							sprintf(ReplyBuff,":P#\n");
							
							break;
						}
				
				case 'V': //�޸Ĺ��ٶȵ������//:FV256#
						{
							uSpeed = atoi((char const *)RxBuffer+3);
							//����uSpeed��Χ��1-1024
							#if 0
							if(uSpeed<1) 
								uSpeed = 1;
							else if(uSpeed>1024)
								uSpeed = 1024;
							#endif
							//printf("uSpeed = %d\n" ,uSpeed);
							
							SetSpeedMoter(uSpeed);
							
							sprintf(ReplyBuff, ":V#\n");
							
							break;
				       }
				

				  
				default:
						    {
								printf("there is wrong input!\n");

								break;
					        }
				}
			}
	}
	
	RxBuffer = NULL;
}


#if 0

#endif

int main()
{	

	
	LED_GPIO_Config();

	SysTick_Init();
	
	USART_Config();

	HC05_Init();

	BASIC_TIM_Init();
	
	if ( DS18B20_Init())	
	{
		printf("\r\n no ds18b20 exit \r\n");
	}	
	printf("\r\n ds18b20 exit \r\n");

	while(1)
	{	
		//������1����
		if (flagrun == 1)
		{
 
			CmdProcess(UART_RxBuffer);
			//printf("%s", ReplyBuff);
			printf("%s\n", ReplyBuff);
			flagrun = 0;
			uart_FlushRxBuffer();
			ReplyBuff[0] = '\0';
			
		}	
			
	  //������2����	
	  if (blurunflag == 1)
	  {
      
			CmdProcess(uart_buff);
			HC05_SendString(ReplyBuff);
			blurunflag = 0;
			clean_rebuff();
			ReplyBuff[0] = '\0';
	  }

	  if ((((iStepCount>0)&&(uStepCount >= uCmdstep))||((iStepCount<0)&&(uStepCount <= uCmdstep))) && (bCmdequ == true))
	  {
			ControlMotor(DISABLE);

			bStateflag = true;

			bStateCount = true;	

			bCmdequ = false;
	  } 	

	  
		
	}
}



/*********************************************END OF FILE**********************/
