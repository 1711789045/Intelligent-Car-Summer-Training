#ifndef __SERVO_H__
#define __SERVO_H__
#include "zf_common_headfile.h"

#define SERVO_PWM_CH1           (TIM2_PWM_CH1_A15)
#define SERVO_PWMMID            400                //�����ֵ
#define SERVO_LIMIT              50                 //������ת��
void servo_init(void);


#endif
