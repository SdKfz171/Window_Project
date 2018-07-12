#include <stdio.h>
#include "library.h"
#include "setup.h"
#include "delay.h"

#define DIGITAL	0
#define ANALOG	1

static void GPIO_setting(void);
static void ADC_setting(void);
static void SysTick_setting(void);
static void Serial_Init(uint32_t baudRate);

void setup(void)
{
	GPIO_setting();
	ADC_setting();
	SysTick_setting();
	
	Serial_Init(115200);
	
	DelayMillis(3);

#ifdef USE_SLEEP
	CoreDebug->DHCSR |= 0x7;
#endif
}

static void GPIO_setting(void)
{
	RCC_ADCCLKConfig(RCC_PCLK2_Div2);
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;
	RCC->APB2ENR |= RCC_APB2ENR_ADC3EN;
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	
	
	GPIO_InitTypeDef GPIO_InitStructure;

	// LED
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	// USART1 TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART1_GPIO_PORT, &GPIO_InitStructure);
	
	// USART1 RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART1_GPIO_PORT, &GPIO_InitStructure);
	
	// A4988 STEP
	GPIO_InitStructure.GPIO_Pin   = A4988_STEP_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(A4988_GPIO_PORT, &GPIO_InitStructure);
	
	// A4988 DIR
	GPIO_InitStructure.GPIO_Pin   = A4988_DIR_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(A4988_GPIO_PORT, &GPIO_InitStructure);
	
	// LIMIT SW0
	GPIO_InitStructure.GPIO_Pin   = LIMIT_SW0_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LIMIT_GPIO_PORT, &GPIO_InitStructure);
	
	// LIMIT SW4
	GPIO_InitStructure.GPIO_Pin   = LIMIT_SW4_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LIMIT_GPIO_PORT, &GPIO_InitStructure);
	
	#if DIGITAL
	// RADIO D0
	GPIO_InitStructure.GPIO_Pin   = RADIO_D0_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RADIO_GPIO_PORT, &GPIO_InitStructure);
	
	// RADIO D1
	GPIO_InitStructure.GPIO_Pin   = RADIO_D1_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RADIO_GPIO_PORT, &GPIO_InitStructure);

	// RADIO D2
	GPIO_InitStructure.GPIO_Pin   = RADIO_D2_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RADIO_GPIO_PORT, &GPIO_InitStructure);
	
	// RADIO D3
	GPIO_InitStructure.GPIO_Pin   = RADIO_D3_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RADIO_GPIO_PORT, &GPIO_InitStructure);
	#endif
	
	#if ANALOG
	// RADIO D0
	GPIO_InitStructure.GPIO_Pin   = RADIO_D0_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RADIO_GPIO_PORT, &GPIO_InitStructure);
	
	// RADIO D1
	GPIO_InitStructure.GPIO_Pin   = RADIO_D1_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RADIO_GPIO_PORT, &GPIO_InitStructure);

	// RADIO D2
	GPIO_InitStructure.GPIO_Pin   = RADIO_D2_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RADIO_GPIO_PORT, &GPIO_InitStructure);
	
	// RADIO D3
	GPIO_InitStructure.GPIO_Pin   = RADIO_D3_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RADIO_GPIO_PORT, &GPIO_InitStructure);
	#endif
}

static void ADC_setting(void){
	ADC_InitTypeDef ADC_InitStructure;
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_Init(ADC2, &ADC_InitStructure);
	ADC_Init(ADC3, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC2, ADC_Channel_13, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC3, ADC_Channel_12, 1, ADC_SampleTime_55Cycles5);
   
	ADC_Cmd(ADC1, ENABLE);
	ADC_ResetCalibration(ADC1);   
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);     
	while(ADC_GetCalibrationStatus(ADC1));
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);    
	
	ADC_Cmd(ADC2, ENABLE);
	ADC_ResetCalibration(ADC2);   
	while(ADC_GetResetCalibrationStatus(ADC2));
	ADC_StartCalibration(ADC2);     
	while(ADC_GetCalibrationStatus(ADC2));
	ADC_SoftwareStartConvCmd(ADC2, ENABLE);    
	
	ADC_Cmd(ADC3, ENABLE);
	ADC_ResetCalibration(ADC3);   
	while(ADC_GetResetCalibrationStatus(ADC3));
	ADC_StartCalibration(ADC3);     
	while(ADC_GetCalibrationStatus(ADC3));
	ADC_SoftwareStartConvCmd(ADC3, ENABLE);  
}

static void SysTick_setting(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	NVIC_SetPriority(SysTick_IRQn, 0);
}

static void Serial_Init(uint32_t baudRate){
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_Clock_InitStructure;
	
	USART_Clock_InitStructure.USART_Clock = USART_Clock_Disable;
	USART_Clock_InitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_Clock_InitStructure.USART_CPOL = USART_CPOL_Low;
	USART_Clock_InitStructure.USART_LastBit = USART_LastBit_Disable;
	USART_ClockInit(USART1, &USART_Clock_InitStructure);
	
	USART_InitStructure.USART_BaudRate = baudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 
	USART_Init(USART1, &USART_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
}