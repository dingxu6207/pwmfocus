
// ������ʱ��TIMx,x[6,7]��ʱ��ʼ������

#include "bsp_TiMbase.h" 
#include "bsp_usart.h"
#include "bsp_led.h"

static void BASIC_TIM_NVIC_Config(void);
static void BASIC_TIM_Mode_Config(void);
static void ADVANCE_TIM_GPIO_Config(void);


static void ADVANCE_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// ����Ƚ�ͨ�� GPIO ��ʼ��
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_CH1_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = ADVANCE_TIM_CH1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(ADVANCE_TIM_CH1_PORT, &GPIO_InitStructure);
}
	

// �ж����ȼ�����
static void BASIC_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQ ;	
		// ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 *typedef struct
 *{ TIM_Prescaler            ����
 *	TIM_CounterMode			     TIMx,x[6,7]û�У���������
 *  TIM_Period               ����
 *  TIM_ClockDivision        TIMx,x[6,7]û�У���������
 *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]����
 *}TIM_TimeBaseInitTypeDef; 
 *-----------------------------------------------------------------------------
 */


static void BASIC_TIM_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;   //ʱ���ṹ��
    TIM_OCInitTypeDef  TIM_OCInitStructure;           //����ȽϽṹ���ʼ��

	//ʱ���ṹ���ʼ��
	//TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
		
    // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
    BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, ENABLE);
	
    // �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
    TIM_TimeBaseStructure.TIM_Period = BASIC_TIM_Period;	

	// ʱ��Ԥ��Ƶ��Ϊ
    TIM_TimeBaseStructure.TIM_Prescaler = BASIC_TIM_Prescaler;
	
    // ʱ�ӷ�Ƶ���� ��������ʱ��û�У����ù�
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		
	// ����������ģʽ��������ʱ��ֻ�����ϼ�����û�м���ģʽ������
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
		
	// �ظ���������ֵ��������ʱ��û�У����ù�
	//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	// ��ʼ����ʱ��
    TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);


	/*--------------------����ȽϽṹ���ʼ��-------------------*/	
	// ����ΪPWMģʽ2
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	// ���ʹ��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// �����������
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	// ����ռ�ձȴ�С
	TIM_OCInitStructure.TIM_Pulse = BASIC_TIM_Period/2;
	// ���ͨ����ƽ��������
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	// ���ͨ�����е�ƽ��������
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;

	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	//ʹ��TIM1_CH1Ԥװ�ؼĴ���
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	//ʹ��TIM1Ԥװ�ؼĴ���
    TIM_ARRPreloadConfig(TIM1, ENABLE); 
	//�����ж�Դ��ֻ�����ʱ���ж�
    TIM_UpdateRequestConfig(TIM1, TIM_UpdateSource_Regular);
		
		// ����������жϱ�־λ
    TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);
	  
		// �����������ж�
    TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);
		
		// ʹ�ܼ�����
    TIM_Cmd(BASIC_TIM, DISABLE);	

	  TIM_CtrlPWMOutputs(TIM1, DISABLE);
}

void BASIC_TIM_Init(void)
{
		ADVANCE_TIM_GPIO_Config();
		BASIC_TIM_NVIC_Config();
		BASIC_TIM_Mode_Config();		
}

void ControlMotor(FunctionalState NewState)
{
    
	if (NewState == DISABLE)
	{
		//ʹ�ܶ�ʱ��	      
    	TIM_Cmd(BASIC_TIM, DISABLE);
		TIM_CtrlPWMOutputs(BASIC_TIM, DISABLE); //��ֹ���
		GPIO_SetBits(LED5_GPIO_PORT, LED5_GPIO_PIN); //DRV8825ʹ���ź�
		printf("TIM is disable!\n");
	}
	else
	{
		//ʹ�ܶ�ʱ��	      
    	TIM_Cmd(BASIC_TIM, ENABLE);
		printf("TIM is able!\n");
		TIM_CtrlPWMOutputs(BASIC_TIM, ENABLE); //���	
		GPIO_ResetBits(LED5_GPIO_PORT, LED5_GPIO_PIN); //DRV8825ʹ���ź�
	}
}


void SetSpeedMoter(u16 SpeedMoter)
{
	//���ö�ʱ����װֵ	
    TIM_SetAutoreload(BASIC_TIM, SpeedMoter);
    //����ռ�ձ�Ϊ50%	
    TIM_SetCompare1(BASIC_TIM, SpeedMoter>>1);
}
/*********************************************END OF FILE**********************/
