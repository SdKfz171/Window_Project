#include "stm32f10x.h"                  // Device header
#include <string.h>

#include "serial.h"

void USART_SendChar(char ch) {
	while(!USART_GetFlagStatus(USART1, USART_FLAG_TC));    // wait for "Transmission Complete" flag cleared
	USART_SendData(USART1, ch);
	GPIO_SetBits(GPIOD,GPIO_Pin_2);
}

void USART_SendInt(int32_t num) {
	char str[10]; // 10 chars max for INT32_MAX
	int i = 0;
	if (num < 0) {
		USART_SendChar('-');
		num *= -1;
	}
	do str[i++] = num % 10 + '0'; while ((num /= 10) > 0);
	for (i--; i >= 0; i--) USART_SendChar(str[i]);
}

void USART_SendInt0(int32_t num) {
	char str[10]; // 10 chars max for INT32_MAX
	int i = 0;
	if (num < 0) {
		USART_SendChar('-');
		num *= -1;
	}
	if ((num < 10) && (num >= 0)) USART_SendChar('0');
	do str[i++] = num % 10 + '0'; while ((num /= 10) > 0);
	for (i--; i >= 0; i--) USART_SendChar(str[i]);
}

void USART_SendHex8(uint16_t num) {
	USART_SendChar(HEX_CHARS[(num >> 4)   % 0x10]);
	USART_SendChar(HEX_CHARS[(num & 0x0f) % 0x10]);
}

void USART_SendHex16(uint16_t num) {
	uint8_t i;
	for (i = 12; i > 0; i -= 4) USART_SendChar(HEX_CHARS[(num >> i) % 0x10]);
	USART_SendChar(HEX_CHARS[(num & 0x0f) % 0x10]);
}

void USART_SendHex32(uint32_t num) {
	uint8_t i;
	for (i = 28; i > 0; i -= 4)	USART_SendChar(HEX_CHARS[(num >> i) % 0x10]);
	USART_SendChar(HEX_CHARS[(num & 0x0f) % 0x10]);
}

void USART_SendStr(char *str) {
	while (*str) USART_SendChar(*str++);
}

void USART_SendBuf(char *buf, uint16_t bufsize) {
	uint16_t i;
	for (i = 0; i < bufsize; i++) USART_SendChar(*buf++);
}

void USART_SendBufPrintable(char *buf, uint16_t bufsize, char subst) {
	uint16_t i;
	char ch;
	for (i = 0; i < bufsize; i++) {
		ch = *buf++;
		USART_SendChar(ch > 32 ? ch : subst);
	}
}

void USART_SendBufHex(char *buf, uint16_t bufsize) {
	uint16_t i;
	char ch;
	for (i = 0; i < bufsize; i++) {
		ch = *buf++;
		USART_SendChar((char)ch);
		//USART_SendChar((char)HEX_CHARS[(ch >> 4)   % 0x10]);
		//USART_SendChar((char)HEX_CHARS[(ch & 0x0f) % 0x10]);
	}
}

void USART_SendBufHexFancy(char *buf, uint16_t bufsize, uint8_t column_width, char subst) {
	uint16_t i = 0,len,pos;
	char buffer[column_width];

	while (i < bufsize) {
		// Line number
		USART_SendHex16(i);
		USART_SendChar(':'); USART_SendChar(' '); // Faster and less code than USART_SendStr(": ");

		// Copy one line
		if (i+column_width >= bufsize) len = bufsize - i; else len = column_width;
		memcpy(buffer,&buf[i],len);

		// Hex data
		pos = 0;
		while (pos < len) USART_SendHex8(buffer[pos++]);
		USART_SendChar(' ');

		// Raw data
		pos = 0;
		do USART_SendChar(buffer[pos] > 32 ? buffer[pos] : subst); while (++pos < len);
		USART_SendChar('\n');

		i += len;
	}
}