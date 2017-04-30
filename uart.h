#ifndef uart_h
#define uart_h

#include "GenericTypeDefs.h"
#include "Compiler.h"


///////////////////////////////////////////////////
//Константы
#define UART_MAX_LENGHT_DATA		128

#define UART_MODE_TEXT				0x00
#define UART_MODE_BYTE				0x01

//Интерфейсы
#define UART_INTERFACE_NON			0x00
#define UART_INTERFACE_BT			0x01
#define UART_INTERFACE_ST			0x02


//Структуры
typedef struct
{
	unsigned char UART_MODE;
	unsigned char UART_INTERFACE;
	unsigned char UART_RX_DATA_VALUE;
	BOOL UART_RX_FLAG;
	char UART_RX_DATA[UART_MAX_LENGHT_DATA];
	char UART_TX_DATA[UART_MAX_LENGHT_DATA];
	unsigned int UART_RX_DATA_POS;
	unsigned int UART_RX_DATA_LENGHT;
} UART_STRUCT;

void UART_Init(UART_STRUCT *p, unsigned long speed, unsigned char mode);
void UART_Interrupt(UART_STRUCT *p);
void UART_Task(UART_STRUCT *p);
void UART_Send_String(UART_STRUCT *p, const char* reply);

#endif