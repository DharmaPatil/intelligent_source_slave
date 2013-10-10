#include "main.h"

void SSP_Handler(void){
	PIR1bits.SSPIF = 0;
	if(SSPCON1bits.SSPOV)
		SSPCON1bits.SSPOV = 0;  //clear OV if overflowed

	SspQueue[SSPCON++] = SSPBUF;
	
	if(SSPCON < 124){ //С��124ʱ�� �������������� ���͵�������
		SSPBUF = --SSPTEMP;  //�����Ϊ �˸�����ͨ�ŵ��� ������λ��
	}else if(SSPCON < 200){
		SSPBUF = S_To_M_buf[SSPCON - 124];  //���������������ʱ��ͬʱ���ʹӻ�����
		if(SSPCON == 194){  //���ϴ��յ��ǵ�193������  ����У��֮ǰ���ݽ�����ɼ���У���
			S_To_M_buf[73] = SSP_CHECKSUM / 100 + 0X30;
			S_To_M_buf[74] = SSP_CHECKSUM % 100 / 10 + 0X30;
			S_To_M_buf[75] = SSP_CHECKSUM % 10 + 0X30; 	                    //������������У���
		}
	}else{ //end of communication
		if(SspQueue[193] == S_To_M_buf[73] && SspQueue[194] == S_To_M_buf[74]
		                                                                   && SspQueue[195] == S_To_M_buf[75]) {
			SSP_Succeed = 1;                                              //�ӻ����������ݶ�
			TROUBLE = 0;
			Time0con = 0;
			userBuf[2] = 0;                                             //��ʪ��ģ��ͨѶ��ȷ����λ����
			S_To_M_buf[49] = S_To_M_buf[50] = S_To_M_buf[51] = 'N';
			S_To_M_buf[68] = 'Y';                                      //ͨ�ųɹ�����׻�״̬
			if (SspQueue[120] == 'Y'){          //��1���޸�
				memcpy((char *) &S_To_M_buf[23], (const char *) (&SspQueue[121]), 11); //�����ֻ��ŵ��洢��
				SaveParameters(0X00, (unsigned char *) (&S_To_M_buf[23]), 11);
			}else if(SspQueue[121] == '1')//?????
				memcpy((char *) &S_To_M_buf[23], (const char *) (&SspQueue[121]), 11); //�����ֻ��ŵ��洢��
			
			if (SspQueue[132] == 'Y'){                                      //��2���޸�
				memcpy((char *) &S_To_M_buf[34], (const char *) (&SspQueue[133]), 11); //�����ֻ��ŵ��洢��
				SaveParameters(0X0B, (unsigned char *) (&S_To_M_buf[34]), 11);
			}else if(SspQueue[133] == '1')
				memcpy((char *) &S_To_M_buf[34], (const char *) (&SspQueue[133]), 11);

			if (SspQueue[157] == '1')
				memcpy((char *) &S_To_M_buf[56], (const char *) (&SspQueue[157]), 11);

			SystemParameters[19] = 0XFF;                                  //����ϱ�־
			SaveParameters(0XB3, (unsigned char *) (&SystemParameters[19]), 1); //�洢һ��״̬  ����״̬
			//�յ�������ʱ�����ֻ�1���Ͷ�������
			if (SspQueue[183] == 'Y')
				SM1 = 1;
			//�յ�������ʱ�����ֻ�2���Ͷ�������
			if (SspQueue[184] == 'Y')
				SM2 = 1;
		}else{
			SSP_Succeed = 0;
		}
		if (SspQueue[196] == S_To_M_buf[70] && SspQueue[197] == S_To_M_buf[71]
		                                                                   && SspQueue[198] == S_To_M_buf[72])
			S_To_M_buf[12] = 'Z';                   //�����մӻ�������ȷ ���������ѷ��� ���������λ//LJ debug
		else
			Nop()
			;
		Nop()
		;
		SSP_CHECKSUM = 0;
		SSPCON = 0;
		CloseSPI();
	}

	if (SspQueue[0] != 0x02)             							//������ֲ��ɹ� ���ۼ�
		SSPCON = 0;
	if (SSPCON < 194)                    								//���ձ߼���У���
		SSP_CHECKSUM += SspQueue[SSPCON - 1];
}