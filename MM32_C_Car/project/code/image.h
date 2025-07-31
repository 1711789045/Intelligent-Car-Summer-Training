#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "zf_common_headfile.h"


#define IMAGE_W            MT9V03X_W
#define IMAGE_H            MT9V03X_H

#define REFERENCE_ROW       5          //参考点统计行数
#define REFERENCE_COL       80         //参考点统计列数
#define WHITEMAXMUL        13         //白点最大值基于参考点的倍数 10为不放大
#define WHITEMINMUL         7          //白点最小值基于参考点的倍数 10为不放大
#define BLACKPOINT          50         //黑点值

#define CONTRASTOFFSET      3          //搜线对比度偏移
#define STOPROW            0           //搜线停止行
#define SEARCHRANGE        10          //搜线半径
#define CIRCLE_1_TIME      15        //环岛状态一计时时间，单位10ms
#define CIRCLE_2_TIME      50         //环岛状态二计时时间，单位10ms
#define CIRCLE_4_TIME      25           //环岛状态四计时时间，单位10ms
#define CIRCLE_5_TIME      25           //环岛状态五计时时间，单位10ms
#define STOP_ANALYSE_LINE  (IMAGE_H-40)          //斑马线检测行
#define STOP_THRESHOLD    30			 //斑马线跳变点数量阈值
#define STRETCH_NUM       80          //补线延伸长度

extern uint8 reference_point;
extern uint8 white_max_point;          //动态白点最大值
extern uint8 white_min_point;          //动态白点最小值
extern uint8 remote_distance[IMAGE_W]; //最长白列
extern uint8 reference_col;
extern uint8 reference_contrast_ratio; //参考对比度
extern uint16 reference_line[IMAGE_H];      // 存储参考列
extern uint16 left_edge_line[IMAGE_H];      // 存储左边线
extern uint16 right_edge_line[IMAGE_H];      // 存储右边线
extern uint8 final_mid_line;
extern uint8 prospect;                     //前瞻值
extern uint8 single_edge_err[IMAGE_H];
extern uint8 circle_flag;                  //环岛标志位
extern uint16 circle_time;                  
extern uint8 mid_mode;                    //循线模式
extern uint8 if_circle;                     //1为启用圆环，0为关闭圆环
extern uint8 mid_weight_1[IMAGE_H];
extern uint8 mid_weight_2[IMAGE_H];
extern uint8 mid_weight_3[IMAGE_H];
extern uint8 mid_weight_4[IMAGE_H];
extern uint8 mid_weight_5[IMAGE_H];


extern uint8 mid_weight[IMAGE_H];



void get_image(void);
void get_reference_point(const uint8 image[][IMAGE_W]);
void search_reference_col(const uint8 image[][IMAGE_W]);
void search_line(const uint8 image[][IMAGE_W]);
void image_display_edge_line(const uint8 image[][IMAGE_W],uint16 width,uint16 height);
void image_process(uint16 display_width,uint16 display_height,uint8 mode);

#endif
