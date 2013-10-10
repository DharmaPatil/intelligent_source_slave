#include "main.h"

void SSP_Handler(void){
	PIR1bits.SSPIF = 0;
	if(SSPCON1bits.SSPOV)
		SSPCON1bits.SSPOV = 0;  //clear OV if overflowed

	SspQueue[SSPCON++] = SSPBUF;
	
	if(SSPCON < 124){ //小于124时候 不发送有用数据 发送调试数据
		SSPBUF = --SSPTEMP;  //加这个为 了跟主机通信调试 对数据位置
	}else if(SSPCON < 200){
		SSPBUF = S_To_M_buf[SSPCON - 124];  //这里接收主机数据时候同时发送从机数据
		if(SSPCON == 194){  //若上次收的是第193个数据  主机校验之前数据接收完成计算校验和
			S_To_M_buf[73] = SSP_CHECKSUM / 100 + 0X30;
			S_To_M_buf[74] = SSP_CHECKSUM % 100 / 10 + 0X30;
			S_To_M_buf[75] = SSP_CHECKSUM % 10 + 0X30; 	                    //计算主机数据校验和
		}
	}else{ //end of communication
		if(SspQueue[193] == S_To_M_buf[73] && SspQueue[194] == S_To_M_buf[74]
		                                                                   && SspQueue[195] == S_To_M_buf[75]) {
			SSP_Succeed = 1;                                              //从机收主机数据对
			TROUBLE = 0;
			Time0con = 0;
			userBuf[2] = 0;                                             //温湿度模块通讯正确计数位清零
			S_To_M_buf[49] = S_To_M_buf[50] = S_To_M_buf[51] = 'N';
			S_To_M_buf[68] = 'Y';                                      //通信成功清楚雷击状态
			if (SspQueue[120] == 'Y'){          //主1被修改
				memcpy((char *) &S_To_M_buf[23], (const char *) (&SspQueue[121]), 11); //复制手机号到存储区
				SaveParameters(0X00, (unsigned char *) (&S_To_M_buf[23]), 11);
			}else if(SspQueue[121] == '1')//?????
				memcpy((char *) &S_To_M_buf[23], (const char *) (&SspQueue[121]), 11); //复制手机号到存储区
			
			if (SspQueue[132] == 'Y'){                                      //主2被修改
				memcpy((char *) &S_To_M_buf[34], (const char *) (&SspQueue[133]), 11); //复制手机号到存储区
				SaveParameters(0X0B, (unsigned char *) (&S_To_M_buf[34]), 11);
			}else if(SspQueue[133] == '1')
				memcpy((char *) &S_To_M_buf[34], (const char *) (&SspQueue[133]), 11);

			if (SspQueue[157] == '1')
				memcpy((char *) &S_To_M_buf[56], (const char *) (&SspQueue[157]), 11);

			SystemParameters[19] = 0XFF;                                  //清故障标志
			SaveParameters(0XB3, (unsigned char *) (&SystemParameters[19]), 1); //存储一次状态  故障状态
			//收到主机定时给主手机1发送短信命令
			if (SspQueue[183] == 'Y')
				SM1 = 1;
			//收到主机定时给主手机2发送短信命令
			if (SspQueue[184] == 'Y')
				SM2 = 1;
		}else{
			SSP_Succeed = 0;
		}
		if (SspQueue[196] == S_To_M_buf[70] && SspQueue[197] == S_To_M_buf[71]
		                                                                   && SspQueue[198] == S_To_M_buf[72])
			S_To_M_buf[12] = 'Z';                   //主机收从机数据正确 短信命令已发出 清短信命令位//LJ debug
		else
			Nop()
			;
		Nop()
		;
		SSP_CHECKSUM = 0;
		SSPCON = 0;
		CloseSPI();
	}

	if (SspQueue[0] != 0x02)             							//如果握手不成功 则不累加
		SSPCON = 0;
	if (SSPCON < 194)                    								//边收边计算校验和
		SSP_CHECKSUM += SspQueue[SSPCON - 1];
}