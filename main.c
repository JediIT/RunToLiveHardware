//���������� includes
#include <htc.h>

//��������� includes
#include "config.h"

//UART
#include "uart.h"

//����������� ���� ����������������
#if !defined(__18F4550) && !defined(__18F2550) && !defined(__18F2455) && !defined(__18F4455)
	#error "��� ��������� ������������ ������ ���������������� PIC18F4550, PIC18F2550, PIC18F2455 ��� PIC18F4455."
#endif

//������������ ��� PIC18F2550 ��� PIC18F4550:
//���������������� ����� �1 (������������ ����������)
//��������� ��������� 20Mhz �� ����� ����������� �� 48Mhz � ������������� ��� ������ � USB
__CONFIG(1, USBPLL & IESODIS & FCMDIS & HSPLL & CPUDIV1 & PLLDIV5);
//���������������� ����� �2
__CONFIG(2, VREGEN & PWRTDIS & BOREN & BORV20 & WDTDIS & WDTPS32K);
//���������������� ����� �3
__CONFIG(3, PBDIGITAL & LPT1DIS & MCLREN);
//���������������� ����� �4
__CONFIG(4, XINSTDIS & STVREN & LVPDIS & ICPORTDIS & DEBUGDIS);
//���������������� ����� 5, 6 and 7 (������������ ������ ��������)
__CONFIG(5, UNPROTECT);
__CONFIG(6, UNPROTECT);
__CONFIG(7, UNPROTECT);


#define  testbit(var, nomer_bit)   ((var) & (1 <<(nomer_bit)))
#define  setbit(var, nomer_bit)    ((var) |= (1 << (nomer_bit)))
#define  clrbit(var, nomer_bit)    ((var) &= ~(1 << (nomer_bit)))


//��������� prototypes
void InitialiseSystem(void);
void SystemIO(void);


bit set_sw0=0;
unsigned int tm_sw0=0;
unsigned char SWITCH0_STATE=0;

bit set_sw1=0;
unsigned int tm_sw1=0;
unsigned char SWITCH1_STATE=0;

//�������� ����������
unsigned long test=50;

int time1;
int time2;


UART_STRUCT st;


void delayms(unsigned int ms)
{
	while(ms--)	__delay_ms(1);
}

void interrupt hi_isr(void)
{
	if(TMR0IF)
	{
		//TIMER0 = 1 ms

		TMR0IF = 0;
		TMR0H = t0_h;
		TMR0L = t0_l;

        time1=time1+1;
        time2=time2+1;

		if (time1==500) //����������� ������ 500 ms
		{
			time1=0;          
			LED0^=1;
		}
	}

	//���������� UART
	UART_Interrupt(&st);

}

void interrupt low_priority low_isr(void)
{

}

//������������� �������
void InitialiseSystem(void)
{
    GIE=0;    		//��� �� ������������� ����������
    GIEH=0;
    PEIE=0;   		//��� �� ������������� ���������� 
    GIEL=0;   		//������������ �������

    ADCON1=0b00001111; //������������ ������ (���������� ��� ��������)

	//������������ ������ �� ����(1) ��� �����(0)
	TRISA = 0b00000111;
	TRISB = 0b00000000;
	TRISC = 0b00000000;
#if defined(__18F4550)
	TRISD = 0b00000000;
	TRISE = 0b00000000;
#endif

	//������� ������
	PORTA = 0b00000000;
	PORTB = 0b00000000;
	PORTC = 0b00000000;
#if defined(__18F4550)
	PORTD = 0b00000000;
	PORTE = 0b00000000;
#endif


    T0CON=0b10000000; //��� ���./����. ������ TIMER0
                      //16 ������ ������
					  //1:2
    TMR0IE=1;         //��������� ���������� �� ������� TMR0   
   
    IPEN=1;   //��������� ������������� ��������� ����������� ����������
    TMR0IP=1; //������� ��������� ���������� �� TIMER0

	//������������� UART
	//UART_Init(&st, 115200, UART_MODE_TEXT);
	UART_Init(&st, 9600, UART_MODE_TEXT);

    GIE=1;		//��������� ��� �� ������������� ����������
    GIEH=1;
    PEIE=1;		//��������� ��� �� ������������� ���������� 
    GIEL=1;		//������������ �������
    IPEN=1;		//��������� ������������� ��������� ����������� ����������
}

void SystemIO(void)
{
	if (SWITCH0==1 && set_sw0==0)
	{
		tm_sw0++;
		if (tm_sw0>=10000)
		{		
			set_sw0=1;
			SWITCH0_STATE=1;
			LED1=1;

			UART_Send_String(&st, "> Fire\r\n");
			UART_Send_String(&st, "lat#46.475861\r\n");
			UART_Send_String(&st, "lon#30.739490\r\n");
			UART_Send_String(&st, "radius#10\r\n");
			UART_Send_String(&st, "typeId#1\r\n");
			UART_Send_String(&st, "photoId#1\r\n");
			UART_Send_String(&st, "<\r\n");
		}
	}
	if (SWITCH0==0)
	{
		set_sw0=0;
		tm_sw0=0;
		SWITCH0_STATE=0;
		LED1=0;
	}

	if (SWITCH1==1 && set_sw1==0)
	{
		tm_sw1++;
		if (tm_sw1>=10000)
		{		
			set_sw1=1;
			SWITCH1_STATE=1;
			LED2=1;

			UART_Send_String(&st, "> SOS\r\n");
			UART_Send_String(&st, "lat#46.4758543\r\n");
			UART_Send_String(&st, "lon#30.7399353\r\n");
			UART_Send_String(&st, "radius#10\r\n");
			UART_Send_String(&st, "typeId#2\r\n");
			UART_Send_String(&st, "photoId#1\r\n");
			UART_Send_String(&st, "<\r\n");
		}
	}
	if (SWITCH1==0)
	{
		set_sw1=0;
		tm_sw1=0;
		SWITCH1_STATE=0;
		LED2=0;
	}
}
       
//Program:                                 
void main()
{

	InitialiseSystem();

	UART_Send_String(&st, "RunToLive\r\n");

	while(1==1)
	{
		SystemIO();

		UART_Task(&st);
    }   	
}
