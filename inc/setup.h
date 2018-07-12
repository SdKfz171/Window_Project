#include <stm32f10x_gpio.h>
#include <stm32f10x_spi.h>

#define SYS_FREQ	72000000
#define I2C_CLOCK   50000

void setup(void);

#define _Baud_Rate 			115200

#define LED_GPIO_PORT		GPIOD
#define LED_PIN				GPIO_Pin_2

#define USART1_GPIO_PORT	GPIOA
#define USART1_TX 			GPIO_Pin_9
#define USART1_RX			GPIO_Pin_10

#define nRF24_GPIO_PORT		GPIOB
#define nRF24_IRQ_PIN   	GPIO_Pin_10
#define nRF24_CE_PIN    	GPIO_Pin_11
#define nRF24_CSN_PIN   	GPIO_Pin_12
#define nRF24_SCK_PIN		GPIO_Pin_13
#define nRF24_MISO_PIN		GPIO_Pin_14
#define nRF24_MOSI_PIN		GPIO_Pin_15

#define nRF24_CE_L()        GPIO_ResetBits(GPIOB, nRF24_CE_PIN)
#define nRF24_CE_H()        GPIO_SetBits(GPIOB, nRF24_CE_PIN)
#define nRF24_CSN_L()       GPIO_ResetBits(GPIOB, nRF24_CSN_PIN)
#define nRF24_CSN_H()       GPIO_SetBits(GPIOB, nRF24_CSN_PIN)

#define DHT11_GPIO_PORT		GPIOA
#define DHT11_OUTPUT_PIN	GPIO_Pin_2

#define A4988_GPIO_PORT		GPIOC
#define A4988_DIR_PIN		GPIO_Pin_11
#define A4988_STEP_PIN		GPIO_Pin_12

#define LIMIT_GPIO_PORT		GPIOA
#define LIMIT_SW0_PIN		GPIO_Pin_14
#define LIMIT_SW1_PIN		GPIO_Pin_4
#define LIMIT_SW2_PIN		GPIO_Pin_5
#define LIMIT_SW3_PIN		GPIO_Pin_6
#define LIMIT_SW4_PIN		GPIO_Pin_15

#define RADIO_GPIO_PORT		GPIOC
#define RADIO_D0_ADC		ADC1
#define RADIO_D1_ADC		ADC2
#define RADIO_D2_ADC		ADC3
#define RADIO_D0_PIN		GPIO_Pin_0
#define RADIO_D1_PIN		GPIO_Pin_1
#define RADIO_D2_PIN		GPIO_Pin_2
#define RADIO_D3_PIN		GPIO_Pin_3

