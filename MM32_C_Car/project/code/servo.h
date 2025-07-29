#ifndef __SERVO_H__
#define __SERVO_H__
#include "zf_common_headfile.h"

#define SERVO_MOTOR_PWM             (TIM2_PWM_CH1_A15)                          // 定义主板上舵机对应引脚
#define SERVO_MOTOR_FREQ            (50 )                                       // 定义主板上舵机频率  请务必注意范围 50-300

#define SERVO_MOTOR_LIMIT               (13.5)
#define SERVO_MOTOR_MID               (62)
#define SERVO_MOTOR_L_MAX           (48.5)                                       // 定义主板上舵机活动范围 角度
#define SERVO_MOTOR_R_MAX           (75.5)                                       // 定义主板上舵机活动范围 角度
#define SERVO_MOTOR_DUTY(x)         ((float)PWM_DUTY_MAX / (1000.0 / (float)SERVO_MOTOR_FREQ) * (0.5 + (float)(x) / 90.0))

void servo_init(void);
void servo_setangle(float angle);
void servo_set_pid(float kp,float ki,float kd1,float kd2);
void servo_control(uint8 mid_line);
void servo_process(void);


extern uint8 servo_f;

#endif
