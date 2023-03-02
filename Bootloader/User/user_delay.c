#include "user_delay.h"

// extern TIM_HandleTypeDef htim6;

// /*
// *********************************************************************************************************
// *	函 数 名	: user_delay_us
// *	功能说明	: 按us进行延时
// *	形    参	: us: 延迟的us数，0~65530
// *	返 回 值	: 无
// *	说明		: 普通定时器实现us延时	
// *********************************************************************************************************
// */
// void user_delay_us(uint16_t us)
// {
//     uint16_t  differ = 0xffff-us-5;
//     //设置定时器2的技术初始值
//     __HAL_TIM_SetCounter(&htim6, differ);
//     //开启定时器
//     HAL_TIM_Base_Start(&htim6);

//     while( differ<0xffff-5)
//     {
//         differ = __HAL_TIM_GetCounter(&htim6);
//     };
//     //关闭定时器
//     HAL_TIM_Base_Stop(&htim6);
// }

// /*
// *********************************************************************************************************
// *	函 数 名	: user_delay_ms
// *	功能说明	: 按ms进行延时
// *	形    参	: ms: 延迟的ms数 
// *	返 回 值	: 无
// *	说明		: 普通定时器实现ms延时	
// *********************************************************************************************************
// */
// void user_delay_ms(uint32_t ms)
// {
// 	for(uint32_t i =0; i < ms; i++)
// 		user_delay_us(1000);
// }

/*
*********************************************************************************************************
*	函 数 名	: HAL_Delay_us
*	功能说明	: 按us进行延时
*	形    参	: us: 延迟的us数，0~2^32/fac_us, fac_us为SysTick时钟的频率倍数，在FreeRTOS中为AHB时钟频率
*	返 回 值	: 无
*	说明		: 滴答定时器实现us延时	
*********************************************************************************************************
*/
void HAL_Delay_us(uint16_t us)
{
#define fac_us 	480
	
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;    //LOAD的值
    ticks = us * fac_us;   //需要的节拍数
    told = SysTick->VAL;    //刚进入时的计数器的值
    while (1)
    {
        tnow = SysTick->VAL;
        if(tnow != told)
        {
            if(tnow < told) tcnt += told - tnow;    //这里注意一下SYSTICK是一个递减计数器
            else tcnt += reload - tnow + told;  //计数器向下溢出
            told = tnow;
            if(tcnt >= ticks) break;    //时间超过/等于要延迟的时间，则退出
        }
    }
}

