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

typedef union  _FLAG						
{  
 struct
  { 
	unsigned b0:1;    
	unsigned b1:1;         
    unsigned b2:1;	       			
    unsigned b3:1;          
    unsigned b4:1;          
    unsigned b5:1;	       			
    unsigned b6:1;          
    unsigned b7:1;         
  }Inbit;
  unsigned char fByte;         
}Flag;
//sys
#define SSP_Succeed         sysFlag.Inbit.b0     //1:����ͨѶ�ɹ�		0��û�ɹ�    
#define TROUBLE	            sysFlag.Inbit.b1     //1:����ͨѶ����		0��û����  

#define HadAC				sysFlag.Inbit.b2
//gms
#define gsmOnOff 			PORTEbits.RE1//1:on  0:off
#define csqFlag				gsmFlag.Inbit.b0  	//1:���յ�csq��Ϣ		0��û��
#define cpinFlag			gsmFlag.Inbit.b1	//1:����cpin��Ϣ����	0��û��
#define cregFlag			gsmFlag.Inbit.b2	//1:����creg��Ϣ����	0��û��
#define okFlag			    gsmFlag.Inbit.b3	//1:���յ�ok��Ϣ		0��û��
#define cpinreFlag			gsmFlag.Inbit.b4	//1:���յ�cpin��Ϣ		0��û��
#define cregreFlag			gsmFlag.Inbit.b5	//1:���յ�creg��Ϣ		0��û��
#define smreFlag			gsmFlag.Inbit.b6	//1�����յ�����			0:û��
#define ssmtxFlag			gsmFlag.Inbit.b7    //1�����ŷ��ͳɹ�       0�����ŷ���δ�ɹ�
#define errorFlag			gsmFlag1.Inbit.b0	//1:���յ�error��Ϣ		0��û��
#define gsmSM1              gsmFlag1.Inbit.b1	//1:�����ֻ�1����Դ״̬����		0��û�� 
#define gsmSM2              gsmFlag1.Inbit.b2	//1:�����ֻ�1����Դ״̬����		0��û��  
#define NR     				'A'
#define RC					'B'//receive cmd
#define RSM					'C'	//receive short message
#endif