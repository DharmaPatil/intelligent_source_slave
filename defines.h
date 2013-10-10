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
#define SSP_Succeed         sysFlag.Inbit.b0     //1:主从通讯成功		0：没成功    
#define TROUBLE	            sysFlag.Inbit.b1     //1:主从通讯故障		0：没故障  

#define HadAC				sysFlag.Inbit.b2
//gms
#define gsmOnOff 			PORTEbits.RE1//1:on  0:off
#define csqFlag				gsmFlag.Inbit.b0  	//1:接收到csq信息		0：没有
#define cpinFlag			gsmFlag.Inbit.b1	//1:接收cpin信息正常	0：没有
#define cregFlag			gsmFlag.Inbit.b2	//1:接收creg信息正常	0：没有
#define okFlag			    gsmFlag.Inbit.b3	//1:接收到ok信息		0：没有
#define cpinreFlag			gsmFlag.Inbit.b4	//1:接收到cpin信息		0：没有
#define cregreFlag			gsmFlag.Inbit.b5	//1:接收到creg信息		0：没有
#define smreFlag			gsmFlag.Inbit.b6	//1：接收到短信			0:没有
#define ssmtxFlag			gsmFlag.Inbit.b7    //1：短信发送成功       0：短信发送未成功
#define errorFlag			gsmFlag1.Inbit.b0	//1:接收到error信息		0：没有
#define gsmSM1              gsmFlag1.Inbit.b1	//1:给主手机1发电源状态短信		0：没有 
#define gsmSM2              gsmFlag1.Inbit.b2	//1:给主手机1发电源状态短信		0：没有  
#define NR     				'A'
#define RC					'B'//receive cmd
#define RSM					'C'	//receive short message
#endif