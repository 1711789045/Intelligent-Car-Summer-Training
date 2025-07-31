#ifndef __IMU_H__
#define __IMU_H__

#define RtA (57.2957795f)  // 弧度转角度，180/π ≈ 57.2957795
#define Ki  0.005f         // 积分系数
#define DT  0.010f          // 计算周期的一半，单位s
void ComputeEulerAngles(void) ;
extern  float pitch ,roll,yaw;

#endif
