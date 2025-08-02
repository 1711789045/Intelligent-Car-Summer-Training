#include "zf_common_headfile.h"
#include "auto_menu.h"
#include "math.h"
#include "imu.h"

/*逐飞互补滤波解算*/
uint8 gyro_ration = 4;     // 角速度置信度
uint8 acc_ration = 4;      // 加速度置信度
float filtering_angle = 0; // 解算出的roll角度
float angle_roll_temp;          // 角度计算中间变量

float angle_yaw_temp = 0.0f;
float yaw=0;

float call_cycle = 0.010f;

int16_t gx;
int16_t gy;
int16_t gz;
int16_t ax;
int16_t ay;
int16_t az;

void first_order_complementary_filtering(void)
{
    // 获取传感器数据
    imu963ra_get_acc();
    imu963ra_get_gyro();

    // 读取 IMU 数据、消除零漂
    gx = imu963ra_gyro_x + gx_error;
    gy = imu963ra_gyro_y + gy_error;
    gz = imu963ra_gyro_z + gz_error;
    ax = imu963ra_acc_x;
    ay = imu963ra_acc_y;
    az = imu963ra_acc_z;

    // 陀螺仪数据处理：去除小值噪声
    if (abs(gx) < 25)
        gx = 0;
    if (abs(gy) < 25)
        gy = 0;
    if (abs(gz) < 25)
        gz = 0;

    float gyro_temp;
    float acc_temp;
//    gyro_temp = gx * gyro_ration;
//    acc_temp = (ay - angle_roll_temp) * acc_ration;
//    angle_roll_temp += ((gyro_temp + acc_temp) * call_cycle);
//    filtering_angle = angle_roll_temp + machine_mid;
		
    gyro_temp = gx * gyro_ration;
    angle_roll_temp += ((gyro_temp ) * call_cycle);
    filtering_angle = angle_roll_temp + machine_mid;
	
	
//    gyro_temp = gz * gyro_ration;
//    acc_temp = (az - angle_yaw_temp) * acc_ration;
//    angle_yaw_temp += ((gyro_temp + acc_temp) * call_cycle);
//    yaw = angle_yaw_temp ;
}


