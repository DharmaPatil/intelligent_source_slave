#ifndef __GLOBLEVAR_H
#define __GLOBLEVAR_H
//mainbuf
extern UINT8 SspQueue[200];
extern UINT8 TimeOut;
extern UINT16 Time0con;
extern UINT8 SystemParameters[24];
extern UINT8 S_To_M_buf0[76];
extern UINT8 S_To_M_buf1[76];
extern Flag  sysFlag;
extern UINT8 acStatus[2];
extern UINT8 netStatus[2];
extern UINT8 troubleStatus[2];
extern UINT8 chargeStatus[2];
extern UINT8 lightStatus[2];
extern UINT8 modeStatus[2];
//gsm
extern UINT8 gsmBuf[200];
extern UINT8 ReadMessagecon;
extern UINT8 gsmreStatus;
extern Flag  gsmFlag,gsmFlag1;
extern UINT8 csqH,csqL;
extern UINT8 *Prt;
extern UINT8 gsmPhoneNum[12];
extern UINT8 smCmd;
extern UINT8 smAdd;
extern UINT8 phone1SM[6];
extern UINT8 phone2SM[6];
#endif