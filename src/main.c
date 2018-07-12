#include "library.h"
#include "setup.h"
#include "serial.h"
#include "main.h"

#define GO_LEFT		1
#define GO_RIGHT   -1
#define STAY		0

#define ANGLE_360	200
#define ANGLE_270	150
#define ANGLE_180	100
#define ANGLE_90	50

#define DIR_0		1
#define DIR_1		0

int direction = STAY;

#define DELAY_TIM_FREQUENCY_US 1000000		/* = 1MHZ -> timer runs in microseconds */
#define DELAY_TIM_FREQUENCY_MS 1000			/* = 1kHZ -> timer runs in milliseconds */

// Init timer for Microseconds delays
void _init_us() {
	// Enable clock for TIM2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

	// Time base configuration
	TIM_TimeBaseInitTypeDef TIM;
	TIM_TimeBaseStructInit(&TIM);
	TIM.TIM_Prescaler = (SystemCoreClock/DELAY_TIM_FREQUENCY_US)-1;
	TIM.TIM_Period = UINT16_MAX;
	TIM.TIM_ClockDivision = 0;
	TIM.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&TIM);

	// Enable counter for TIM2
	TIM_Cmd(TIM2,ENABLE);
}

// Init and start timer for Milliseconds delays
void _init_ms() {
	// Enable clock for TIM2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

	// Time base configuration
	TIM_TimeBaseInitTypeDef TIM;
	TIM_TimeBaseStructInit(&TIM);
	TIM.TIM_Prescaler = (SystemCoreClock/DELAY_TIM_FREQUENCY_MS)-1;
	TIM.TIM_Period = UINT16_MAX;
	TIM.TIM_ClockDivision = 0;
	TIM.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&TIM);

	// Enable counter for TIM2
	TIM_Cmd(TIM2,ENABLE);
}

// Stop timer
void _stop_timer() {
	TIM_Cmd(TIM2,DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,DISABLE); // Powersavings?
}

// Do delay for nTime milliseconds
void Delay_ms(uint32_t mSecs) {
	// Init and start timer
	_init_ms();

	// Dummy loop with 16 bit count wrap around
	volatile uint32_t start = TIM2->CNT;
	while((TIM2->CNT-start) <= mSecs);

	// Stop timer
	_stop_timer();
}

// Do delay for nTime microseconds
void Delay_us(uint32_t uSecs) {
	// Init and start timer
	_init_us();

	// Dummy loop with 16 bit count wrap around
	volatile uint32_t start = TIM2->CNT;
	while((TIM2->CNT-start) <= uSecs);

	// Stop timer
	_stop_timer();
}

// function
void Motor_Move(int direction){
	
	switch(direction){
	case GO_LEFT:
		GPIO_ResetBits(A4988_GPIO_PORT, A4988_DIR_PIN);
		Step_Move(1);
		break;
	case GO_RIGHT:
		GPIO_SetBits(A4988_GPIO_PORT, A4988_DIR_PIN);
		Step_Move(1);
		break;
	case STAY:
		GPIO_ResetBits(A4988_GPIO_PORT, A4988_STEP_PIN);
		break;
	}
}

void Step_Move(int step){
	for(int i = 0; i < step; i++){
		GPIO_SetBits(A4988_GPIO_PORT, A4988_STEP_PIN);
		Delay_us(4000);
		GPIO_ResetBits(A4988_GPIO_PORT, A4988_STEP_PIN);
		Delay_us(4000);
	}
}

int main(){
	setup();
	
	USART_SendStr("START!\r\n");
	
	while(1){
		
		if(ADC_GetConversionValue(ADC3) == 4095){
			USART_SendStr("C|");
			USART_SendInt(ADC_GetConversionValue(ADC3));
			USART_SendStr("Stop Door\r\n");
			direction = STAY;
			Motor_Move(STAY);
		}
		else if(ADC_GetConversionValue(ADC1) == 4095){
			USART_SendStr("B|");
			USART_SendInt(ADC_GetConversionValue(ADC1));
			USART_SendStr("Open Door\r\n");
			direction = GO_RIGHT;
			Motor_Move(GO_RIGHT);
		}
		else if(ADC_GetConversionValue(ADC2) == 4095){
			USART_SendStr("A|");
			USART_SendInt(ADC_GetConversionValue(ADC2));
			USART_SendStr("Close Door\r\n");
			direction = GO_LEFT;
			Motor_Move(GO_LEFT);
		}
		
		if(direction == GO_RIGHT)
			Motor_Move(GO_RIGHT);
		if(direction == GO_LEFT)
			Motor_Move(GO_LEFT);
		
		if(!GPIO_ReadInputDataBit(LIMIT_GPIO_PORT, LIMIT_SW0_PIN) && direction != STAY){
			USART_SendStr("limit0\r\n");
			Motor_Move(STAY);
		}
		if(!GPIO_ReadInputDataBit(LIMIT_GPIO_PORT, LIMIT_SW4_PIN) && direction != STAY){
			USART_SendStr("limit4\r\n");
			Motor_Move(STAY);
		}
	}
	
	return 0;
}