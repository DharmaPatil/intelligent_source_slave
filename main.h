#ifndef MAIN_H
#define	MAIN_H

//Ìõ¼þ±àÒë
#define p18F46K20
//#define p18F45K20

#ifdef	__cplusplus
extern "C" {
#endif
//includes
#ifdef p18F46K20
#include  "p18f46k20.h"
#elif defined p18F45K20
#include  "p18f46k20.h"
#endif
#include  "stdlib.h"
#include  "delays.h"
#include  "string.h"
#include  "init.h"
#include  "math.h"
#include  "stdio.h"
#include  "string.h"
#include  "usart.h"
#include  "timers.h"
#include  "reset.h"
#include  "spi.h"
#include  "delays.h"
#include  "defines.h"
#include  "globleVar.h"
#include  "gsm.h"
#include  "adc.h"
#include  "irq.h"
#include  "GenericTypeDefs.h"
//
void main (void) ;
void InterruptVectorHigh (void);
void InterruptVectorLow (void);
void InterruptServiceHigh(void);
void InterruptServiceLow(void);


#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */




