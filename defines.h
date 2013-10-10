#ifndef __DEFINES_H
#define __DEFINES_H
//USART
#define		fosc 64000000L 
#define		BAUD 9600
#define		DIVIDER ((UINT)(fosc/(4UL * BAUD) -1)) 	
//types
typedef signed int            INT;
typedef signed char           INT8;
typedef signed short int      INT16;
typedef signed long int       INT32;
typedef unsigned int          UINT;
typedef unsigned char         UINT8;
typedef unsigned short int    UINT16;
typedef unsigned long int     UINT32;

//gms
#define gsmOnOff 			PORTEbits.RE1//1:on  0:off

#endif