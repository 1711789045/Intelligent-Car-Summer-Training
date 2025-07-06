#include "zf_common_headfile.h"

typedef struct {
    float last_error;      // ��һ�����
    float prev_error;      // ���ϴ�������΢�֣�
    float output;          // ��ǰ���ֵ
} PID_TypeDef;

//����ʽpid
float pid_increment(PID_TypeDef *pid, float target, float current, 
                    float limit, float kp, float ki, float kd) 
{
    float error = target - current;
    
    float p_term = kp * (error - pid->last_error);
    float i_term = ki * error;       
    float d_term = kd * (error - 2*pid->last_error + pid->prev_error);
    
    float increment = p_term + i_term + d_term;
    pid->output += increment;

    // ���������ʷ
    pid->prev_error = pid->last_error;
    pid->last_error = error;

    // ����޷�
    if(pid->output > limit) pid->output = limit;
    else if(pid->output < -limit) pid->output = -limit;
    
    return pid->output;
}
