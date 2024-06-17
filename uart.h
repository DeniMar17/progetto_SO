#pragma once

void printf_init(void);
unsigned char usart_ReceiveByte(void);
void usart_TransmitByte(unsigned char data);
void usart_TransmitString(char *ptrStr);
void init_USART(void);
void usart_pstr(char *s);

