#include "zf_common_headfile.h"
#include  "servo.h"


//ռ�ձ����ֵ��10000

void servo_init(void){
	// ��ʼ��pwmͨ��
	pwm_init(SERVO_PWM_CH1, 50, SERVO_PWMMID);

}

void servo_setangle(int16 angle){
	// ��ʼ��pwmͨ��
	angle = func_limit(angle,SERVO_LIMIT);
	pwm_set_duty(SERVO_PWM_CH1, SERVO_PWMMID+angle);

}

