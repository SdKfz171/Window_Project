#define HEX_CHARS      "0123456789ABCDEF"

void USART_SendChar(char ch);

void USART_SendInt(int32_t num);
void USART_SendInt0(int32_t num);
void USART_SendHex8(uint16_t num);
void USART_SendHex16(uint16_t num);
void USART_SendHex32(uint32_t num);

void USART_SendStr(char *str);

void USART_SendBuf(char *buf, uint16_t bufsize);
void USART_SendBufPrintable(char *buf, uint16_t bufsize, char subst);
void USART_SendBufHex(char *buf, uint16_t bufsize);
void USART_SendBufHexFancy(char *buf, uint16_t bufsize, uint8_t column_width, char subst);


