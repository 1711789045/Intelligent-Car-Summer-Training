/*********************************************************************************************************************
* MM32F327X-G8P Opensourec Library 即（MM32F327X-G8P 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
* 
* 本文件是 MM32F327X-G8P 开源库的一部分
* 
* MM32F327X-G8P 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
* 
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
* 
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
* 
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
* 
* 文件名称          zf_driver_adc
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          IAR 8.32.4 or MDK 5.37
* 适用平台          MM32F327X_G8P
* 店铺链接          https://seekfree.taobao.com/
* 
* 修改记录
* 日期              作者                备注
* 2022-08-10        Teternal            first version
********************************************************************************************************************/

#ifndef _zf_driver_adc_h_
#define _zf_driver_adc_h_

#include "zf_common_typedef.h"

typedef enum                                                                    // 枚举 ADC 引脚通道  此枚举定义不允许用户修改
{
    ADC1_CH0_A0         = 0x0000,                                               // 0x 0[ADC1] 0[CH00] 00[A0]
    ADC1_CH1_A1         = 0x0101,                                               // 0x 0[ADC1] 1[CH01] 01[A1]
    ADC1_CH2_A2         = 0x0202,                                               // 0x 0[ADC1] 2[CH02] 02[A2]
    ADC1_CH3_A3         = 0x0303,                                               // 0x 0[ADC1] 3[CH03] 03[A3]
    ADC1_CH4_A4         = 0x0404,                                               // 0x 0[ADC1] 4[CH04] 04[A4]
    ADC1_CH5_A5         = 0x0505,                                               // 0x 0[ADC1] 5[CH05] 05[A5]
    ADC1_CH6_A6         = 0x0606,                                               // 0x 0[ADC1] 6[CH06] 06[A6]
    ADC1_CH7_A7         = 0x0707,                                               // 0x 0[ADC1] 7[CH07] 07[A7]
    ADC1_CH8_B0         = 0x0820,                                               // 0x 0[ADC1] 8[CH08] 20[B0]
    ADC1_CH9_B1         = 0x0921,                                               // 0x 0[ADC1] 9[CH09] 21[B1]
    ADC1_CH10_C0        = 0x0A40,                                               // 0x 0[ADC1] A[CH10] 40[C0]
    ADC1_CH11_C1        = 0x0B41,                                               // 0x 0[ADC1] B[CH11] 41[C1]
    ADC1_CH12_C2        = 0x0C42,                                               // 0x 0[ADC1] C[CH12] 42[C2]
    ADC1_CH13_C3        = 0x0D43,                                               // 0x 0[ADC1] D[CH13] 43[C3]

    ADC1_CH14_TEMP      = 0x0EFF,                                               // 0x 0[ADC1] E[CH14] 温度传感器通道
    ADC1_CH15_VS        = 0x0FFF,                                               // 0x 0[ADC1] F[CH15] 内部参考电压通道

    ADC2_CH0_A0         = 0x1000,                                               // 0x 1[ADC2] 0[CH00] 00[A0]
    ADC2_CH1_A1         = 0x1101,                                               // 0x 1[ADC2] 1[CH01] 01[A1]
    ADC2_CH2_A2         = 0x1202,                                               // 0x 1[ADC2] 2[CH02] 02[A2]
    ADC2_CH3_A3         = 0x1303,                                               // 0x 1[ADC2] 3[CH03] 03[A3]
    ADC2_CH4_A4         = 0x1404,                                               // 0x 1[ADC2] 4[CH04] 04[A4]
    ADC2_CH5_A5         = 0x1505,                                               // 0x 1[ADC2] 5[CH05] 05[A5]
    ADC2_CH6_A6         = 0x1606,                                               // 0x 1[ADC2] 6[CH06] 06[A6]
    ADC2_CH7_A7         = 0x1707,                                               // 0x 1[ADC2] 7[CH07] 07[A7]
    ADC2_CH8_B0         = 0x1820,                                               // 0x 1[ADC2] 8[CH08] 20[B0]
    ADC2_CH9_B1         = 0x1921,                                               // 0x 1[ADC2] 9[CH09] 21[B1]
    ADC2_CH10_C0        = 0x1A40,                                               // 0x 1[ADC2] A[CH10] 40[C0]
    ADC2_CH11_C1        = 0x1B41,                                               // 0x 1[ADC2] B[CH11] 41[C1]
    ADC2_CH12_C2        = 0x1C42,                                               // 0x 1[ADC2] C[CH12] 42[C2]
    ADC2_CH13_C3        = 0x1D43,                                               // 0x 1[ADC2] D[CH13] 43[C3]
    ADC2_CH14_C4        = 0x1E44,                                               // 0x 1[ADC2] E[CH14] 44[C4]
    ADC2_CH15_C5        = 0x1F45,                                               // 0x 1[ADC2] F[CH15] 45[C5]

}adc_channel_enum;

typedef enum                                                                    // 枚举 ADC 分辨率   此枚举定义不允许用户修改
{
    ADC_12BIT,                                                                  // 12位分辨率
    ADC_11BIT,                                                                  // 11位分辨率
    ADC_10BIT,                                                                  // 10位分辨率
    ADC_9BIT,                                                                   // 9位分辨率
    ADC_8BIT,                                                                   // 8位分辨率
}adc_resolution_enum;

uint16      adc_convert             (adc_channel_enum ch);
uint16      adc_mean_filter_convert (adc_channel_enum ch, const uint8 count);
void        adc_init                (adc_channel_enum ch, adc_resolution_enum resolution);

#endif
