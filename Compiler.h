#ifndef __COMPILER_H
#define __COMPILER_H

// Include proper device header file
#if defined(__18CXX) && !defined(HI_TECH_C)	
	// PIC18 processor with Microchip C18 compiler
    #define COMPILER_MPLAB_C18
    #include <p18cxxx.h>
#elif defined(__PICC18__) && defined(HI_TECH_C)	
	// PIC18 processor with (Microchip) HI-TECH PICC-18 compiler
	#if !defined(__18CXX)
		#define __18CXX
	#endif
    #define COMPILER_HITECH_PICC18
	#include <htc.h>
#elif (defined(_PIC12) || defined(_PIC14) || defined(_PIC14E)) && defined(HI_TECH_C)
	// PIC10/12/16 processor with (Microchip) HI-TECH PICC compiler
    #define COMPILER_HITECH_PICC
    #include <htc.h>
#elif (defined(__PIC24F__) || defined(__PIC24FK__)) && defined(__C30__)	// Microchip C30 compiler
	// PIC24F processor
    #define COMPILER_MPLAB_C30
	#include <p24Fxxxx.h>
#elif defined(__PIC24H__) && defined(__C30__)	// Microchip C30 compiler
	// PIC24H processor
    #define COMPILER_MPLAB_C30
	#include <p24Hxxxx.h>
#elif defined(__PIC24E__) && defined(__C30__)	// Microchip C30 compiler
	// PIC24E processor
    #define COMPILER_MPLAB_C30
	#include <p24Exxxx.h>
#elif defined(__dsPIC33F__) && defined(__C30__)	// Microchip C30 compiler
	// dsPIC33F processor
    #define COMPILER_MPLAB_C30
	#include <p33Fxxxx.h>
#elif defined(__dsPIC33E__) && defined(__C30__)	// Microchip C30 compiler
	// dsPIC33E processor
    #define COMPILER_MPLAB_C30
	#include <p33Exxxx.h>
#elif defined(__dsPIC30F__) && defined(__C30__)	// Microchip C30 compiler
	// dsPIC30F processor
    #define COMPILER_MPLAB_C30
	#include <p30fxxxx.h>
#elif defined(__C30__)		// Microchip C30 compiler, but targeting "generic-16bit" processor.
    #define COMPILER_MPLAB_C30
	#include <p30sim.h>
	// Define some useful inline assembly functions which are normally in the 
	// processor header files, but absent from the generic p30sim.h file.
	#if !defined(Nop)
		#define Nop()    __builtin_nop()
		#define ClrWdt() {__asm__ volatile ("clrwdt");}
		#define Sleep()  {__asm__ volatile ("pwrsav #0");}
		#define Idle()   {__asm__ volatile ("pwrsav #1");}
	#endif
#elif defined(__PIC32MX__)	// Microchip C32 compiler
	#if !defined(__C32__)
		#define __C32__
	#endif
    #define COMPILER_MPLAB_C32
	#include <p32xxxx.h>
	#include <plib.h>
#else
	#error Unknown processor or compiler.  See Compiler.h
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Base RAM and ROM pointer types for given architecture
#if defined(__PIC32MX__)
	#define PTR_BASE		unsigned long
	#define ROM_PTR_BASE	unsigned long
#elif defined(__C30__)
	#define PTR_BASE		unsigned short
	#define ROM_PTR_BASE	unsigned short
#elif defined(COMPILER_MPLAB_C18)
	#define PTR_BASE		unsigned short
	#define ROM_PTR_BASE	unsigned short long
#elif defined(COMPILER_HITECH_PICC18)
	//#define PTR_BASE		unsigned short
	//#define ROM_PTR_BASE	unsigned long
#endif


// Definitions that apply to all except Microchip MPLAB C Compiler for PIC18 MCUs (C18)
#if !defined(COMPILER_MPLAB_C18)
	#define memcmppgm2ram(a,b,c)	memcmp(a,b,c)
	#define strcmppgm2ram(a,b)		strcmp(a,b)
	#define memcpypgm2ram(a,b,c)	memcpy(a,b,c)
	#define strcpypgm2ram(a,b)		strcpy(a,b)
	#define strncpypgm2ram(a,b,c)	strncpy(a,b,c)
	#define strstrrampgm(a,b)		strstr(a,b)
	#define	strlenpgm(a)			strlen(a)
	#define strchrpgm(a,b)			strchr(a,b)
	#define strcatpgm2ram(a,b)		strcat(a,b)
#endif


// Definitions that apply to all 8-bit products
// (PIC10, PIC12, PIC16, PIC18)
#if defined(__18CXX) || defined(COMPILER_HITECH_PICC)
	#define	__attribute__(a)

    #define FAR                         far

	// Microchip C18 specific defines
	#if defined(COMPILER_MPLAB_C18)
	    #define ROM                 	rom
	#endif
	
	// HI TECH specific defines
	#if defined(COMPILER_HITECH_PICC18) || defined(COMPILER_HITECH_PICC)
	    #define ROM                 	const
		#define rom
        #ifndef Nop()
        #define Nop()               	asm("NOP");
        #endif
        #ifndef ClrWdt()
        #define ClrWdt()				asm("CLRWDT");
        #endif
        #ifndef Reset()
        #define Reset()					asm("RESET");
        #endif
        #ifndef Sleep()
        #define Sleep()					asm("SLEEP");
        #endif
	#endif
    
// Definitions that apply to all 16-bit and 32-bit products
// (PIC24F, PIC24H, dsPIC30F, dsPIC33F, and PIC32)
#else
	#define	ROM						const

	// 16-bit specific defines (PIC24F, PIC24H, dsPIC30F, dsPIC33F)
	#if defined(__C30__)
		#define Reset()				asm("reset")
        #define FAR                 __attribute__((far))
	#endif

	// 32-bit specific defines (PIC32)
	#if defined(__PIC32MX__)
        #if (__C32_VERSION__ < 200)
            #define persistent
        #endif
		#define far
        #define FAR
		#define Reset()				SoftReset()
		#define ClrWdt()			(WDTCONSET = _WDTCON_WDTCLR_MASK)

		// MPLAB C Compiler for PIC32 MCUs version 1.04 and below don't have a 
		// Nop() function. However, version 1.05 has Nop() declared as _nop().
		#if !defined(Nop) && (__C32_VERSION__ <= 104)
			#define Nop()				asm("nop")
		#endif
	#endif
#endif



#endif
