#include "main.h" 
 
#pragma udata gpr0

#pragma udata gpr1		//ssp
UINT8 SspQueue[200];
UINT8 TimeOut;
UINT16 Time0con;
UINT8 acStatus[2];//Y�н���  N�޽���
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
UINT8 gsmreCount;//���ռ���
UINT8 ReadMessagecon;
UINT8 gsmBuf[200];
UINT8 gsmreStatus;//A:not receive  B:receive cmd   C:receive short message
Flag  gsmFlag,gsmFlag1;
UINT8 csqH,csqL;
UINT8 *Prt;//���ն���ָ��
UINT8 gsmPhoneNum[12];
UINT8 smCmd;
UINT8 phone1SM[6];
UINT8 phone2SM[6];
//phonexSM
/*
1:����		Y�н��� N�޽���  0xff������
2������		Y����ͨ	N�����  0xff������
3������		Y�й���			 0xff������
4���ȳ��	Y�ȳ��			 0xff������
5���׻�		Y���׻�			 0xff������
6����Դģʽ״̬  A��ֱ����  B����ֱ Cֱ���� D����ֱ E������ F������ 0xff������
*/
#pragma udata gpr4

