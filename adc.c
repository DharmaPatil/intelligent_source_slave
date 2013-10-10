#include "main.h"

unsigned char buf_ad[9];

static unsigned int FilterRead_Ad(unsigned char chooseAN)
{
	unsigned char i;
	unsigned int max=0;
	unsigned int min=0xffff;
	unsigned int sum=0;
	unsigned int result_temp;

	ADCON0=chooseAN;
	for(i=4;i>0;i--){
		ADCON0bits.GO=1;
		while(ADCON0bits.GO)
			;
		Nop();
		result_temp = (((unsigned int)ADRESH << 8) + (unsigned int)ADRESL);
		ClrWdt();
		sum=sum+result_temp;
		if(max < result_temp){
			max = result_temp;
		}else if(min > result_temp){
			min = result_temp;
		}
	}

	return (sum-max-min)/2;
}

static unsigned int commit_convert(unsigned char chooseAN)
{
	unsigned int ad_result = 0;
	int reval = 0;
	int buffer_bit;

	ad_result = FilterRead_Ad(chooseAN);

	switch (chooseAN)
	{
	case Converter_SUN:
		buffer_bit = SUN;
		if (0x0370 >= ad_result){
			return 1;
		}
		//判断有无光能
		/*
		 if((ad_result/4.092)>150)   //大于150V有光能
		 HadSUN = 1;
		 else
		 HadSUN = 0;
		 */
		break;
	case Converter_CHV:
		buffer_bit = CHV;
		if (0x0370 >= ad_result){
			return 2;
		}
		break;
	case Converter_CHI:
		buffer_bit = CHI;
		if (0x0370 >= ad_result){
			return 3;
		}
		break;
	case Converter_AC:
		buffer_bit = AC;
		if (0x07 <= ad_result){
			return 4;
		}
		//判断有无交流
		if ((ad_result / 4.092) > 150)   //大于150V有交流
			HadAC = 1;
		else
			HadAC = 0;
		break;
	case Converter_T:
		ad_result = (ad_result * 2.443 - 100) / 10 - 40;//摄氏度
		buf_ad[T] = (unsigned char) (ad_result);
		return 0;
	}

	buf_ad[buffer_bit] = (unsigned char) (ad_result >> 8);
	buf_ad[buffer_bit+1] = (unsigned char) (ad_result);

	return 0;
}

int ad(void) {
	while(0 != commit_convert(Converter_SUN))
			Nop();
	commit_convert(Converter_CHV);
	commit_convert(Converter_CHI);
	commit_convert(Converter_AC);
	commit_convert(Converter_T);
	return 0;
}
