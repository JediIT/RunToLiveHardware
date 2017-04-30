//Частота работы микроконтроллера (48 Mhz)
#define _XTAL_FREQ 48000000
#define CLOCK_FREQ 48000000

//Настройки таймеров
#define t0		1							//Timer0 divides down to 1 ms
#define PREG0	(CLOCK_FREQ/1000)/4*t0/2	//1:2 prescaler
#define t0_h	((-PREG0)>>8) & 0xff;
#define t0_l	(-PREG0) & 0xff;
	
//Тип контроллера и версия прошивки
#if defined(__18F2550)
	#define TYPE_PIC	1
#endif
#if defined(__18F4550)
	#define TYPE_PIC	2
#endif
#if defined(__18F2455)
	#define TYPE_PIC	3
#endif
#if defined(__18F4455)
	#define TYPE_PIC	4
#endif

#define MAJOR_VERSION	1
#define MINOR_VERSION	0

//Назначение входов/выходов микроконтроллера к новым именам
#define LED0	LATC0
#define LED1	LATC1
#define LED2	LATC2
#define SWITCH0	!RA0
#define SWITCH1	!RA1
#define SWITCH2	!RA2
