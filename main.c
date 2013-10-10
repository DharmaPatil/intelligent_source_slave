/***************************************************************************************************
//Dilename ：电源
//Edition  : 
//Author   : LJ
//Device   : pic18f46k20  at 16MHz pll  64MHz
//Data     : 2013/10/09
//Compiled : Using MPLAB8.92 C18 v3.40 
***************************************************************************************************/
/*短信命令队列那里有bug  在出队之后放如存储区后，如果通信的是另一个区 该次短信通信有可能被覆盖*/
#include "main.h"
/**************************C O N F I G U R A T I O N   B I T S ******************************/

#pragma config FOSC = HSPLL, FCMEN = OFF, IESO = OFF								//注意选择外部时钟4倍频必须启动双速启动才好用 // CONFIG1H  INTIO67
#pragma config PWRT = ON, BOREN = ON, BORV = 30                           	    // CONFIG2L
#pragma config WDTEN = ON, WDTPS = 1024       //看门给狗的周期是4ms  * 2048     // CONFIG2H//开看门狗
#pragma config MCLRE = ON, HFOFST = OFF ,LPT1OSC = OFF, PBADEN = OFF, CCP2MX = PORTC// CONFIG3H
#pragma config STVREN = ON, LVP = OFF, XINST = OFF   //扩展指令关闭                  // CONFIG4L
//#pragma config CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF                       	// CONFIG5L
#pragma config CP0 = ON, CP1 = ON, CP2 = ON, CP3 = ON                   	    // CONFIG5L
#pragma config CPB = OFF, CPD = OFF                                         	// CONFIG5H
//#pragma config WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF               	// CONFIG6L
#pragma config WRT0 = ON, WRT1 = ON, WRT2 = ON, WRT3 = ON               	// CONFIG6L  
#pragma config WRTB = OFF, WRTC = ON, WRTD = OFF                           	// CONFIG6H
//#pragma config EBTR0 = OFF, EBTR1 = OFF, EBTR2 = OFF, EBTR3 = OFF           	// CONFIG7L
#pragma config EBTR0 = ON, EBTR1 = ON, EBTR2 = ON, EBTR3 = ON           	// CONFIG7L
#pragma config EBTRB = OFF                                               	    // CONFIG7H
/**************************C O N F I G U R A T I O N   B I T S ******************************/





/********************************************* I N T E R R U P T S ***********************************************/
// High priority interrupt vector  高优先级中断向量
#pragma code InterruptVectorHigh = 0x08
void InterruptVectorHigh (void)
{
  _asm
    goto InterruptServiceHigh 
  _endasm
}
//--------------------------------------------------------------------------------------------------------------
// Low priority interrupt vector  低优先级中断向量
#pragma code InterruptVectorLow = 0x18
void InterruptVectorLow (void)
{
  _asm
    goto InterruptServiceLow  
  _endasm
}
/** *******************************************D E C L A R A T I O N S *******************************************/
#pragma code 
// -------------------- Iterrupt Service Routines --------------------------
#pragma interrupt InterruptServiceHigh  // "interrupt" pragma also for high priority
 void InterruptServiceHigh(void)
{
//:::::::::::::::::::::::::::::::::电平变化中断0:::::::::::::::::::::::::::::::::::::::::::::::::://
    //:::::::::::::::::::::::::::::::::SSP中断::::::::::::::::::::::::::::::::::::::::::::::::::::::://
   if ( PIR1bits.SSPIF&&PIE1bits.SSPIE )	
	{
//		SSP_Handler();
		PIR1bits.SSPIF = 0;
		return;	
	}
	 else if (INTCONbits.INT0IF&&INTCONbits.INT0IE)			 
    {
        INTCONbits.INT0IF = 0;
//        Int0_Handler();
        INTCONbits.INT0IF = 0;//在清一次 标志
	return;
    }
 //::::::::::::::::::::::::::::::::::TIME1::::::::::::::::::::::::::::::::::::::::::::::::::://
    else if  (PIR1bits.TMR1IF&&PIE1bits.TMR1IE)		
    {
		PIR1bits.TMR1IF = 0;
//		TMR1_Handler();
		return;
    }
}
#pragma interruptlow InterruptServiceLow// "interruptlow" pragma for low priority
void InterruptServiceLow(void)			                             //低优先级保护现场占用软件堆栈
{
	if (PIE1bits.ADIE&&PIR1bits.ADIF)                                           //			 
    {
        PIR1bits.ADIF = 0;
//        AD_Handler();
        return;
    } 
    else if (PIR1bits.RCIF)			
    {
//		RC_Handler();
		return;
	}
    //::::::::::::::::::::::::::::::::::TIME0中断::::::::::::::::::::::::::::::::::::::::::::::::://
    else if  (INTCONbits.TMR0IF&&INTCONbits.TMR0IE)
    {
		INTCONbits.TMR0IF = 0;
//        TMR0_Handler();
		return;
    }   
    //::::::::::::::::::::::::::::::::::TIME2::::::::::::::::::::::::::::::::::::::::::::::::::://
    else if  (PIR1bits.TMR2IF)
    {
		PIR1bits.TMR2IF = 0;
//		TMR2_Handler();
		return;
    }
//:::::::::::::::::::::::::::::::::电平变化中断2::::::::::::::::::::::::::::::::::::::::::::::::://
    else if (INTCON3bits.INT2IF&&INTCON3bits.INT2IE)
    {
        INTCON3bits.INT2IF = 0;
//        INTCON3bits.INT2IE=0; 
//		Int2_Handler();
		return;
    }     
}


void main (void)
{
	while(1);
}