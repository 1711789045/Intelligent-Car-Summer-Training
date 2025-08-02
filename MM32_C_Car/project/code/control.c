#include "zf_common_headfile.h"
#include "control.h"
#include "motor.h"
#include "servo.h"
#include "auto_menu.h"
#include "imu.h"
#include "beep.h"

int speed = 0;
uint8 ramp_flag = 0;

void ramp_analysis(void){
	if(filtering_angle < -600){
		ramp_flag = 1;
		beep_flag = 1;
	}
	if(filtering_angle>-400){
		ramp_flag = 0;
	}
}

void direction_control(void){
	speed = basic_speed;
	
//	ramp_analysis();
//	if(ramp_flag)
//		speed = 150;
	
	servo_process();
	motor_process();
}


