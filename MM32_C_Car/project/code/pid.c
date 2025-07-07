#include "zf_common_headfile.h"

typedef struct {
    float last_error;      // ��һ�����
    float prev_error;      // ���ϴ�������΢�֣�
    float output;          // ��ǰ���ֵ
} PID_INCREMENT_TypeDef;

typedef struct {
    float last_error;      // ��һ�����
    float error_sum;      // ������
    float output;          // ��ǰ���ֵ
} PID_POSITIONAL_TypeDef;

//����ʽpid
float pid_increment(PID_INCREMENT_TypeDef *pid, float target, float current, 
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


float pid_positional(PID_POSITIONAL_TypeDef *pid, float target, float current, 
                     float limit, float kp, float ki, float kd) {
    // ���㵱ǰ���
    float error = target - current;
	pid->error_sum += error;
    
    // PID������Ӽ������
    float p_term = kp * error;
    float i_term = ki * pid->error_sum;
    float d_term = kd * (error-pid->last_error);
    pid->output = p_term + i_term + d_term;
    
    // ���ֿ����ʹ���
    if(pid->output > limit) {
        pid->output = limit;
    }
    else if(pid->output < -limit) {
        pid->output = -limit;
    }
    
    // ���������ʷ
    pid->last_error = error;
    
    return pid->output;
}
