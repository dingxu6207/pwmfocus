#ifndef __LED_H
#define	__LED_H


#include "stm32f10x.h"


/* ����LED���ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����Ĵ��뼴�ɸı���Ƶ�LED���� */
// R-��ɫ
#define LED1_GPIO_PORT    	GPIOB			              /* GPIO�˿� */
#define LED1_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define LED1_GPIO_PIN		GPIO_Pin_8			        /* ���ӵ�SCLʱ���ߵ�GPIO */

// G-��ɫ DIR�����ź�
#define LED2_GPIO_PORT    	GPIOA/* GPIO�˿� */
#define LED2_GPIO_CLK 	    RCC_APB2Periph_GPIOA		/* GPIO�˿�ʱ�� */
#define LED2_GPIO_PIN		GPIO_Pin_7                 /* ���ӵ�SCLʱ���ߵ�GPIO */

// B-��ɫ �����ź�
#define LED3_GPIO_PORT    	GPIOB			              /* GPIO�˿� */
#define LED3_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define LED3_GPIO_PIN		GPIO_Pin_15			        /* ���ӵ�SCLʱ���ߵ�GPIO */

// D-�ѻ�
#define LED4_GPIO_PORT    	GPIOB			              /* GPIO�˿� */
#define LED4_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define LED4_GPIO_PIN		GPIO_Pin_10			        /* ���ӵ�SCLʱ���ߵ�GPIO */

//DRV8825ʹ���ź�
#define LED5_GPIO_PORT    	GPIOA			              /* GPIO�˿� */
#define LED5_GPIO_CLK 	    RCC_APB2Periph_GPIOA		/* GPIO�˿�ʱ�� */
#define LED5_GPIO_PIN		    GPIO_Pin_4		        /* ���ӵ�SCLʱ���ߵ�GPIO */

//M0�ź� 
#define M0_GPIO_PORT        GPIOA
#define M0_GPIO_CLK         RCC_APB2Periph_GPIOA
#define M0_GPIO_PIN         GPIO_Pin_5

//M1�ź� 
#define M1_GPIO_PORT        GPIOA
#define M1_GPIO_CLK         RCC_APB2Periph_GPIOA
#define M1_GPIO_PIN         GPIO_Pin_6

//M2�ź� 
#define M2_GPIO_PORT        GPIOA
#define M2_GPIO_CLK         RCC_APB2Periph_GPIOA
#define M2_GPIO_PIN         GPIO_Pin_11

//RST�ź� 
#define RST_GPIO_PORT        GPIOA
#define RST_GPIO_CLK         RCC_APB2Periph_GPIOA
#define RST_GPIO_PIN         GPIO_Pin_12

//SLF�ź� 
#define SLF_GPIO_PORT        GPIOA
#define SLF_GPIO_CLK         RCC_APB2Periph_GPIOA
#define SLF_GPIO_PIN         GPIO_Pin_13


#define ON  0
#define OFF 1

/* ʹ�ñ�׼�Ĺ̼������IO*/
#define LED1(a)	if (a)	\
					GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED1_GPIO_PORT,LED1_GPIO_PIN)

#define LED2(a)	if (a)	\
					GPIO_SetBits(LED2_GPIO_PORT,LED2_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED2_GPIO_PORT,LED2_GPIO_PIN)

#define LED3(a)	if (a)	\
					GPIO_SetBits(LED3_GPIO_PORT,LED3_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED3_GPIO_PORT,LED3_GPIO_PIN)


/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //���Ϊ�ߵ�ƽ		
#define digitalLo(p,i)		 {p->BRR=i;}	 //����͵�ƽ
#define digitalToggle(p,i) {p->ODR ^=i;} //�����ת״̬


/* �������IO�ĺ� */
#define LED1_TOGGLE		 digitalToggle(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_OFF		   digitalHi(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_ON			   digitalLo(LED1_GPIO_PORT,LED1_GPIO_PIN)

#define LED2_TOGGLE		 digitalToggle(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_OFF		   digitalHi(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_ON			   digitalLo(LED2_GPIO_PORT,LED2_GPIO_PIN)

#define LED3_TOGGLE		 digitalToggle(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define LED3_OFF		   digitalHi(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define LED3_ON			   digitalLo(LED3_GPIO_PORT,LED3_GPIO_PIN)

/* ������ɫ������߼��÷�ʹ��PWM�ɻ��ȫ����ɫ,��Ч������ */

//��
#define LED_RED  \
					LED1_ON;\
					LED2_OFF\
					LED3_OFF

//��
#define LED_GREEN		\
					LED1_OFF;\
					LED2_ON\
					LED3_OFF

//��
#define LED_BLUE	\
					LED1_OFF;\
					LED2_OFF\
					LED3_ON

					
//��(��+��)					
#define LED_YELLOW	\
					LED1_ON;\
					LED2_ON\
					LED3_OFF
//��(��+��)
#define LED_PURPLE	\
					LED1_ON;\
					LED2_OFF\
					LED3_ON

//��(��+��)
#define LED_CYAN \
					LED1_OFF;\
					LED2_ON\
					LED3_ON
					
//��(��+��+��)
#define LED_WHITE	\
					LED1_ON;\
					LED2_ON\
					LED3_ON
					
//��(ȫ���ر�)
#define LED_RGBOFF	\
					LED1_OFF;\
					LED2_OFF\
					LED3_OFF

void LED_GPIO_Config(void);

#endif /* __LED_H */
