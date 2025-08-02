#include "zf_common_headfile.h"
#include  "motor.h"
#include "pid.h"
#include "auto_menu.h"
#include "image.h"
#include "control.h"
//占空比最大值是10000

uint8 stop_flag = 0;
uint8 motor_f = 0;
int16 encoder_data_l = 0;
int16 encoder_data_r = 0;

static PID_INCREMENT_TypeDef pid_left = {0};
static PID_INCREMENT_TypeDef pid_right = {0};

float motor_pid_kp = 8.0,motor_pid_ki = 2.0,motor_pid_kd = 4.0;
uint8 differential_mode = 0;

void motor_init(void){
	// 初始化pwm通道2和4
	pwm_init(MOTOR_L_PWM_CH4, 17000, 0);
	pwm_init(MOTOR_R_PWM_CH2, 17000, 0);
	
	// 初始化 LED2 输出 默认高电平 推挽输出模式
	gpio_init(MOTOR_L_DIR, GPO, GPIO_HIGH, GPO_PUSH_PULL);
	gpio_init(MOTOR_R_DIR, GPO, GPIO_HIGH, GPO_PUSH_PULL);
}

void motor_set_pid(float kp,float ki,float kd){
	motor_pid_kp = kp;
	motor_pid_ki = ki;
	motor_pid_kd = kd;
}

void motor_setpwm(uint8 motor,int16 speed){

	if(!motor){
		if(speed >= 0){
			if(speed>SPEED_LIMIT)
				speed = SPEED_LIMIT;
			pwm_set_duty(MOTOR_L_PWM_CH4, speed);
			gpio_set_level(MOTOR_L_DIR, GPIO_HIGH);
		}
		else{
			speed = -speed;
			if(speed>=SPEED_LIMIT)
				speed = SPEED_LIMIT;
			pwm_set_duty(MOTOR_L_PWM_CH4, speed);
			gpio_set_level(MOTOR_L_DIR, GPIO_LOW);
		}
	}
	else{
		if(speed >= 0){
			if(speed>SPEED_LIMIT)
				speed = SPEED_LIMIT;
			pwm_set_duty(MOTOR_R_PWM_CH2, speed);
			gpio_set_level(MOTOR_R_DIR, GPIO_HIGH);
		}
		else{
			speed = -speed;
			if(speed>=SPEED_LIMIT)
				speed = SPEED_LIMIT;
			pwm_set_duty(MOTOR_R_PWM_CH2, speed);
			gpio_set_level(MOTOR_R_DIR, GPIO_LOW);
		}
	}
}

void motor_setspeed(int16 target, float current_l, float current_r,uint8 differential_mode) {
	int16 speed_l = 0,speed_r = 0;
	if(!differential_mode){
		speed_l = pid_increment(&pid_left, target, current_l, 
                                 SPEED_LIMIT, motor_pid_kp, motor_pid_ki, motor_pid_kd);
    
		speed_r = pid_increment(&pid_right, target, current_r, 
                                 SPEED_LIMIT, motor_pid_kp, motor_pid_ki, motor_pid_kd);
	}
	if(differential_mode){
		float err = final_mid_line - IMAGE_W/2;
//		float k = func_limit_ab((func_abs(err)-20)/10.0,0,1.0);
		float k = func_limit_ab((func_abs(err)-10),0,100);

		if(final_mid_line - IMAGE_W/2 >10){
			speed_r = pid_increment(&pid_right,target+ (int16)(dif_speed_reduce*k), current_r, 
                                 SPEED_LIMIT, motor_pid_kp, motor_pid_ki, motor_pid_kd);
			speed_l = pid_increment(&pid_left, target+  (int16)(dif_speed_plus*k), current_l, 
                                 SPEED_LIMIT, motor_pid_kp, motor_pid_ki, motor_pid_kd);
		}
		else if(final_mid_line - IMAGE_W/2 <-10){
			speed_r = pid_increment(&pid_right,target+ (int16)(dif_speed_plus*k), current_r, 
                                 SPEED_LIMIT, motor_pid_kp, motor_pid_ki, motor_pid_kd);
		
			speed_l = pid_increment(&pid_left, target+ (int16)(dif_speed_reduce*k), current_l, 
                                 SPEED_LIMIT, motor_pid_kp, motor_pid_ki, motor_pid_kd);
		}
		else{
			speed_r = pid_increment(&pid_right, target, current_r, 
                                 SPEED_LIMIT, motor_pid_kp, motor_pid_ki, motor_pid_kd);
			speed_l = pid_increment(&pid_left, target, current_l, 
                                 SPEED_LIMIT, motor_pid_kp, motor_pid_ki, motor_pid_kd);
		}
	}
//	ips200_show_int(0,208,speed_l,4);
//	ips200_show_int(0,224,speed_r,4);
	
    motor_setpwm(MOTOR_L, speed_l);
    motor_setpwm(MOTOR_R, speed_r);
}

void motor_lose_line_protect(void){
	if(prospect<5 && start_time > 30){
		stop_flag = 1;
		servo_flag = 0;
		motor_flag = 0;
	}
}

void motor_process(void){
	if(motor_f){
			motor_lose_line_protect();
			
			if(stop_flag)
				motor_setspeed(0,encoder_data_l,encoder_data_r,0);
			if(motor_flag && !stop_flag){
				motor_setspeed(speed,encoder_data_l,encoder_data_r,differential_mode);
				
			}
			motor_f = 0;
		}
}

