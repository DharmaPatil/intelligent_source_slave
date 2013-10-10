#ifndef ADC_H
#define ADC_H

#define   Converter_SUN           0b00000001   //AN0
#define   Converter_CHV           0b00000101   //AN1
#define   Converter_CHI           0b00001001   //AN2
#define   Converter_T             0b00010001   //AN4
#define   Converter_AC            0b00010101   //AN5

#define   SUN           0
#define   CHV           2
#define   CHI           4
#define   AC            6
#define   T             8


int ad(void);

#endif /* ADC_H */
