#include "main.h"
/*------------------------------------------------------------------------------------------
//���Ŵ�����
-------------------------------------------------------------------------------------------*/


//�������� 
//unsigned char reCount;//���ռ���
signed char SCAOffset,OAOffset,UDOffset;
//��������
unsigned char gsmRec(unsigned char reData,unsigned char reDataQueue[]);
unsigned char gsmAna(unsigned char reDataQueue[],unsigned char MPhone[],unsigned char NPhone[],unsigned char NowPhone[]);
unsigned char gsmPhone(unsigned char reDataQueue[],unsigned char MPhone[],unsigned char NPhone[],unsigned char NowPhone[],unsigned char flag);
unsigned char gsmOC(unsigned char reDataQueue[]);
unsigned char gsmPA(unsigned char reDataQueue[]);extern unsigned char gsmPhoneCon(unsigned char reDataQueue[],unsigned char MPhone[],unsigned char NPhone[],unsigned char NowPhone[]);  
unsigned char gsmData(unsigned char seDataQueue[],unsigned char Mdata[],unsigned char NowPhone[]);
unsigned char SendMessage2(unsigned char *str,unsigned char*buf, char Commend);
unsigned char phoneNumWoR(unsigned char Phone[]);
unsigned char lenConvert(unsigned char len[]);
void usartSendData(unsigned char  *pp,unsigned char m);
void My_Delay(unsigned char ConMs);
void My_Delay1(unsigned char ConMs);
char Distinguish_Send(unsigned char Cmd,unsigned char *buf ,unsigned char *Phone);
//**************************************************************************************
//�������ƣ�unsigned char gsmRec(unsigned char reData,unsigned char reDataQueue[]);
//�������������ն�������
//�������ڣ�
//��    ע����Ҫ����һ��ȫ�ֵĽ��ռ�������reCount�����������ʱ������1��δ��ɷ���0
//�޸����ڣ�
//�޸�˵����
//************************************************************************************** 
unsigned char gsmRec(unsigned char reData,unsigned char reDataQueue[])
{
	ClrWdt();
	reDataQueue[gsmreCount]=reData;
	if(reDataQueue[gsmreCount]=='+')
		{
			gsmreCount=1;
			reDataQueue[0]='+';
			return 0;
		}
	if(gsmreCount>6&&reDataQueue[gsmreCount]==0x0A&&reDataQueue[1]=='C'&&reDataQueue[2]=='M'&&reDataQueue[3]=='G'&&reDataQueue[4]=='R')
		{
			gsmreCount=1;
			reDataQueue[0]='$';	
			Nop();
			return 0;
		}
	if(reDataQueue[0]=='$'&&reDataQueue[gsmreCount-2]=='K'&&reDataQueue[gsmreCount-1]==0x0D&&reDataQueue[gsmreCount]==0x0A&&gsmreCount>31)
		{
			Nop();
			return 1;
		}
	if(gsmreCount>199)
		gsmreCount=0;
	gsmreCount++;
	ClrWdt();
	return 0;
}
//**************************************************************************************
//�������ƣ�unsigned char gsmAna(unsigned char reDataQueue[],unsigned char MPhone[],unsigned char NPhone[],unsigned char NowPhone[])
//�����������������ݽ���
//�������ڣ�
//��    ע:MPhone>33  NPhone>11 NowPhone>11,����ֵ����--0,��ô������   1����Ȩ  2������  
//�޸����ڣ�
//�޸�˵����
//************************************************************************************** 
unsigned char gsmAna(unsigned char reDataQueue[],unsigned char MPhone[],unsigned char NPhone[],unsigned char NowPhone[])
{
	unsigned char  gsmLen=0;
	unsigned char 	i,j,t;
	ClrWdt();
	
	gsmLen=lenConvert(reDataQueue+1);
	SCAOffset=(gsmLen-0x08)*2;
	t=21+SCAOffset;
	gsmLen=lenConvert(reDataQueue+t);
	OAOffset=(gsmLen-0x0D)+SCAOffset;
	UDOffset=OAOffset;
	//�������ݳ���
	t=57+UDOffset;
	gsmLen=lenConvert(reDataQueue+t);
	gsmLen/=2;


	Nop();
    gsmPhoneCon(reDataQueue,MPhone,NPhone,NowPhone);//��ȡ�ֻ���
	if(!phoneNumWoR(NowPhone))	return 0xFF;		
	switch(gsmLen)
	{
		case 0x03:			//3������
					{
						t=59+UDOffset;
						i=gsmOC(reDataQueue+t);	//��������
						if(i==0) return 0;
						else if(i==2) return 2;
						j=gsmPhoneCon(reDataQueue,MPhone,NPhone,NowPhone);//�ж��ֻ���
						if(j!=0) return i;		//���������Ȩ�ֻ��������ִ��
						else return 1;			//���Ƿ�����Ȩ
						break;
					}
		case 0x0D:			//13������
					{
						i=gsmPhoneCon(reDataQueue,MPhone,NPhone,NowPhone);//�ж��ֻ���
						if(i==0) return 1;
						t=59+UDOffset;
						j=gsmPhone(reDataQueue+t,MPhone,NPhone,NowPhone,i);//��д�ֻ���
						return j;
						break;
					}

		default:  return 2;
	}
	ClrWdt();	
}
//**************************************************************************************
//�������ƣ�unsigned char gsmOC(unsigned char gsmLen,unsigned char reDataQueue[])
//��������������3�������ж�
//�������ڣ�
//��    ע: reDataQueueΪ��Ч���ݣ������벿�֣������жϺ������,0--��ô����2--��������������ѭ����ʦЭ��
//�޸����ڣ�
//�޸�˵����
//************************************************************************************** 
unsigned char gsmOC(unsigned char reDataQueue[])
{
	//��������
	//����
	if(reDataQueue[0]=='6'&&reDataQueue[1]=='0'&&reDataQueue[2]=='0'&&reDataQueue[3]=='E'
		&&reDataQueue[4]=='4'&&reDataQueue[5]=='E'&&reDataQueue[6]=='4'&&reDataQueue[7]=='8'
		&&reDataQueue[8]=='6'&&reDataQueue[9]=='8'&&reDataQueue[10]=='3'&&reDataQueue[11]=='7'	)		//��ô��
		{
			ClrWdt();
			return 0;
		}
	else if(reDataQueue[0]=='4'&&reDataQueue[1]=='E'&&reDataQueue[2]=='A'&&reDataQueue[3]=='4')	//��
		{
            ClrWdt();
			if(reDataQueue[4]=='7'&&reDataQueue[5]=='6'&&reDataQueue[6]=='F'&&reDataQueue[7]=='4')//ֱ
			{
				if(reDataQueue[8]=='4'&&reDataQueue[9]=='F'&&reDataQueue[10]=='9'&&reDataQueue[11]=='B')		//��
				{
	        		return 'G';
					Nop();
				}
				
			}
			else if(reDataQueue[4]=='9'&&reDataQueue[5]=='6'&&reDataQueue[6]=='9'&&reDataQueue[7]=='4')//��
			{
				if(reDataQueue[8]=='4'&&reDataQueue[9]=='E'&&reDataQueue[10]=='0'&&reDataQueue[11]=='0')		//һ
				{
	        		return 'H';
					Nop();
				}
				else if(reDataQueue[8]=='4'&&reDataQueue[9]=='E'&&reDataQueue[10]=='8'&&reDataQueue[11]=='C')	//��
				{
	        		return 'I';
				}
			}
			else if(reDataQueue[4]=='6'&&reDataQueue[5]=='D'&&reDataQueue[6]=='6'&&reDataQueue[7]=='E')//��
			{
				if(reDataQueue[8]=='4'&&reDataQueue[9]=='E'&&reDataQueue[10]=='0'&&reDataQueue[11]=='0')		//һ
				{
	        		return  'J';
				}
				else if(reDataQueue[8]=='4'&&reDataQueue[9]=='E'&&reDataQueue[10]=='8'&&reDataQueue[11]=='C')	//��
				{
	        		return  'K';
				}
			}
		}		
	else if(reDataQueue[0]=='5'&&reDataQueue[1]=='1'&&reDataQueue[2]=='4'&&reDataQueue[3]=='9')	//��
		{
            ClrWdt();
			if(reDataQueue[4]=='9'&&reDataQueue[5]=='6'&&reDataQueue[6]=='9'&&reDataQueue[7]=='4')//��
			{
				if(reDataQueue[8]=='4'&&reDataQueue[9]=='E'&&reDataQueue[10]=='0'&&reDataQueue[11]=='0')		//һ
				{
	        		return 'L';
				}
				else if(reDataQueue[8]=='4'&&reDataQueue[9]=='E'&&reDataQueue[10]=='8'&&reDataQueue[11]=='C')	//��
				{
	        		return 'M';
				}
			}
			else if(reDataQueue[4]=='6'&&reDataQueue[5]=='D'&&reDataQueue[6]=='6'&&reDataQueue[7]=='E')//��
			{
				if(reDataQueue[8]=='4'&&reDataQueue[9]=='E'&&reDataQueue[10]=='0'&&reDataQueue[11]=='0')		//һ
				{
	        		return 'R';
				}
				else if(reDataQueue[8]=='4'&&reDataQueue[9]=='E'&&reDataQueue[10]=='8'&&reDataQueue[11]=='C')	//��
				{
	        		return 'O';
				}
			}
		}
	else if(reDataQueue[0]=='6'&&reDataQueue[1]=='5'&&reDataQueue[2]=='E'&&reDataQueue[3]=='0')	//��
		{
            ClrWdt();
			if(reDataQueue[4]=='9'&&reDataQueue[5]=='6'&&reDataQueue[6]=='9'&&reDataQueue[7]=='4')//��
			{
				if(reDataQueue[8]=='4'&&reDataQueue[9]=='E'&&reDataQueue[10]=='0'&&reDataQueue[11]=='0')		//һ
				{
	        		return 'P';
				}
				else if(reDataQueue[8]=='4'&&reDataQueue[9]=='E'&&reDataQueue[10]=='8'&&reDataQueue[11]=='C')	//��
				{
	        		return 'Q';
				}
			}
		}
	else if(reDataQueue[0]=='5'&&reDataQueue[1]=='4'&&reDataQueue[2]=='0'&&reDataQueue[3]=='8')	//��
		{
            ClrWdt();
			if(reDataQueue[4]=='8'&&reDataQueue[5]=='F'&&reDataQueue[6]=='7'&&reDataQueue[7]=='D')//��
			{
				if(reDataQueue[8]=='4'&&reDataQueue[9]=='E'&&reDataQueue[10]=='0'&&reDataQueue[11]=='0')		//һ
				{
	        		return 'A';
				}
				else if(reDataQueue[8]=='4'&&reDataQueue[9]=='E'&&reDataQueue[10]=='0'&&reDataQueue[11]=='9')	//��
				{
	        		return 'N';
				}
				else if(reDataQueue[8]=='5'&&reDataQueue[9]=='6'&&reDataQueue[10]=='D'&&reDataQueue[11]=='B')	//��
				{
	        		return 'T';
				}
			}
		}
	else if(reDataQueue[0]=='6'&&reDataQueue[1]=='5'&&reDataQueue[2]=='A'&&reDataQueue[3]=='D')	//��
		{
            ClrWdt();
			if(reDataQueue[4]=='8'&&reDataQueue[5]=='F'&&reDataQueue[6]=='7'&&reDataQueue[7]=='D')//��
			{
				if(reDataQueue[8]=='4'&&reDataQueue[9]=='E'&&reDataQueue[10]=='0'&&reDataQueue[11]=='0')		//һ
				{
	        		return 'B';
				}
				else if(reDataQueue[8]=='4'&&reDataQueue[9]=='E'&&reDataQueue[10]=='8'&&reDataQueue[11]=='C')	//��
				{
	        		return 'C';
				}
				else if(reDataQueue[8]=='4'&&reDataQueue[9]=='E'&&reDataQueue[10]=='0'&&reDataQueue[11]=='9')	//��
				{
	        		return 'S';
				}
				else if(reDataQueue[8]=='5'&&reDataQueue[9]=='6'&&reDataQueue[10]=='D'&&reDataQueue[11]=='B')	//��
				{
	        		return 'U';
				}
			}
			if(reDataQueue[4]=='9'&&reDataQueue[5]=='0'&&reDataQueue[6]=='0'&&reDataQueue[7]=='6')//��
				if(reDataQueue[8]=='5'&&reDataQueue[9]=='3'&&reDataQueue[10]=='D'&&reDataQueue[11]=='8')	//��
				{
	        		return 'D';
				}
		}
	else if(reDataQueue[0]=='4'&&reDataQueue[1]=='E'&&reDataQueue[2]=='3'&&reDataQueue[3]=='B')	//��
		{
            ClrWdt();
			if(reDataQueue[4]=='5'&&reDataQueue[5]=='9'&&reDataQueue[6]=='0'&&reDataQueue[7]=='D')//��
			{
				if(reDataQueue[8]=='4'&&reDataQueue[9]=='F'&&reDataQueue[10]=='4'&&reDataQueue[11]=='D')		//λ
				{
	        		return 'E';
				}
			}
		}
	else if(reDataQueue[0]=='4'&&reDataQueue[1]=='E'&&reDataQueue[2]=='C'&&reDataQueue[3]=='E')	//��
		{
            ClrWdt();
			if(reDataQueue[4]=='5'&&reDataQueue[5]=='9'&&reDataQueue[6]=='0'&&reDataQueue[7]=='D')//��
			{
				if(reDataQueue[8]=='4'&&reDataQueue[9]=='F'&&reDataQueue[10]=='4'&&reDataQueue[11]=='D')		//λ
				{
	        		return 'F';
				}
			}
		}
	return 2;		//����
	ClrWdt();
}
//**************************************************************************************
//�������ƣ�unsigned char gsmPhone(unsigned char reDataQueue[],unsigned char MPhone[],unsigned char NPhone[],unsigned char NowPhone[],unsigned char flag)
//��������������13�������ж�,�޸��ֻ���
//�������: V�����ֻ�1  W�����ֻ�2  X�����ֻ�3 ��Ȩ�ֻ�
//�������ڣ�
//��    ע: reDataQueueΪ��Ч���ݣ������벿�֣������жϺ������
//�޸����ڣ�
//�޸�˵����
//************************************************************************************** 
unsigned char gsmPhone(unsigned char reDataQueue[],unsigned char MPhone[],unsigned char NPhone[],unsigned char NowPhone[],unsigned char flag)
{
	unsigned char i,j;
	ClrWdt();
	if(reDataQueue[0]=='4'&&reDataQueue[1]=='E'&&reDataQueue[2]=='3'&&reDataQueue[3]=='B'&&flag!=0)	//��
		{
			if(reDataQueue[4]=='0'&&reDataQueue[5]=='0'&&reDataQueue[6]=='3'&&reDataQueue[7]=='1')//1
				{
					j=11;
					for(i=0;i<11;i++)
						{
							if(reDataQueue[j+i*4]<'0'||reDataQueue[j+i*4]>'9')
							return 2;							
						}
					for(i=0;i<11;i++)
						{
							MPhone[i]=reDataQueue[j+i*4];
						}
					return 'V';
				}
			else if(reDataQueue[4]=='0'&&reDataQueue[5]=='0'&&reDataQueue[6]=='3'&&reDataQueue[7]=='2')//2
				{
					j=11;
					for(i=0;i<11;i++)
						{
							if(reDataQueue[j+i*4]<'0'||reDataQueue[j+i*4]>'9')
							return 2;
						}
					for(i=0;i<11;i++)
						{
							MPhone[i+11]=reDataQueue[j+i*4];
						}
					return 'W';
				}
/*			else if(reDataQueue[4]=='0'&&reDataQueue[5]=='0'&&reDataQueue[6]=='3'&&reDataQueue[7]=='3')//3
				{
					j=11;
					for(i=0;i<11;i++)
						MPhone[i+22]=reDataQueue[j+i*4];
					return 'X';
				}*/
		}
	else if(reDataQueue[0]=='6'&&reDataQueue[1]=='7'&&reDataQueue[2]=='4'&&reDataQueue[3]=='3')	//Ȩ
		{
			if(reDataQueue[4]=='0'&&reDataQueue[5]=='0'&&reDataQueue[6]=='3'&&reDataQueue[7]=='1')//1
				{
					if(flag!=4) return 1;
					j=11;
					for(i=0;i<11;i++)
						{
							if(reDataQueue[j+i*4]<'0'||reDataQueue[j+i*4]>'9')
							return 2;
						}
					for(i=0;i<11;i++)
						NPhone[i]=reDataQueue[j+i*4];
					return 'Y';
				}
		}
	else
			return 2;		//����
}
//**************************************************************************************
//�������ƣ�unsigned char gsmPA(unsigned char reDataQueue[])
//����������������Ķ������� ������֤
//�������ڣ�
//��    ע: reDataQueueΪȫ�����ݣ�����1--������ȷ��0--�������
//�޸����ڣ�
//�޸�˵����
//************************************************************************************** 
unsigned char gsmPA(unsigned char reDataQueue[])
{
	unsigned char date[4],i;
		//�������� �� ��abcd,a=a*b%10
		date[0]=((reDataQueue[46]-'0')*(reDataQueue[45]-'0'));
		date[1]=((reDataQueue[45]-'0')*(reDataQueue[48]-'0'));
		date[2]=((reDataQueue[48]-'0')*(reDataQueue[47]-'0'));
		date[3]=((reDataQueue[47]-'0')*(reDataQueue[46]-'0'));
		date[0]=date[0]%10+'0';
		date[1]=date[1]%10+'0';
		date[2]=date[2]%10+'0';
		date[3]=date[3]%10+'0';
	for(i=0;i<4;i++)
		{
			if(date[i]!=reDataQueue[62+i*4]) return 0; 
		}
	return 1;
}
//**************************************************************************************
//�������ƣ�unsigned char gsmPhoneCon(unsigned char reDataQueue[],unsigned char MPhone[],unsigned char NPhone[],unsigned char NowPhone[])
//������������ǰ�ֻ��ź�
//�������ڣ�
//��    ע: reDataQueueΪȫ�����ݣ�����0--��Ȩ�ֻ���1--���ֻ�1��2--���ֻ�2��3--���ֻ�3��4--Ȩ���ֻ�1
//�޸����ڣ�
//�޸�˵����
//************************************************************************************** 
unsigned char gsmPhoneCon(unsigned char reDataQueue[],unsigned char MPhone[],unsigned char NPhone[],unsigned char NowPhone[])
{
	unsigned char i,j;
	ClrWdt();
//�������ǰ�ֻ���
	NowPhone[0]=reDataQueue[28+OAOffset];	
	NowPhone[1]=reDataQueue[27+OAOffset];
	NowPhone[2]=reDataQueue[30+OAOffset];
	NowPhone[3]=reDataQueue[29+OAOffset];
	NowPhone[4]=reDataQueue[32+OAOffset];
	NowPhone[5]=reDataQueue[31+OAOffset];
	NowPhone[6]=reDataQueue[34+OAOffset];
	NowPhone[7]=reDataQueue[33+OAOffset];
	NowPhone[8]=reDataQueue[36+OAOffset];
	NowPhone[9]=reDataQueue[35+OAOffset];
	NowPhone[10]=reDataQueue[38+OAOffset];
//������Ȩ�ֻ��ŶԱ�
	for(j=0;j<11;j++)		//Ȩ���ֻ��ж�
	{
		if(NowPhone[j]!=NPhone[j]) break;
	}
	if(j==11) return 4;
	for(i=0;i<2;i++)		//���ֻ��ж�
	{
		for(j=0;j<11;j++)
		{
			if(NowPhone[j]!=MPhone[j+i*11]) break;
		}
		if(j==11) return (i+1);
	}

	return 0;
}




//**************************************************************************************
//�������ƣ�unsigned char SendMessage2(unsigned char *str, char Commend)
//����������ֻ����׵�ַstr   ���Ͷ��� ���� Commend   ����Ϊ200���������ַ *buf
//����������ɹ� 1   ʧ�� 0
//����������
//�������ڣ�
//**************************************************************************************
unsigned char SendMessage2(unsigned char *str,unsigned char*buf, char Commend)
	{
		ClrWdt();
//		printf((const far rom char *)"%c\r\n",0X1A );
        putcUSART(0X1A);
		putcUSART('\r');
		putcUSART('\n');
		My_Delay(1);
		memset(buf,0,200);         //��buf�������ڽ������
        putrsUSART((const far rom char *)"AT+CMGS=18\r");////ԭ"AT+CMGS=18\r\n"
        My_Delay(3);							                //waiting return '>'
       if(strchr(  (const far char *)(buf),'>' ) == NULL)                                          
    	{
         return 0;								         //û�յ� Ӧ������
    	}
	    memset(buf,0,200);                                  
	    memcpypgm2ram(buf,(const far rom char *)"0011000B8100000000000000081004",31);  //д��"0011000B8100000000000000081004";
        //gsmInvertNumbers((char* )str,(char* )(buf+10),11);//д���ֻ���//begin from "0" after "81"
        *(buf+10) = *(str+1);
        *(buf+11) = *str;
        *(buf+12) = *(str+3);
        *(buf+13) = *(str+2);
        *(buf+14) = *(str+5);
        *(buf+15) = *(str+4);
        *(buf+16) = *(str+7);
        *(buf+17) = *(str+6);
        *(buf+18) = *(str+9);
        *(buf+19) = *(str+8);
        *(buf+20) = 'F';
        *(buf+21) = *(str+10);
	    switch (Commend)
	    {
	    	case '2':                                                      //ִ��
	         	memcpypgm2ram((buf+30),(const far rom char *)"6267884C",8);
		 		break;
	     	case '3':                                                     //��Ȩ
		 		memcpypgm2ram((buf+30),(const far rom char *)"65E06743",8);
		 		break;
		 	case '4':                                                      //����
		 		memcpypgm2ram((buf+30),(const far rom char *)"95198BEF",8);
		 		break;
		 	case '5':                                                      //�г�
		 		memcpypgm2ram((buf+30),(const far rom char *)"67095145",8);
		 		break;
		 	case '6':                                                      //�޳�
		 		memcpypgm2ram((buf+30),(const far rom char *)"65E05145",8);
		 		break;
		 	case '7':                                                      //�ȳ�
		 		memcpypgm2ram((buf+30),(const far rom char *)"7B495145",8);
		 		break;
		 	case '8':                                                      //�Ƚ�
		 		memcpypgm2ram((buf+30),(const far rom char *)"7B494EA4",8);
		 		break;
		 	case '9':                                                      //��һ
		 		memcpypgm2ram((buf+30),(const far rom char *)"7B494E00",8);
		 		break;
		 	case '0':                                                      //�ȶ�
		 		memcpypgm2ram((buf+30),(const far rom char *)"7B494E8C",8);
		 		break;
		 	case 'a':                                                      //����
		 		memcpypgm2ram((buf+30),(const far rom char *)"7B494E09",8);
		 		break;
		 	case 'b':                                                      //�ɹ�
		 		memcpypgm2ram((buf+30),(const far rom char *)"6210529F%c",8);
		 		break;
		 	case 'c':                                                      //ʧ��
		 		memcpypgm2ram((buf+30),(const far rom char *)"59318D25%c",8);
		 		break;
         	case 'd':                                                      //����
		 		memcpypgm2ram((buf+30),(const far rom char *)"6545969C",8);
		 		break;
		 	case 'e':                                                      //�´�
		 		memcpypgm2ram((buf+30),(const far rom char *)"4E0B6B21",8);
		 		break;
         	default  :                                                     //����
		 		memcpypgm2ram((buf+30),(const far rom char *)"6545969C",8);
         		break;
	    }
	    *(buf+38)= 0x1a;
	    //GSM_BUF[39] = '\r';
	    //GSM_BUF[40] = '\n';
	    *(buf+39) = '\0';
//	    gsmOKFlag = 0;
//		gsmERRORFlag = 0;
//		TimeOut = 0;
        putsUSART((char *)buf);
/*        while(1)
        {
	        if(OKFlag)
	        return 1;
			else if(ERRORFlag)
			return 0;
			else if(TimeOut >= 60)//LJ
			return 0;
			//���� ���ԼӸ� 60�볬ʱ ��ͣ ����
		   ClrWdt(); 
		}
*/
}

//**************************************************************************************
//�������ƣ�void Distinguish_Send(void)
//���������Cmd ���� *buf
//��������� �ɹ�����1 ʧ�ܷ���0
//��    ע��
//�޸�˵����
//����������GSM���������ֻ�������
//**************************************************************************************
char Distinguish_Send(unsigned char Cmd,unsigned char *buf ,unsigned char *Phone)
{
	unsigned char i;
		ClrWdt();
//		printf((const far rom char *)"%c\r\n",0X1A );
		putcUSART(0X1A);
		putcUSART('\r');
		putcUSART('\n');
		My_Delay(1);
		memset(buf,0,200);                               //��buf�������ڽ������
        putrsUSART((const far rom char *)"AT+CMGS=20\r");//ԭ"AT+CMGS=18\r\n"
        My_Delay(3);							         //waiting return '>'
//    	if(strchr( (const far char *)( buf),'>' ) == NULL)                                          
//    	{
//         return 0;								         //û�յ� Ӧ������
//    	}
    for(i=0;i<200;i++)
    {
	    if(buf[i] == '>')
	    break;
	    
    }
    if(i>199)
    return 0;
    memcpypgm2ram(buf,(const far rom char *)"0011000B8100000000000000081006",31);
	//gsmInvertNumbers((char* )Phone,(char* )(buf+10),11);
    //gsmInvertNumbers((char* )str,(char* )(buf+10),11);//д���ֻ���//begin from "0" after "81"
        *(buf+10) = *(Phone+1);
        *(buf+11) = *Phone;
        *(buf+12) = *(Phone+3);
        *(buf+13) = *(Phone+2);
        *(buf+14) = *(Phone+5);
        *(buf+15) = *(Phone+4);
        *(buf+16) = *(Phone+7);
        *(buf+17) = *(Phone+6);
        *(buf+18) = *(Phone+9);
        *(buf+19) = *(Phone+8);
        *(buf+20) = 'F';
        *(buf+21) = *(Phone+10);
        switch (Cmd)
        {
                case 'A': 		//�ȳ��
                                strcatpgm2ram((char *)buf,(const far rom char *)"7B4951457535");
                                break;
                case 'B': 		//�н��� 
                                strcatpgm2ram((char *)buf,(const far rom char *)"67094EA46D41");
                                break;
                case 'C':       //�޽���
                                strcatpgm2ram((char *)buf,(const far rom char *)"65E04EA46D41");
                                break;
                case 'D': 		//�й���
                                strcatpgm2ram((char *)buf,(const far rom char *)"6709514980FD");
                                break;
                case 'E':       //�޹���
                                strcatpgm2ram((char *)buf,(const far rom char *)"65E0514980FD");
                                break;
                case 'F':       //����ͨ
                                strcatpgm2ram((char *)buf,(const far rom char *)"7F517EDC901A");
                                break;
                case 'G':       //�����
                                strcatpgm2ram((char *)buf,(const far rom char *)"7F517EDC65AD");
                                break;
                case 'H':       //�й���
                                strcatpgm2ram((char *)buf,(const far rom char *)"67096545969C");
                                break;
                case 'I':       //�޹���
                                strcatpgm2ram((char *)buf,(const far rom char *)"65E06545969C");
                                break;
                case 'J':       //���׻�
                                strcatpgm2ram((char *)buf,(const far rom char *)"670996F751FB");
                                break;
                case 'K':       //����ֱ
                                strcatpgm2ram((char *)buf,(const far rom char *)"9694523076F4");
                                break;
                case 'L':       //ֱ����
                                strcatpgm2ram((char *)buf,(const far rom char *)"76F452309694");
                                break;
                case 'M':       //ֱ����
                                strcatpgm2ram((char *)buf,(const far rom char *)"76F452306D6E");
                                break;
                case 'N':       //����ֱ
                                strcatpgm2ram((char *)buf,(const far rom char *)"6D6E523076F4");
                                break;                 
                case 'O':       //������
                                strcatpgm2ram((char *)buf,(const far rom char *)"969452306D6E");
                                break;
                case 'P':       //������
                                strcatpgm2ram((char *)buf,(const far rom char *)"6D6E52309694");
                                break;                    
		}
		*(buf+42) = 0x1a;
		*(buf+43) = '\0';
//		OKFlag = 0;
//		ERRORFlag = 0;
//		TimeOut = 0;
		putsUSART((char *)buf);
/*        while(1)
        {
	        if(OKFlag)
	        return 1;
			else if(ERRORFlag)
			return 0;
			else if(TimeOut >= 60)//LJ��
			return 0;
			//���� ���ԼӸ� 60�볬ʱ ��ͣ ����
		   ClrWdt(); 
		}*/
}


//**************************************************************************************
//�������ƣ�unsigned char gsmData(unsigned char seDataQueue[],unsigned char Mdata[],unsigned char NowPhone[])
//����������
//�������ڣ�
//��    ע: seDataQueueΪ���͵����ݻ��棬Mdata[]Ϊ��������ͨѶ���ݵĵ�ַ������0--����ʧ�ܣ�1--���ͳɹ�
//			��Ҫ����һ��ȫ�ֵķ��ͳɹ���־����seOKflag
//�޸����ڣ�
//�޸�˵����
//************************************************************************************** 
unsigned char gsmData(unsigned char seDataQueue[],unsigned char Mdata[],unsigned char NowPhone[])
{	
	unsigned char i=1,j=0,k=0,normalFlag=1;
	unsigned char s[2]={0x1A,'\0'};
	ClrWdt(); 
//	OKFlag = 0;
//	ERRORFlag = 0;
//	TimeOut = 0;
	usartSendData(s,1);
/*	while(1)
    {
		if(OKFlag)
	    break;
		if(ERRORFlag)
		break;
		ClrWdt(); 
		if(TimeOut >= 3)
		break;
    }
*/
	if(Mdata[177]=='A')	normalFlag=1;
	else normalFlag=0;
	Nop();
		   
			//3��:050003390201 64��:��220V��13.0V��14.0V05.0A��12.0V03.0A��һ12.0V2.00A�ض�12.0V2.00A����12.0V
			ClrWdt();
		    My_Delay(1);
			ClrWdt();
	        putrsUSART((const far rom char *)("AT+CMGS=149\r"));//15+134=149
		    My_Delay(3);
			ClrWdt();
			memset(seDataQueue,0,200);//��ջ���
		//��ֵ����ͷ
			strcatpgm2ram((char *)seDataQueue,(const far rom char *)"0051000D9168");
		//��ֵ�ֻ�����
			seDataQueue[12]=NowPhone[1];
			seDataQueue[13]=NowPhone[0];
			seDataQueue[14]=NowPhone[3];
			seDataQueue[15]=NowPhone[2];
			seDataQueue[16]=NowPhone[5];
			seDataQueue[17]=NowPhone[4];
			seDataQueue[18]=NowPhone[7];
			seDataQueue[19]=NowPhone[6];
			seDataQueue[20]=NowPhone[9];
			seDataQueue[21]=NowPhone[8];
			seDataQueue[22]='F';
			seDataQueue[23]=NowPhone[10];
			strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"00080186050003520201");
			//start: seDataQueue+44
			//44:��2(51)2(55)0(59)V
			strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"4EA40032003200300056");
			seDataQueue[51]=Mdata[15];
			seDataQueue[55]=Mdata[16];
			seDataQueue[59]=Mdata[17];
			Nop();
			//64:��1(71)3(75).0(83)V
			strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"514900310033002E00300056");
			seDataQueue[71]=Mdata[18];
			seDataQueue[75]=Mdata[19];
			seDataQueue[83]=Mdata[21];
			Nop();
			//88:��1(95)4(99).0(107)V0(115)5(119).0(127)A
			strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"514500310034002E0030005600300035002E00300041");
			seDataQueue[95]=Mdata[22];
			seDataQueue[99]=Mdata[23];
			seDataQueue[107]=Mdata[25];
			seDataQueue[115]=Mdata[26];
			seDataQueue[119]=Mdata[27];
			seDataQueue[127]=Mdata[29];
			Nop();
			//132:��1(139)2(143).0(151)V0(159)3(163).0(171)A    end-(175)
			strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"4F9B00310032002E0030005600300033002E00300041");
			seDataQueue[139]=Mdata[30];
			seDataQueue[143]=Mdata[31];
			seDataQueue[151]=Mdata[33];
			seDataQueue[159]=Mdata[34];
			seDataQueue[163]=Mdata[35];
			seDataQueue[171]=Mdata[37];
			usartSendData(seDataQueue,176);//����176��
		//	putsUSART(seDataQueue);
			Nop();
			//start 0:��һ1(11)2(15).0(23)V2(31).0(39)0(43)A
			memset(seDataQueue,0,200);//�������
			strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"8F7D4E0000310032002E003000560032002E003000300041");
			seDataQueue[11]=Mdata[38];
			seDataQueue[15]=Mdata[39];
			seDataQueue[23]=Mdata[41];
			seDataQueue[31]=Mdata[42];
			seDataQueue[39]=Mdata[44];
			seDataQueue[43]=Mdata[45];
			Nop();
			//48:�ض�1(59)2(63).0(71)V2(79).0(87)0(91)A  
			strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"8F7D4E8C00310032002E003000560032002E003000300041");
			seDataQueue[59]=Mdata[46];
			seDataQueue[63]=Mdata[47];
			seDataQueue[71]=Mdata[49];
			seDataQueue[79]=Mdata[50];
			seDataQueue[87]=Mdata[52];
			seDataQueue[91]=Mdata[53];

			//96:����1(107)2(111).0(119)V  end-(123)
			strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"8F7D4E0900310032002E00300056");
			seDataQueue[107]=Mdata[54];
			seDataQueue[111]=Mdata[55];
			seDataQueue[119]=Mdata[57];
			Nop();



			//96:����1(107)2(111).0(119)V
/*			strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"8F7D4E9400310032002E00300056");
			seDataQueue[107]=Mdata[98];
			seDataQueue[111]=Mdata[99];
			seDataQueue[119]=Mdata[101];
			Nop();*/
			usartSendData(seDataQueue,124);//����124��
			Nop();
			ClrWdt();
//	    	OKFlag = 0;
//			ERRORFlag = 0;
//			TimeOut = 0;
   			usartSendData(s,1);
/*        	while(!OKFlag)                   //���ͳɹ������� ������������ ���ɹ�����0  ��ʱ����0
        	{
				if(ERRORFlag)
				return 0;
				if(TimeOut >= 60)
				return 0;
				//���� ���ԼӸ� 10�볬ʱ ��ͣ ����
		   		ClrWdt(); 
			}
			Nop();
*/

			if(normalFlag==1)
			{
				//3��:050003390202 61��:2.00A�Ŵ���������������ˮ��������+24��ʪ22%����+28������000/000/000/000��ԴAƿ������F000
			    My_Delay(1);
		        putrsUSART((const far rom char *)("AT+CMGS=143\r"));//15+128=143//���ĳ�����Ҫ�޸�
			    My_Delay(3);
				memset(seDataQueue,0,200);//��ջ���
				//��ֵ����ͷ
				strcatpgm2ram((char *)seDataQueue,(const far rom char *)"0051000D9168");
				//��ֵ�ֻ�����
				seDataQueue[12]=NowPhone[1];
				seDataQueue[13]=NowPhone[0];
				seDataQueue[14]=NowPhone[3];
				seDataQueue[15]=NowPhone[2];
				seDataQueue[16]=NowPhone[5];
				seDataQueue[17]=NowPhone[4];
				seDataQueue[18]=NowPhone[7];
				seDataQueue[19]=NowPhone[6];
				seDataQueue[20]=NowPhone[9];
				seDataQueue[21]=NowPhone[8];
				seDataQueue[22]='F';
				seDataQueue[23]=NowPhone[10];			    
				strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"00080180050003520202");//���ĳ�����Ҫ�޸�
				Nop();
			}
			else if(normalFlag==0)
			{
				//3��:050003390202 63��:2.00A����12.0V2.00A����+24��ʪ22%����+28�潻ԴAƿ������F000�ȳ�
			    My_Delay(1);
		        putrsUSART((const far rom char *)("AT+CMGS=147\r"));//15+132=147//���ĳ�����Ҫ�޸�
			    My_Delay(3);
			    		memset(seDataQueue,0,200);//��ջ���
		//��ֵ����ͷ
				strcatpgm2ram((char *)seDataQueue,(const far rom char *)"0051000D9168");
				//��ֵ�ֻ�����
				seDataQueue[12]=NowPhone[1];
				seDataQueue[13]=NowPhone[0];
				seDataQueue[14]=NowPhone[3];
				seDataQueue[15]=NowPhone[2];
				seDataQueue[16]=NowPhone[5];
				seDataQueue[17]=NowPhone[4];
				seDataQueue[18]=NowPhone[7];
				seDataQueue[19]=NowPhone[6];
				seDataQueue[20]=NowPhone[9];
				seDataQueue[21]=NowPhone[8];
				seDataQueue[22]='F';
				seDataQueue[23]=NowPhone[10];
				strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"00080184050003520202");//���ĳ�����Ҫ�޸�
				Nop();
			}
			//start: seDataQueue+44
			//start: seDataQueue+44
			//44:2(47).0(55)0(59)A
			strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"0032002E003000300041");
			seDataQueue[47]=Mdata[58];
			seDataQueue[55]=Mdata[60];
			seDataQueue[59]=Mdata[61];
			Nop();



			//44:2(47).0(55)0(59)A
/*			strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"0032002E003000300041");
			seDataQueue[47]=Mdata[102];
			seDataQueue[55]=Mdata[104];
			seDataQueue[59]=Mdata[105];
			Nop();*/
			//64:�Ŵ���������������ˮ����
			if(Mdata[86]=='Y')  //�Ŵ�����
			{
				strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"95E878C16B635E38");				
			}
			else if(Mdata[86]=='N')//�Ŵ�����
			{
				strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"95E878C151654FB5");				
			}

			if(Mdata[85]=='Y')//��������
			{
				strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"7EA259166B635E38");				
			}
			else if(Mdata[85]=='N')//��������
			{
				strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"7EA2591651654FB5");				
			}

			if(Mdata[87]=='Y')//��ˮ����
			{
				strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"6D786C346B635E38");				
			}
			else if(Mdata[87]=='N')//��ˮ����
			{
				strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"6D786C3462A58B66");				
			}
			Nop();
			//112:����+(123)2(127)4(131)��
			strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"73AF6E29002B003200342103");
			if(Mdata[63]=='X')
			{
				seDataQueue[122]='5';
				seDataQueue[123]='8';
				seDataQueue[126]='5';
				seDataQueue[127]='8';
				seDataQueue[130]='5';
				seDataQueue[131]='8';	
			}
			else
			{
				if(Mdata[62]=='+')	
					seDataQueue[123]='B';
				else if(Mdata[62]=='-')	
					seDataQueue[123]='D';
				seDataQueue[127]=Mdata[63];
				seDataQueue[131]=Mdata[64];
			}
			Nop();
			//136:ʪ2(143)2(147)%  end--(151)
			strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"6E7F003200320025");
			if(Mdata[65]=='X')
			{
				seDataQueue[142]='5';
				seDataQueue[143]='8';
				seDataQueue[146]='5';
				seDataQueue[147]='8';	
			}
			else
			{
				seDataQueue[143]=Mdata[65];
				seDataQueue[147]=Mdata[66];
			}
			Nop();


			usartSendData(seDataQueue,152);//����152��
			Nop();
			//start 0:����+(11)2(15)8(19)��
			memset(seDataQueue,0,200);//�������
			strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"8BBE6E29002B003200382103");
			if(Mdata[67]=='+')	
				seDataQueue[11]='B';
			else if(Mdata[67]=='-')	
				seDataQueue[11]='D';
			seDataQueue[15]=Mdata[68];
			seDataQueue[19]=Mdata[69];
			Nop();
			//24:����(32)0(36)0(40)0/(48)0(52)0(56)0/(64)0(68)0(72)0/(80)0(84)0(88)0--end(91)
			strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"5185963B003000300030002F003000300030002F003000300030002F003000300030");
			for(j=0;j<4;j++)
			{
				if(j==0) i=70;
				else if(j==1) i=72;
				else if(j==2) i=106;
				else if(j==3) i=108;
				if(Mdata[i]=='X')
				{
					seDataQueue[34+j*16]=seDataQueue[34+4+j*16]=seDataQueue[34+8+j*16]='5';
					seDataQueue[35+j*16]=seDataQueue[35+4+j*16]=seDataQueue[35+8+j*16]='8';
				}
				else 
				{
					if(j==0) k=83;
					else if(j==1) k=84;
					else if(j==2) k=110;
					else if(j==3) k=111;
					if(Mdata[k]=='C')
					{
						seDataQueue[34+j*16]='4';
						seDataQueue[35+j*16]='8';
					}
					seDataQueue[35+4+j*16]=Mdata[i];
					seDataQueue[35+8+j*16]=Mdata[i+1];	
				}
			}			

			i=68;
			//24+i:��ԴAƿ������--51	/24:��Դ��0(39).0(47)ʱ--51
			if(Mdata[75]=='.')		//���
			{
				strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"4EA46E9051450030002E003065F6");
				if(Mdata[178]=='A')		//��
					{
						seDataQueue[24+i]='4';
						seDataQueue[25+i]='E';
						seDataQueue[26+i]='A';
						seDataQueue[27+i]='4';						
					}
				else if(Mdata[178]=='S')	//��
					{
						seDataQueue[24+i]='5';
						seDataQueue[25+i]='1';
						seDataQueue[26+i]='4';
						seDataQueue[27+i]='9';						
					}
				else if(Mdata[178]==0)	//��
					{
						seDataQueue[24+i]='6';
						seDataQueue[25+i]='5';
						seDataQueue[26+i]='E';
						seDataQueue[27+i]='0';						
					}	
				seDataQueue[39+i]=Mdata[74];//���ʱ��
				seDataQueue[47+i]=Mdata[76];			
			}
			else		//����
			{
				strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"4EA46E90004174F64F9B969479BB");
				if(Mdata[74]=='A')		//��
					{
						seDataQueue[24+i]='4';
						seDataQueue[25+i]='E';
						seDataQueue[26+i]='A';
						seDataQueue[27+i]='4';						
					}
				else if(Mdata[74]=='B')	//��
					{
						seDataQueue[24+i]='5';
						seDataQueue[25+i]='1';
						seDataQueue[26+i]='4';
						seDataQueue[27+i]='9';						
					}
				else if(Mdata[74]=='Z')	//��
					{
						seDataQueue[24+i]='6';
						seDataQueue[25+i]='5';
						seDataQueue[26+i]='E';
						seDataQueue[27+i]='0';						
					}

				if(Mdata[75]=='C')	//A
					{
						seDataQueue[32+i]='0';
						seDataQueue[33+i]='0';
						seDataQueue[34+i]='4';
						seDataQueue[35+i]='1';						
					}
				else if(Mdata[75]=='D')	//B
					{
						seDataQueue[32+i]='0';
						seDataQueue[33+i]='0';
						seDataQueue[34+i]='4';
						seDataQueue[35+i]='2';						
					}
				else if(Mdata[75]=='Y')	//��
					{
						seDataQueue[32+i]='6';
						seDataQueue[33+i]='5';
						seDataQueue[34+i]='E';
						seDataQueue[35+i]='0';						
					}

				if(Mdata[76]=='E')	//����
					{
						seDataQueue[44+i]='9';
						seDataQueue[45+i]='6';
						seDataQueue[46+i]='9';
						seDataQueue[47+i]='4';
						seDataQueue[48+i]='4';
						seDataQueue[49+i]='F';
						seDataQueue[50+i]='9';
						seDataQueue[51+i]='B';						
					}
				else if(Mdata[76]=='F')	//����
					{
						seDataQueue[44+i]='6';
						seDataQueue[45+i]='D';
						seDataQueue[46+i]='6';
						seDataQueue[47+i]='E';
						seDataQueue[48+i]='5';
						seDataQueue[49+i]='1';
						seDataQueue[50+i]='4';
						seDataQueue[51+i]='5';						
					}
				else if(Mdata[76]=='X')	//ֱ��
					{
						seDataQueue[44+i]='7';
						seDataQueue[45+i]='6';
						seDataQueue[46+i]='F';
						seDataQueue[47+i]='4';
						seDataQueue[48+i]='4';
						seDataQueue[49+i]='F';
						seDataQueue[50+i]='9';
						seDataQueue[51+i]='B';						
					}
			}
			Nop();
			//52+i:0(55)0(59)1(63)-0(71)2(75)0(79)
	
			//80+i:F(83)0(87)0(91)0  end--(95)
			//52+i:F(55)0(59)0(63)0  end--(67)
			strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"0046003000300030");
			if(Mdata[77]=='E')
				seDataQueue[55+i]='5';
			else if(Mdata[77]=='F')
				seDataQueue[55+i]='6';
			seDataQueue[59+i]=Mdata[78];
			seDataQueue[63+i]=Mdata[79];
			seDataQueue[67+i]=Mdata[80];	
			if(normalFlag==1)					
				usartSendData(seDataQueue,68+i);//����68��
			else
			{
				if(Mdata[177]=='B')	//�ȳ�
					strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"7B495145");
				else if(Mdata[177]=='C')//�޳�
					strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"65E05145;");
				else if(Mdata[177]=='D')//�г�
					strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"67095145");
				else if(Mdata[177]=='H')//��A
					strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"7B490041");
				else if(Mdata[177]=='I')//��B
					strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"7B490042");
				else if(Mdata[177]=='J')//��ȫ
					strcatpgm2ram((char *)(seDataQueue),(const far rom char *)"7B495168");
				usartSendData(seDataQueue,76+i);//����76��				
			}
			Nop();		
//	    	OKFlag = 0;
//			ERRORFlag = 0;
//			TimeOut = 0;
   			usartSendData(s,1);
/*        	while(1)
        	{
	        	if(OKFlag)
	       		return 1;
				else if(ERRORFlag)
				return 0;
				else if(TimeOut >= 60)//LJ
				return 0;
				//���� ���ԼӸ� 60�볬ʱ ��ͣ ����
		   		ClrWdt(); 
			}
			Nop();*/
}
//**************************************************************************************
//�������ƣ�unsigned char phoneNumWoR(unsigned char Phone[])
//�����������ж��ֻ������Ƿ�Ϊ���֣�1������������ȷ  0���������ַ�-����
//�������ڣ�
//��    ע: 
//�޸����ڣ�
//�޸�˵����
//************************************************************************************** 
unsigned char phoneNumWoR(unsigned char Phone[])
{
	unsigned char i;
	for(i=0;i<11;i++)
	{
		if(Phone[i]<'0'||Phone[i]>'9')
		return 0;
	}
	if(Phone[0]!='1')  return 0;
	if(Phone[1]=='0'||Phone[1]=='1'||Phone[1]=='2'||Phone[1]=='7'||Phone[1]=='9')
		return 0;	
	return 1;
}
//**************************************************************************************
//�������ƣ�unsigned char lenConvert(unsigned char len[])
//�������������յ��Ķ�������֡��ASCII��16������ת������ͨ��10������
//�������ڣ�
//��    ע: 
//�޸����ڣ�
//�޸�˵����
//************************************************************************************** 
unsigned char lenConvert(unsigned char len[])
{
	unsigned char gsmLen1;
	if(len[0]>'9'&&len[1]>'9')
	{
		gsmLen1=(len[0]-'A'+10)*16+(len[1]-'A'+10);
	}
	else if(len[0]<'9'&&len[1]>'9')
	{
		gsmLen1=(len[0]-'0')*16+(len[1]-'A'+10);
	}
	else if(len[0]>'9'&&len[1]<'9')
	{
		gsmLen1=(len[0]-'A'+10)*16+(len[1]-'A'+10);
	}
	else if(len[0]<'9'&&len[1]<'9')
	{
		gsmLen1=(len[0]-'0')*16+(len[1]-'0');
	}
	return gsmLen1;	
}

//**************************************************************************************
//�������ƣ�void usartSendData(unsigned char  *pp,unsigned char m)
//����������
//�������ڣ�
//��    ע: 
//�޸����ڣ�
//�޸�˵����
//************************************************************************************** 
void usartSendData(unsigned char  *pp,unsigned char m)
{  unsigned char i;
   for(i=0;i<m;i++)
     {TXREG=*pp++;
      for(;!TXSTAbits.TRMT;)
        {;}
        Nop();
     }
   ClrWdt();
}
//**************************************************************************************
//�������ƣ�void My_Delay(unsigned char ConMs)100ms*ConMs
//����������
//�������ڣ�
//��    ע: 
//�޸����ڣ�
//�޸�˵����
//************************************************************************************** 
void My_Delay(unsigned char ConMs)
{
    unsigned char con = 0;
    do
    {
	    Delay10KTCYx(160);
	    ClrWdt();
	    con++;
       }
    while(con<ConMs);
}
//**************************************************************************************
//�������ƣ�void My_Delay1(unsigned char ConMs)10ms*ConMs
//����������
//�������ڣ�
//��    ע: 
//�޸����ڣ�
//�޸�˵����
//************************************************************************************** 
void My_Delay1(unsigned char ConMs)
{
    unsigned char con = 0;
    do
    {
	    Delay10KTCYx(16);
	    ClrWdt();
	    con++;
       }
    while(con<ConMs);
}

//**************************************************************************************
//�������ƣ�void gsmCheck(UINT8 *gsmbuf)
//����������
//�������ڣ�
//��    ע: 
//�޸����ڣ�
//�޸�˵����
//************************************************************************************** 
void gsmCheck(UINT8 *gsmbuf)
{

}
