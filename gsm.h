#ifndef __GSM_H
#define __GSM_H

//变量声明 
extern unsigned char gsmreCount;//接收计数
extern signed char SCAOffset,OAOffset,UDOffset;
//函数声明
extern unsigned char gsmRec(unsigned char reData,unsigned char reDataQueue[]);
extern unsigned char gsmAna(unsigned char reDataQueue[],unsigned char MPhone[],unsigned char NPhone[],unsigned char NowPhone[]);
extern unsigned char gsmPhone(unsigned char reDataQueue[],unsigned char MPhone[],unsigned char NPhone[],unsigned char NowPhone[],unsigned char flag);
extern unsigned char gsmOC(unsigned char reDataQueue[]);
extern unsigned char gsmPA(unsigned char reDataQueue[]);
extern unsigned char gsmPhoneCon(unsigned char reDataQueue[],unsigned char MPhone[],unsigned char NPhone[],unsigned char NowPhone[]);  
extern unsigned char gsmData(unsigned char seDataQueue[],unsigned char Mdata[],unsigned char NowPhone[]);
extern unsigned char SendMessage2(unsigned char *str,unsigned char*buf, char Commend);
extern unsigned char phoneNumWoR(unsigned char Phone[]);
extern unsigned char lenConvert(unsigned char len[]);
extern void usartSendData(unsigned char  *pp,unsigned char m);
extern void My_Delay(unsigned char ConMs);
extern void My_Delay1(unsigned char ConMs);
extern char Distinguish_Send(unsigned char Cmd,unsigned char *buf ,unsigned char *Phone);
#endif