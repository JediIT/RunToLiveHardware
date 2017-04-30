#include <htc.h>
#include "uart.h"
#include "config.h"

void UART_Interrupt(UART_STRUCT *p)
{
	unsigned char c;

	if(RCIF)
	{
		RCIF = 0;

		//Чтение регистра
		p->UART_RX_DATA_VALUE = RCREG;
	
		//Режим UART_MODE_TEXT:
		if (p->UART_MODE==UART_MODE_TEXT)
		{
			if (p->UART_RX_DATA_VALUE!=0x00)
			{
				if (p->UART_RX_DATA_VALUE!='\n')
				{
					if (p->UART_RX_DATA_POS < UART_MAX_LENGHT_DATA)
					{
						p->UART_RX_DATA[p->UART_RX_DATA_POS] = p->UART_RX_DATA_VALUE;
						p->UART_RX_DATA_POS++;
					}
					else
					{
						p->UART_RX_DATA_POS=0;
						p->UART_RX_FLAG=1;
					}
				}
				else
				{
					if (p->UART_RX_DATA_POS < UART_MAX_LENGHT_DATA)
					{
						p->UART_RX_DATA[p->UART_RX_DATA_POS] = p->UART_RX_DATA_VALUE;
					}
					p->UART_RX_DATA_POS=0;
					p->UART_RX_FLAG=1;
				}
			}
		}
	
		//Режим UART_MODE_BYTE:
		if (p->UART_MODE==UART_MODE_BYTE)
		{
			p->UART_RX_DATA[p->UART_RX_DATA_POS] = p->UART_RX_DATA_VALUE;
			p->UART_RX_DATA_POS++;
		
			if (p->UART_RX_DATA_POS==3)
			{
				if (p->UART_RX_DATA[0]=='$'
					&& p->UART_RX_DATA[1]>0
					&& p->UART_RX_DATA[2]>0)
				{
					p->UART_RX_DATA_LENGHT=p->UART_RX_DATA[2];
				}
				else
					p->UART_RX_DATA_POS=0;
			}
			if (p->UART_RX_DATA_LENGHT > 0 && p->UART_RX_DATA_POS == p->UART_RX_DATA_LENGHT)
			{
				p->UART_RX_DATA_POS=0;
				p->UART_RX_FLAG=1;
			}
		}
	}
}

void UART_Init(UART_STRUCT *p, unsigned long speed, unsigned char mode)
{
	TRISC6 = 0;
	TRISC7 = 1;

	SYNC = 0;
	TX9 = 0;
	TXEN = 1;

	RX9 = 0;
	CREN = 1;
	SPEN = 1;

	switch (speed)
	{
		case 115200:
			BRG16 = 0;
			BRGH = 1;
			SPBRG = (int)(((CLOCK_FREQ/speed)/16)-1);	
			break;
		case 9600:
			BRG16 = 0;
			BRGH = 0;
			SPBRG = (int)(((CLOCK_FREQ/speed)/64)-1);	
			break;
	}

	RCIE = 1;
}

void UART_Send_String(UART_STRUCT *p, const char* reply)
{
   	unsigned char c;

	while((c = *reply++))
	{
		TXREG = c;
		while(TRMT == 0);
	}
}

void UART_Task(UART_STRUCT *p)
{
    unsigned int i=0;
	unsigned int offset=0;
	int sfr_adr;
	int sfr_dat;
	char s[32];

	if (p->UART_RX_FLAG==1)
	{
		p->UART_RX_FLAG=0;

		//Режим UART_MODE_TEXT:
		if (p->UART_MODE==UART_MODE_TEXT)
		{
			if (p->UART_RX_DATA[0] == '$')
			{
				//get the command byte
				switch(p->UART_RX_DATA[1])
				{
					case '?':
						UART_Send_String(p, "> RunToLive\r\n");
						break;
					default:
						UART_Send_String(p, "> ERROR [1]\r\n");
						break;
				}
		
				//Символ ожидания приёма новых команд
				UART_Send_String(p, "<\r\n");
		
				//Очистка буфера
				for (i=0; i<UART_MAX_LENGHT_DATA; i++)
				{
					p->UART_RX_DATA[i]=0;
				}
			}
			else
			{			
				//Очистка буфера
				for (i=0; i<UART_MAX_LENGHT_DATA; i++)
				{
					p->UART_RX_DATA[i]=0;
				}
			}
		}
	}
}
