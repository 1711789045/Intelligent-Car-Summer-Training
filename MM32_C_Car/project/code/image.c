#include "zf_common_headfile.h"
#include "image.h"

uint8 reference_point = 0;
uint8 white_max_point = 0;             //��̬�׵����ֵ
uint8 white_min_point = 0;             //��̬�׵���Сֵ
uint8 remote_distance[IMAGE_W] = {0};   //�����
uint8 reference_col = 0;
uint8 reference_contrast_ratio = 0.1*200; //�ο��Աȶ�
uint16 reference_line[IMAGE_H] = {0};      // �洢�ο���
uint16 left_edge_line[IMAGE_H] = {0};      // �洢�����
uint16 right_edge_line[IMAGE_H] = {0};      // �洢�ұ���
uint8 user_image[IMAGE_H][IMAGE_W];  //�洢ͼ��

uint8 mid_line[IMAGE_H] = {0};         //��������λ��
uint8 mid_weight[IMAGE_H] = {           //��������Ȩ��
	1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,
	1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,
	1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,
	1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,
	1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,
	6 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,
	7 ,8 ,9 ,10,11,12,13,14,15,16,
	17,18,19,20,20,20,20,19,18,17,
	16,15,14,13,12,11,10,9 ,8 ,7 ,
	6 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,6 ,
	1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,
	1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 
};
uint8 final_mid_line = 0;                 //��Ȩ����ֵ
uint8 last_final_mid_line = 0;            //�ϴμ�Ȩ����ֵ
uint8 prospect = 0;                     //ǰհֵ
uint8 cross_flag = 0;                    //ʮ�ֱ�־λ

void get_image(void){
	memcpy(user_image,mt9v03x_image,IMAGE_H*IMAGE_W);
}




void get_reference_point(const uint8 image[][IMAGE_W]){
	uint16 temp = 0;                 //���ڱ���ͳ�Ƶ�������
	uint32 temp1 = 0;               //���ڱ���ͳ�Ƶ�����ֵ
	uint16 temp_j1 = (IMAGE_W-REFERENCE_COL)/2;    
	uint16 temp_j2 = (IMAGE_W+REFERENCE_COL)/2;	
	
	temp = REFERENCE_ROW * REFERENCE_COL;
	for(int i = IMAGE_H-REFERENCE_ROW;i <= IMAGE_H-1;i++){
		for(int j = temp_j1;j <= temp_j2;j++){
			temp1 += image[i][j];				//ͳ�Ƶ����
		}
	}
	reference_point = (uint8)func_limit_ab((temp1/temp),BLACKPOINT,255);
	white_max_point = (uint8)func_limit_ab((int32)reference_point * WHITEMAXMUL/10,BLACKPOINT,255);
	white_min_point = (uint8)func_limit_ab((int32)reference_point * WHITEMINMUL/10,BLACKPOINT,255);
	
	ips200_show_int(96,240,reference_point,4);
	ips200_show_int(96,256,white_max_point,4);
	ips200_show_int(96,272,white_min_point,4);
}

void search_reference_col(const uint8 image[][IMAGE_W]){
	int col,row;
	int16 temp1 = 0,temp2 = 0,temp3 = 0;           //��ǰ�� �Աȵ� �Աȶ�
	for(col = 0;col < IMAGE_W;col++){
		remote_distance[col] = IMAGE_H-1;
	}
	for(col = 0;col < IMAGE_W;col+=CONTRASTOFFSET){                  //���У���������һ��
		for	(row = IMAGE_H-1;row >= STOPROW+CONTRASTOFFSET;row-=CONTRASTOFFSET){    //���У��������ж�һ��
			temp1 = image[row][col];
			temp2 = image[row-CONTRASTOFFSET][col];
			
			if(row == 5){   //����Աȶ�
				remote_distance[col] = (uint8)row;
				break;
			}
			
			if(temp2 > white_max_point){           //�Աȵ��ǰ׵�
				continue;
			}
			if(temp1 < white_min_point){           //��ǰ���Ǻڵ�
				remote_distance[col] = (uint8)row;
				break;
 			}
			
			temp3 = (temp1 - temp2)*200/(temp1 + temp2);
			
			if(temp3 >reference_contrast_ratio ){   //����Աȶ�
				remote_distance[col] = (uint8)row;
				break;
			}
		}
	}
	
	for(col = 0;col < IMAGE_W;col+=CONTRASTOFFSET){    //Ѱ�������
		if(remote_distance[reference_col] > remote_distance[col])
			reference_col = col;
	}
	for(row = 0;row < IMAGE_H;row++){
		reference_line[row] =reference_col;
	}
}


void search_line(const uint8 image[][IMAGE_W]){
    uint8 row_max = IMAGE_H - 1;                        // �����ֵ
    uint8 row_min = STOPROW;                                    // ����Сֵ
    uint8 col_max = IMAGE_W - CONTRASTOFFSET-1;           // �����ֵ
    uint8 col_min = CONTRASTOFFSET;                    // ����Сֵ
    int16 leftstartcol  = reference_col;                // ��������ʼ��
    int16 rightstartcol = reference_col;                // ��������ʼ��
    int16 leftendcol    = col_min;                           // ��������ֹ��
    int16 rightendcol   = col_max;                   // ��������ֹ��
    uint8 search_time   = 0;                                    // �������ߴ���
    uint8 temp1 = 0, temp2 = 0;                                 // ��ʱ����  ���ڴ洢ͼ���� 
    int  temp3 = 0;                                             // ��ʱ����  ���ڴ洢�Աȶ�
    int  leftstop = 0, rightstop = 0, stoppoint = 0;            // ������������

    int col, row;

    for(row = row_max; row >= row_min; row --){                //���߹��㵽����
		left_edge_line[row]  = col_min - CONTRASTOFFSET;
        right_edge_line[row] = col_max + CONTRASTOFFSET;
    }
	
	for(row = row_max;row >= row_min;row--){                   
		if(!leftstop){
			search_time = 2;
			do{
				if(search_time == 1){                         //��һ��û�ѵ�
					leftstartcol = reference_col;
					leftendcol = col_min;
				}
				
				search_time--;
				
				for(col = leftstartcol; col >= leftendcol ;col--){
					if(col - CONTRASTOFFSET <0)
						continue;
					temp1 = image[row][col];
					temp2 = image[row][col - CONTRASTOFFSET];
				
					if(temp1 < white_min_point && col == leftstartcol && leftstartcol == reference_col){
						//�жϲο����Ƿ�Ϊ�ڵ㣬���������в������������
						leftstop = 1;
						for(stoppoint = row;stoppoint >= 0;stoppoint--){
							left_edge_line[stoppoint ] = col_min;		//�����������ϵı��߹���
						}
						search_time = 0;
						break;
					}
					
					if(temp1 < white_min_point){           //��ǰ��Ϊ�ڵ�
						left_edge_line[row] = col;
						break;
					}
					if(temp2 > white_max_point){           //�ο���Ϊ�׵�
						continue;
					}
					temp3 = (temp1 - temp2)*200/(temp1 + temp2); //����Աȶ�
					
					if(temp3 >reference_contrast_ratio || col == col_min){   
						left_edge_line[row] = func_limit_ab(col - CONTRASTOFFSET, 0, IMAGE_W-1);		//������߽�
						//��һ���������߰뾶������
						leftstartcol = (uint8)func_limit_ab(col+SEARCHRANGE,col,col_max);  
						leftendcol = (uint8)func_limit_ab(col-SEARCHRANGE,col_min,col);
						search_time = 0;
						break;
					}
				}
			}while(search_time);
		}
		
		
		if(!rightstop){
			search_time = 2;
			do{
				if(search_time == 1){                         //��һ��û�ѵ�
					rightstartcol = reference_col;
					rightendcol = col_max;
				}
				
				search_time--;
				
				for(col = rightstartcol; col <= rightendcol ;col++){
					if(col + CONTRASTOFFSET >= IMAGE_W)
						continue;
					temp1 = image[row][col];
					temp2 = image[row][col + CONTRASTOFFSET];
				
					if(temp1 < white_min_point && col == rightstartcol && rightstartcol == reference_col){
						//�жϲο����Ƿ�Ϊ�ڵ㣬���������в��������ұ���
						rightstop = 1;
						for(stoppoint = row;stoppoint >= 0;stoppoint--){
							right_edge_line[stoppoint ] = col_max;		//�����������ϵı��߹���
						}
						search_time = 0;
						break;
					}
					
					if(temp1 < white_min_point){           //��ǰ��Ϊ�ڵ�
						right_edge_line[row] = col;
						break;
					}
					if(temp2 > white_max_point){           //�ο���Ϊ�׵�
						continue;
					}
					temp3 = (temp1 - temp2)*200/(temp1 + temp2); //����Աȶ�
					
					if(temp3 >reference_contrast_ratio || col == col_max){   
						right_edge_line[row] = func_limit_ab(col + CONTRASTOFFSET, 0, IMAGE_W-1);		//�����ұ߽�
						//��һ���������߰뾶������
						rightstartcol = (uint8)func_limit_ab(col-SEARCHRANGE,col_min,col);  
						rightendcol = (uint8)func_limit_ab(col+SEARCHRANGE,col,col_max);
						search_time = 0;
						break;
					}
				}
			}while(search_time);
		}
	}
}

uint8 image_find_jump_point(uint16 *edge_line,uint8 down_num,uint8 up_num,uint8 jump_num,uint8 model){
	//down_num ���¶���ʼ�� up_num ���϶���ֹ��,modelΪ1ʱ���µ��� Ϊ0ʱ���ϵ���
	uint8 temp_jump_point = 0;
	uint8 temp_data;
	
	if(model){
		temp_jump_point = down_num;
		for(int i = 0;i <down_num-up_num;i++){
			temp_data = func_abs(edge_line[down_num-i]-edge_line[down_num-i-1]);
			if(temp_data>jump_num){
				temp_jump_point = (uint8)(down_num-i);
				break;
			}
		}
	}
	else{
		temp_jump_point = up_num;
		for(int i = 0;i <down_num-up_num;i++){
			temp_data = func_abs(edge_line[up_num+i]-edge_line[up_num+i+1]);
			if(temp_data>jump_num){
				temp_jump_point = (uint8)(up_num+i);
				break;
			}
		}
	}
	return temp_jump_point;
}

void image_connect_point(uint16 *array_value, uint8 num0, uint8 num1)
{
    float point_1 = (float)array_value[num0];
    float point_2 = (float)array_value[num1];
    float temp_slope = (point_2 - point_1) / (num0 - num1);

    for (int i = 0; i < (num0 - num1); i++)
    {
        array_value[num0 - i] = (int8)(temp_slope * i) + array_value[num0];
    }
}

void image_cross_analysis(void){
	uint32 track_width = 0;
	uint8 start_point,end_point;
	for(int i = (IMAGE_H * 2 / 3);i >  (IMAGE_H / 3);i--){
		track_width += (right_edge_line[i] - left_edge_line[i]);
	}
	
	if(track_width > (IMAGE_W * (IMAGE_H * 4 / 15))){
		cross_flag = 1;
	}
	
	if(cross_flag){
		start_point = image_find_jump_point(left_edge_line,IMAGE_H - 5,5,10,1)+2;
		end_point = image_find_jump_point(left_edge_line,start_point-10,5,10,1)-2;
		image_connect_point(left_edge_line,start_point,end_point);
		
		ips200_show_int(96,224,start_point,4);
		ips200_show_int(128,224,end_point,4);
		
		start_point = image_find_jump_point(right_edge_line,IMAGE_H - 5,5,10,1)+2;
		end_point = image_find_jump_point(right_edge_line,start_point-10,5,10,1)-2;
		image_connect_point(right_edge_line,start_point,end_point);
		
		ips200_show_int(96,240,start_point,4);
		ips200_show_int(128,240,end_point,4);

		if(track_width < (IMAGE_W * (IMAGE_H * 1 / 5))){
			cross_flag = 0;
		}
	}
}

void image_calculate_mid(void){
	uint32 mid_weight_sum = 0;        //��Ȩ�����ۼ�ֵ
	uint32 weight_sum = 0;            //Ȩ���ۼ�ֵ
	uint8 temp = 0;                   //��ʱ�洢����
	for(int i = 0;i<IMAGE_H;i++){
		mid_line[i] = (left_edge_line[i] + right_edge_line[i])/2;
		weight_sum += mid_weight[i];
		mid_weight_sum += mid_line[i]*mid_weight[i];
	}
	temp = (uint8)(mid_weight_sum/weight_sum);
	if(!last_final_mid_line)
		final_mid_line = temp;
	else
		final_mid_line = temp*0.8+last_final_mid_line*0.2;  //�����˲�

	last_final_mid_line = final_mid_line;
	
}

void image_calculate_prospect(const uint8 image[][IMAGE_W]){
	int col = IMAGE_W/2;
	int16 temp1 = 0,temp2 = 0,temp3 = 0;
	for	(int row = IMAGE_H-1;row > CONTRASTOFFSET;row--){    //���У��������ж�һ��
		temp1 = image[row][col];
		temp2 = image[row-CONTRASTOFFSET][col];
		
		if(row == 5){   //����Աȶ�
			prospect = IMAGE_H-(uint8)row;
			break;
		}
		
		if(temp1 < white_min_point){           //��ǰ���Ǻڵ�
			prospect = IMAGE_H-(uint8)row;
			break;
		}
		
		if(temp2 > white_max_point){           //�Աȵ��ǰ׵�
			continue;
		}
		
		
		temp3 = (temp1 - temp2)*200/(temp1 + temp2);
		
		if(temp3 >reference_contrast_ratio ){   //����Աȶ�
			prospect = IMAGE_H-(uint8)row;
			break;
		}
	}
//	ips200_show_int(96,224,prospect,4);

}


void image_display_edge_line(const uint8 image[][IMAGE_W],uint16 display_width,uint16 display_height){
	ips200_displayimage03x((const uint8 *)image, display_width, display_height);
	
	for(uint16 i = 0;i<IMAGE_H;i++){
		ips200_draw_point(left_edge_line[i],i,RGB565_RED);
		ips200_draw_point(right_edge_line[i],i,RGB565_BLUE);
		ips200_draw_point(reference_line[i],i,RGB565_YELLOW);
		ips200_draw_point(mid_line[i],i,RGB565_GREEN);
		
//		ips200_show_int(96,208,final_mid_line,4);
	}
}

void image_core(uint16 display_width,uint16 display_height,uint8 mode){
	get_image();
	reference_point = 0; white_max_point = 0;white_min_point = 0;reference_col = 0;
	
	get_reference_point(user_image);
	search_reference_col(user_image);
	search_line(user_image);
	
//	image_cross_analysis();
	image_calculate_mid();
	image_calculate_prospect(user_image);
	
	if(mode)
		image_display_edge_line(user_image,display_width,display_height);
}
