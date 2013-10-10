#include "main.h" 
 
#pragma udata gpr0

#pragma udata gpr1		//ssp
UINT8 SspQueue[200];
UINT8 TimeOut;
UINT16 Time0con;
UINT8 acStatus[2];//Y有交流  N无交流
UINT8 netStatus[2];
UINT8 troubleStatus[2];
UINT8 chargeStatus[2];
UINT8 lightStatus[2];
UINT8 modeStatus[2];
#pragma udata gpr2		//sys
UINT8 S_To_M_buf0[76];
UINT8 S_To_M_buf1[76];
Flag  sysFlag;
#pragma udata gpr3  //gsm
UINT8 gsmreCount;//接收计数
UINT8 ReadMessagecon;
UINT8 gsmBuf[200];
UINT8 gsmreStatus;//A:not receive  B:receive cmd   C:receive short message
Flag  gsmFlag,gsmFlag1;
UINT8 csqH,csqL;
UINT8 *Prt;//接收短信指针
UINT8 gsmPhoneNum[12];
UINT8 smCmd;
UINT8 phone1SM[6];
UINT8 phone2SM[6];
//phonexSM
/*
1:交流		Y有交流 N无交流  0xff不发送
2：网络		Y网络通	N网络断  0xff不发送
3：故障		Y有故障			 0xff不发送
4：等充电	Y等充电			 0xff不发送
5：雷击		Y有雷击			 0xff不发送
6：电源模式状态  A：直到隔  B隔到直 C直到浮 D浮到直 E隔到浮 F浮到隔 0xff不发送
*/
#pragma udata gpr4

