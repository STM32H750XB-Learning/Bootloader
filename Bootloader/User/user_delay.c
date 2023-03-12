#include "user_delay.h"

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

