#include "main.h"
/*------------------------------------------------------------------------------------------
//初始化函数
-------------------------------------------------------------------------------------------*/

//**************************************************************************************
//函数名称：void Init_interrupt(void)
//功能描述：初始化各个中断优先级 二级中断使能 开总中断
//创建日期：2012-12-31
//备    注：
//修改日期：
//修改说明：
//************************************************************************************** 
void Init_interrupt(void)
{  
//INTCON
	INTCONbits.RBIF=0;	INTCONbits.INT0IF=0;	INTCONbits.TMR0IF=0;	INTCONbits.RBIE=0;
	INTCONbits.INT0E=1;	INTCONbits.TMR0IE=0;	INTCONbits.PEIE=0;	INTCONbits.GIE=0;
//INTCON2
	INTCON2bits.RBIP=0;			//低优先级
	INTCON2bits.TMR0IP=0;		//
	INTCON2bits.INTEDG2=0;		//下降沿有效
	INTCON2bits.INTEDG1=0;		//下降沿有效
	INTCON2bits.INTEDG0=0;		//下降沿有效
	INTCON2bits.RBPU=0;			//使能上拉
//INTCON3
	INTCON3bits.INT1IF=0;	INTCON3bits.INT2IF=0;	INTCON3bits.INT1IE=0;
	INTCON3bits.INT2IE=1;	INTCON3bits.INT1IP=0;	INTCON3bits.INT2IP=0;
///*IPR1  0低优先级 1高优先级*/
	IPR1bits.PSPIP=0;	IPR1bits.ADIP=0;	IPR1bits.RCIP=0;	IPR1bits.TXIP=0;
	IPR1bits.SSPIP=1;	IPR1bits.CCP1IP=0;	IPR1bits.TMR2IP=0;	IPR1bits.TMR1IP=0;
///*IPR2*/
	IPR2bits.CCP2IP=0;	IPR2bits.TMR3IP=0;	IPR2bits.LVDIP=0;	IPR2bits.BCLIP=0;
	IPR2bits.EEIP=0;	IPR2bits.C2IP=0;	IPR2bits.C1IP=0;	IPR2bits.OSCFIP=0;
///*PIE1*/
	PIE1bits.TMR1IE=0;	PIE1bits.TMR2IE=0;	PIE1bits.CCP1IE=0;	PIE1bits.SSPIE=0;
	PIE1bits.TXIE=0;	PIE1bits.RCIE=0;	PIE1bits.ADIE=0;	PIE1bits.PSPIE=0;
///*PIE2*/
	PIE2bits.CCP2IE=0;	PIE2bits.TMR3IE=0;	PIE2bits.LVDIE=0;	PIE2bits.BCLIE=0;
	PIE2bits.EEIE=0;	PIE2bits.C2IE=0;	PIE2bits.C1IE=0;	PIE2bits.OSCFIE=0;
    RCONbits.IPEN = 1;          // 使能中断优先级
    
    IPR1bits.SSPIP = 1;                         //高优先级中断
        
    INTCONbits.GIEL = 1;        //
    INTCONbits.GIEH = 1;
}
//**************************************************************************************
//函数名称：void Timer0_Init(void)
//功能描述：定时器0初始化
//创建日期：2012-01-10
//备    注：
//修改日期：
//修改说明：
//************************************************************************************** 
void Timer0_Init(void)  
{
//    Time0con = 0;                  //清time0计数器
    INTCONbits.TMR0IF = 0;			//
    INTCON2bits.TMR0IP=0;		   //优先级
    INTCONbits.TMR0IE = 1;			//
    T0CON = 0b00000111;           // 256分频
    TMR0H = 0x0;
    TMR0L = 0x0;
    T0CONbits.TMR0ON = 1;         // 
}
//**************************************************************************************
//函数名称：void Timer1_Init(void)
//功能描述：定时器1初始化
//创建日期：2012-01-10
//备    注：分配给 模拟串口 1200波特率  三倍速度采样 1/1200/3 = 278uS
//8分频       (4/FOSC)*8*X(max65536) = 278    X=556
//修改日期：
//修改说明：
//************************************************************************************** 

void Timer1_Init(void)
{
	IPR1bits.TMR1IP=1;                //高优先级中断  
	WriteTimer1( 556 );
	OpenTimer1(TIMER_INT_ON&T1_16BIT_RW&T1_SOURCE_INT&T1_PS_1_8&T1_OSC1EN_OFF&T1_SYNC_EXT_OFF);
    T1CONbits.TMR1ON = 0; 
}

//**************************************************************************************
//函数名称：void Timer2_Init(void)
//输入参数：定时器2初始化
//创建日期：2012-01-10
//备    注：
//修改日期：
//修改说明： 1/Fosc = 64000000Hz  1/Fyc = 4/64000000Hz  16预分频 16后分频  
//           =>  (16*16*4*PR2) / 64000000 = 0.01S(10mS)   => PR2 = 625;
//           =>  PR2 = 64000000*T/(16*16*4)    ( 0<T<0.016)
//************************************************************************************** 
void Timer2_Init(void)
{
	T2CON = 0b01111011;	      // 预分频16 后分频16 关闭TIME2
	IPR1bits.TMR2IP = 0;      //低优先级
//	PR2 = 625;                //10mS  周期   因为T2为8位计数器 所以此初值有问题
	PR2 = 255;                //5mS  周期
	PIE1bits.TMR2IE = 1;      //开TMR中断允许
	PIR1bits.TMR2IF = 0;	  //清TMR中断标志位
	T2CONbits.TMR2ON =1;	
}
//**************************************************************************************
//函数名称：void Timer3_Init(void)
//功能描述：定时器3初始化
//创建日期：2012-01-10
//备    注：
//修改日期：
//修改说明：
//************************************************************************************** 
void Timer3_Init(void)
{
/*
    Time3con = 0;                  //清time3计数器
    PIR2bits.TMR3IF = 0;			//
    IPR2bits.TMR3IP=0;		   //优先级
    PIE2bits.TMR3IE = 1;			//
    T3CON = 0b00000111;           // 256分频
    TMR3H = 0x0;
    TMR3L = 0x0;
    T3CONbits.TMR3ON = 1;         //
*/
}

//**************************************************************************************
//函数名称：void InitIIC(void)
//功能描述：IIC功能模块  初始化  使能
//创建日期：2012-01-10
//备    注：
//修改日期：
//修改说明：
//************************************************************************************** 
void InitSSP(void)
{

//SSPCON1
	IPR1bits.SSPIP=1;
	SSPCON1bits.WCOL=0;		SSPCON1bits.SSPOV=0;	SSPCON1bits.SSPEN=1;	SSPCON1bits.CKP=1;
	SSPCON1bits.SSPM3=0;	SSPCON1bits.SSPM2=1;	SSPCON1bits.SSPM1=1;	SSPCON1bits.SSPM0=0;
	SSPCON2=0x00;
	SSPSTAT=0x80;
	SSPADD=0x97;         		//IIC
}

//**************************************************************************************
//函数名称：void InitIO(void)
//功能描述：初始化IO方向 及上电默认值
//创建日期：2012-01-10
//备    注：
//修改日期：
//修改说明：
//************************************************************************************** 
void InitIO(void)
{
    //默认
    LATA = 0XFF;
    LATB = 0XFF;
    LATC = 0XFF;
    LATD = 0XFF;
    LATE = 0XFF;
    //设置
    LATAbits.LATA4 = 0;           //LED灯输出
    TRISA = 0b11101111;                 //全为模拟入
    TRISB = 0XFF;                 //全为TTL入
    LATC  = 0b11111101;           // 模拟串口 RC0 RC2 空闲时低电平 RC1485允许 接受    
    TRISC = 0b10011001;           //根据系统接口
    LATD  = 0b11110000;           //
    TRISD = 0b00000110;           //根据系统接口
    LATE  = 0b00000011;           //RE4为MCLR  短信复位 
    TRISE = 0b00000011;           //根据系统接口

/*（开始）为了调试修改方便 增加每个IO控制操作 应用程序可以修改上面配置 屏蔽此段程序 */
/*（结束）为了调试修改方便 增加每个IO控制操作 应用程序可以修改上面配置 屏蔽此段程序 */
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();

}

//**************************************************************************************
//函数名称：void InitUSART(void)
//功能描述：PIC USART 功能模块初始化
//创建日期：2012-01-10
//备    注：115200 
//修改日期：
//修改说明：
//************************************************************************************** 
void InitUSART(void)
{
    TXSTAbits.TXEN = 0;
    PIE1bits.RCIE=0;
    RCSTAbits.CREN = 0;    
    RCSTAbits.SPEN = 0;
    SPBRGH = DIVIDER>>8;    //
    SPBRG = DIVIDER;
    BAUDCTLbits.DTRXP = 0;		//不反相
    BAUDCTLbits.CKTXP = 0;		//空闲为高电平
    BAUDCTLbits.BRG16 = 1;		//使用16位波特率发生器
    BAUDCTLbits.WUE = 0;		//接收器正常工作
    BAUDCTLbits.ABDEN = 0;		//禁止自动波特率模式
    TXSTAbits.TX9 = 0;
    TXSTAbits.TXEN = 1;
    TXSTAbits.SYNC = 0;
    TXSTAbits.SENDB = 0;
    TXSTAbits.BRGH = 1;
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
    TXSTA = 0x26;
    RCSTA = 0x90;
    IPR1bits.RCIP=0;	
    IPR1bits.TXIP=0;
    PIE1bits.RCIE=1;
}

//**************************************************************************************
//函数名称：void ADC_Init(void)
//功能描述：PIC AD模块功能初始化
//创建日期：2012-01-10
//备    注：
//修改日期：
//修改说明：
//************************************************************************************** 
void ADC_Init(void)
{ // initialize the Analog-To-Digital converter.
    TRISAbits.TRISA0=1;   	      //太阳能电压采集 RA0/AN0       禁止引脚输出驱动器
    TRISAbits.TRISA1=1;           //充电电压采集   RA1/AN1
    TRISAbits.TRISA2=1;           //充电电流采集   RA2/AN2
    TRISAbits.TRISA5=1;			  //温度采集采集   RA5/AN4
    TRISEbits.TRISE0=1;           //交流采集       RE0/AN5
	ANSEL = 0b00110111;	          //将引脚配置为模拟	//改
	ANSELH = 0X00;
   	ADCON0 = 0b00111100;		  //内部1.2V参考电压 ADC 禁止
 	ADCON1 = 0b00010000;          //0X10;  基准-到地  +到外部基准
    ADCON2 = 0b10100110;          //结果右对齐,采样20Tad 频率fosc/64
    IPR1bits.ADIP=0;              //低优先级中断
    PIR1bits.ADIF=0;              //清AD中断标志
    PIE1bits.ADIE=0;              //不允许AD中断
}
//**************************************************************************************
//函数名称：void initMCU(void)
//功能描述：initiate interrupt, timers, SSP, IO port, USART and AD converter.
//创建日期：
//备    注：
//修改日期：
//修改说明：
//**************************************************************************************
void initMCU(void)
{
	Init_interrupt();
	Timer0_Init();
	Timer1_Init();
	Timer2_Init();
	Timer3_Init();
	InitSSP();
	InitIO();
	InitUSART();
	ADC_Init();
}
