#ifndef __INIT_H
#define __INIT_H
extern void InitIO(void);
extern void Timer0_Init(void);
extern void Timer1_Init(void);
extern void Timer2_Init(void);
extern void Timer3_Init(void);
extern void ADC_Init(void);
extern void InitSSP(void);
extern void InitUSART(void);
extern void Init_interrupt(void);
extern void SSP_Init(void);
extern void initMCU(void);

#endif