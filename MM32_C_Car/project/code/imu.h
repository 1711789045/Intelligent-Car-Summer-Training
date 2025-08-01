#ifndef __IMU_H__
#define __IMU_H__

#include "zf_common_typedef.h"
#define gx_error -5
#define gy_error 8
#define gz_error 6
#define machine_mid 0//»úÐµÖÐÖµ
extern float roll,roll_offset,yaw;
extern float filtering_angle;
extern float yaw;
extern int16_t gx   ;
extern int16_t gy   ;
extern int16_t gz   ;
extern int16_t ax   ;
extern int16_t ay   ;
extern int16_t az   ;
void first_order_complementary_filtering(void);

#endif
