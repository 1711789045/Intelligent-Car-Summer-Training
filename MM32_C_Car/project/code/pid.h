#ifndef __PID_H__
#define __PID_H__

typedef struct {
    float last_error;      // ��һ�����
    float prev_error;      // ���ϴ�������΢�֣�
    float output;          // ��ǰ���ֵ
} PID_TypeDef;

float pid_increment(PID_TypeDef *pid, float target, float current, 
                    float limit, float kp, float ki, float kd);
#endif
