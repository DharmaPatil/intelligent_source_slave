#include "main.h"
/*------------------------------------------------------------------------------------------
//��ʼ������
-------------------------------------------------------------------------------------------*/

//**************************************************************************************
//�������ƣ�void Init_interrupt(void)
//������������ʼ�������ж����ȼ� �����ж�ʹ�� �����ж�
//�������ڣ�2012-12-31
//��    ע��
//�޸����ڣ�
//�޸�˵����
//************************************************************************************** 
void Init_interrupt(void)
{  
//INTCON
	INTCONbits.RBIF=0;	INTCONbits.INT0IF=0;	INTCONbits.TMR0IF=0;	INTCONbits.RBIE=0;
	INTCONbits.INT0E=1;	INTCONbits.TMR0IE=0;	INTCONbits.PEIE=0;	INTCONbits.GIE=0;
//INTCON2
	INTCON2bits.RBIP=0;			//�����ȼ�
	INTCON2bits.TMR0IP=0;		//
	INTCON2bits.INTEDG2=0;		//�½�����Ч
	INTCON2bits.INTEDG1=0;		//�½�����Ч
	INTCON2bits.INTEDG0=0;		//�½�����Ч
	INTCON2bits.RBPU=0;			//ʹ������
//INTCON3
	INTCON3bits.INT1IF=0;	INTCON3bits.INT2IF=0;	INTCON3bits.INT1IE=0;
	INTCON3bits.INT2IE=1;	INTCON3bits.INT1IP=0;	INTCON3bits.INT2IP=0;
///*IPR1  0�����ȼ� 1�����ȼ�*/
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
    RCONbits.IPEN = 1;          // ʹ���ж����ȼ�
    
    IPR1bits.SSPIP = 1;                         //�����ȼ��ж�
        
    INTCONbits.GIEL = 1;        //
    INTCONbits.GIEH = 1;
}
//**************************************************************************************
//�������ƣ�void Timer0_Init(void)
//������������ʱ��0��ʼ��
//�������ڣ�2012-01-10
//��    ע��
//�޸����ڣ�
//�޸�˵����
//************************************************************************************** 
void Timer0_Init(void)  
{
//    Time0con = 0;                  //��time0������
    INTCONbits.TMR0IF = 0;			//
    INTCON2bits.TMR0IP=0;		   //���ȼ�
    INTCONbits.TMR0IE = 1;			//
    T0CON = 0b00000111;           // 256��Ƶ
    TMR0H = 0x0;
    TMR0L = 0x0;
    T0CONbits.TMR0ON = 1;         // 
}
//**************************************************************************************
//�������ƣ�void Timer1_Init(void)
//������������ʱ��1��ʼ��
//�������ڣ�2012-01-10
//��    ע������� ģ�⴮�� 1200������  �����ٶȲ��� 1/1200/3 = 278uS
//8��Ƶ       (4/FOSC)*8*X(max65536) = 278    X=556
//�޸����ڣ�
//�޸�˵����
//************************************************************************************** 

void Timer1_Init(void)
{
	IPR1bits.TMR1IP=1;                //�����ȼ��ж�  
	WriteTimer1( 556 );
	OpenTimer1(TIMER_INT_ON&T1_16BIT_RW&T1_SOURCE_INT&T1_PS_1_8&T1_OSC1EN_OFF&T1_SYNC_EXT_OFF);
    T1CONbits.TMR1ON = 0; 
}

//**************************************************************************************
//�������ƣ�void Timer2_Init(void)
//�����������ʱ��2��ʼ��
//�������ڣ�2012-01-10
//��    ע��
//�޸����ڣ�
//�޸�˵���� 1/Fosc = 64000000Hz  1/Fyc = 4/64000000Hz  16Ԥ��Ƶ 16���Ƶ  
//           =>  (16*16*4*PR2) / 64000000 = 0.01S(10mS)   => PR2 = 625;
//           =>  PR2 = 64000000*T/(16*16*4)    ( 0<T<0.016)
//************************************************************************************** 
void Timer2_Init(void)
{
	T2CON = 0b01111011;	      // Ԥ��Ƶ16 ���Ƶ16 �ر�TIME2
	IPR1bits.TMR2IP = 0;      //�����ȼ�
//	PR2 = 625;                //10mS  ����   ��ΪT2Ϊ8λ������ ���Դ˳�ֵ������
	PR2 = 255;                //5mS  ����
	PIE1bits.TMR2IE = 1;      //��TMR�ж�����
	PIR1bits.TMR2IF = 0;	  //��TMR�жϱ�־λ
	T2CONbits.TMR2ON =1;	
}
//**************************************************************************************
//�������ƣ�void Timer3_Init(void)
//������������ʱ��3��ʼ��
//�������ڣ�2012-01-10
//��    ע��
//�޸����ڣ�
//�޸�˵����
//************************************************************************************** 
void Timer3_Init(void)
{
/*
    Time3con = 0;                  //��time3������
    PIR2bits.TMR3IF = 0;			//
    IPR2bits.TMR3IP=0;		   //���ȼ�
    PIE2bits.TMR3IE = 1;			//
    T3CON = 0b00000111;           // 256��Ƶ
    TMR3H = 0x0;
    TMR3L = 0x0;
    T3CONbits.TMR3ON = 1;         //
*/
}

//**************************************************************************************
//�������ƣ�void InitIIC(void)
//����������IIC����ģ��  ��ʼ��  ʹ��
//�������ڣ�2012-01-10
//��    ע��
//�޸����ڣ�
//�޸�˵����
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
//�������ƣ�void InitIO(void)
//������������ʼ��IO���� ���ϵ�Ĭ��ֵ
//�������ڣ�2012-01-10
//��    ע��
//�޸����ڣ�
//�޸�˵����
//************************************************************************************** 
void InitIO(void)
{
    //Ĭ��
    LATA = 0XFF;
    LATB = 0XFF;
    LATC = 0XFF;
    LATD = 0XFF;
    LATE = 0XFF;
    //����
    LATAbits.LATA4 = 0;           //LED�����
    TRISA = 0b11101111;                 //ȫΪģ����
    TRISB = 0XFF;                 //ȫΪTTL��
    LATC  = 0b11111101;           // ģ�⴮�� RC0 RC2 ����ʱ�͵�ƽ RC1485���� ����    
    TRISC = 0b10011001;           //����ϵͳ�ӿ�
    LATD  = 0b11110000;           //
    TRISD = 0b00000110;           //����ϵͳ�ӿ�
    LATE  = 0b00000011;           //RE4ΪMCLR  ���Ÿ�λ 
    TRISE = 0b00000011;           //����ϵͳ�ӿ�

/*����ʼ��Ϊ�˵����޸ķ��� ����ÿ��IO���Ʋ��� Ӧ�ó�������޸��������� ���δ˶γ��� */
/*��������Ϊ�˵����޸ķ��� ����ÿ��IO���Ʋ��� Ӧ�ó�������޸��������� ���δ˶γ��� */
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
    Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();

}

//**************************************************************************************
//�������ƣ�void InitUSART(void)
//����������PIC USART ����ģ���ʼ��
//�������ڣ�2012-01-10
//��    ע��115200 
//�޸����ڣ�
//�޸�˵����
//************************************************************************************** 
void InitUSART(void)
{
    TXSTAbits.TXEN = 0;
    PIE1bits.RCIE=0;
    RCSTAbits.CREN = 0;    
    RCSTAbits.SPEN = 0;
    SPBRGH = DIVIDER>>8;    //
    SPBRG = DIVIDER;
    BAUDCTLbits.DTRXP = 0;		//������
    BAUDCTLbits.CKTXP = 0;		//����Ϊ�ߵ�ƽ
    BAUDCTLbits.BRG16 = 1;		//ʹ��16λ�����ʷ�����
    BAUDCTLbits.WUE = 0;		//��������������
    BAUDCTLbits.ABDEN = 0;		//��ֹ�Զ�������ģʽ
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
//�������ƣ�void ADC_Init(void)
//����������PIC ADģ�鹦�ܳ�ʼ��
//�������ڣ�2012-01-10
//��    ע��
//�޸����ڣ�
//�޸�˵����
//************************************************************************************** 
void ADC_Init(void)
{ // initialize the Analog-To-Digital converter.
    TRISAbits.TRISA0=1;   	      //̫���ܵ�ѹ�ɼ� RA0/AN0       ��ֹ�������������
    TRISAbits.TRISA1=1;           //����ѹ�ɼ�   RA1/AN1
    TRISAbits.TRISA2=1;           //�������ɼ�   RA2/AN2
    TRISAbits.TRISA5=1;			  //�¶Ȳɼ��ɼ�   RA5/AN4
    TRISEbits.TRISE0=1;           //�����ɼ�       RE0/AN5
	ANSEL = 0b00110111;	          //����������Ϊģ��	//��
	ANSELH = 0X00;
   	ADCON0 = 0b00111100;		  //�ڲ�1.2V�ο���ѹ ADC ��ֹ
 	ADCON1 = 0b00010000;          //0X10;  ��׼-����  +���ⲿ��׼
    ADCON2 = 0b10100110;          //����Ҷ���,����20Tad Ƶ��fosc/64
    IPR1bits.ADIP=0;              //�����ȼ��ж�
    PIR1bits.ADIF=0;              //��AD�жϱ�־
    PIE1bits.ADIE=0;              //������AD�ж�
}
//**************************************************************************************
//�������ƣ�void initMCU(void)
//����������initiate interrupt, timers, SSP, IO port, USART and AD converter.
//�������ڣ�
//��    ע��
//�޸����ڣ�
//�޸�˵����
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
