#include "zf_common_headfile.h"
#include "auto_menu.h"
#include "math.h"
#include "imu.h"

static float q0 = 1, q1 = 0, q2 = 0, q3 = 0;    // quaternion elements representing the estimated orientation
static float exInt = 0, eyInt = 0, ezInt = 0;    // scaled integral error
 
float yaw = 0;
float pitch = 0;
float roll = 0;
 
int16_t AX, AY, AZ, GX, GY, GZ;
 
typedef struct {
    float c1, c2, c3, a1, a2, a3, a;
} Degree;
 
Degree d;
 
typedef struct {
    float q0, q1, q2, q3;
    float exInt, eyInt, ezInt;
} Quater;
 
Quater q = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};


void ComputeEulerAngles(void) {
    
    // 计算姿态误差
	imu963ra_get_acc();
	imu963ra_get_gyro();
	d.c1 = imu963ra_gyro_transition(imu963ra_gyro_x);
	d.c2 = imu963ra_gyro_transition(imu963ra_gyro_y);
	d.c3 = imu963ra_gyro_transition(imu963ra_gyro_z);
	
	d.c1 /= RtA;
    d.c2 /= RtA;
    d.c3 /= RtA;
	
	d.a1 = imu963ra_acc_transition(imu963ra_acc_x);
	d.a2 = imu963ra_acc_transition(imu963ra_acc_y);
	d.a3 = imu963ra_gyro_transition(imu963ra_acc_z);

	
    float gx = d.c1, gy = d.c2, gz = d.c3;
    float gravity_x = 2.0f * (q.q1 * q.q3 - q.q0 * q.q2);
    float gravity_y = 2.0f * (q.q0 * q.q1 + q.q2 * q.q3);
    float gravity_z = q.q0*q.q0 - q.q1 * q.q1 - q.q2 * q.q2 + q.q3*q.q3;
    
    float error_x = d.a2 * gravity_z - d.a3 * gravity_y;
    float error_y = d.a3 * gravity_x - d.a1 * gravity_z;
    float error_z = d.a1 * gravity_y - d.a2 * gravity_x;
    
    // 更新四元数
    float Kp = 0.5f;
    
    // 误差积分
    q.exInt += Ki * error_x ;
    q.eyInt += Ki * error_y ;
    q.ezInt += Ki * error_z ;
    
    // 修正角速度
    gx += Kp * error_x + q.exInt;
    gy += Kp * error_y + q.eyInt;
    gz += Kp * error_z + q.ezInt;
    
    // 四元数微分方程
    float q0_dot = (-q.q1 * gx - q.q2 * gy - q.q3 * gz) * DT; 
    float q1_dot = (q.q0 * gx - q.q3 * gy + q.q2 * gz) * DT;
    float q2_dot = (q.q3 * gx + q.q0 * gy - q.q1 * gz) * DT;
    float q3_dot = (-q.q2 * gx + q.q1 * gy + q.q0 * gz) * DT;
    
    // 更新四元数
    q.q0 += q0_dot;
    q.q1 += q1_dot;
    q.q2 += q2_dot;
    q.q3 += q3_dot;
    
    // 归一化
    float norm = sqrtf(q.q0 * q.q0 + q.q1 * q.q1 + q.q2 * q.q2 + q.q3 * q.q3);
    if (norm > 1e-6f) {
        float inv_norm = 1.0f / norm;
        q.q0 *= inv_norm;
        q.q1 *= inv_norm;
        q.q2 *= inv_norm;
        q.q3 *= inv_norm;
    }
    
    // 计算欧拉角
    float q0q0 = q.q0 * q.q0;
    float q1q1 = q.q1 * q.q1;
    float q2q2 = q.q2 * q.q2;
    float q3q3 = q.q3 * q.q3;
    
    roll = atan2f(2.0f * (q.q2 * q.q3 + q.q0 * q.q1), q0q0 - q1q1 - q2q2 + q3q3);
    pitch = asinf(-2.0f * (q.q1 * q.q3 - q.q0 * q.q2));
    yaw = atan2f(2.0f * (q.q1 * q.q2 + q.q0 * q.q3), q0q0 + q1q1 - q2q2 - q3q3);
    
   // 转换为度
    roll *= RtA;
    pitch *= RtA;
    yaw *= RtA;
}

