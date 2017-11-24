#include "car_hcsr04.h"
 
u16 msHcCount = 0;//ms计数

/*************************************************************************
* 名    称：void Car_Hcsr04Init(void)
* 功    能：测距初始化
* 入口参数：无
* 出口参数：无
* 说    明，用到TIM4，输出管脚为PB5，PB6
* 调用方法： 
****************************************************************************/
 
void Car_Hcsr04Init()
{
	
				TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;     //生成用于定时器设置的结构体
				NVIC_InitTypeDef NVIC_InitStructure; 
				GPIO_InitTypeDef GPIO_InitStructure;
					
				RCC_APB2PeriphClockCmd(HCSR04_CLK, ENABLE);
				//IO初始化
				GPIO_InitStructure.GPIO_Pin =HCSR04_TRIG;      					  //发送电平引脚
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				  //推挽输出
				GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);
				GPIO_ResetBits(HCSR04_PORT,HCSR04_TRIG);
				 
				GPIO_InitStructure.GPIO_Pin =   HCSR04_ECHO;              //返回电平引脚
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;							//下拉输入
				GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);        
	
        //定时器初始化 使用基本定时器TIM4       
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);   			//使能对应RCC时钟
        //配置定时器基础结构体
        TIM_TimeBaseStructure.TIM_Period = 1000;										 //设置在下一个更新事件装入活动的自动重装载寄存器周期的值         计数到1000为1ms
        TIM_TimeBaseStructure.TIM_Prescaler =(72-1);								 //设置用来作为TIMx时钟频率除数的预分频值  1M的计数频率 1US计数
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
        TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 							//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位  
				
				TIM_ClearFlag(TIM4, TIM_FLAG_Update);  			 //清除更新中断，免得一打开中断立即产生中断
        TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);   	 //打开定时器更新中断
				
				NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
				NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
				NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
				NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
				NVIC_Init(&NVIC_InitStructure);
				      
}

/*************************************************************************
* 名    称：static void OpenTimerForHc() 
* 功    能：开定时器
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法： 
****************************************************************************/
static void OpenTimerForHc()       //打开定时器
{
        TIM_SetCounter(TIM4,0);		//清除计数
        msHcCount = 0;
        TIM_Cmd(TIM4, ENABLE); 		//使能TIMx外设
}
 
/*************************************************************************
* 名    称：static void CloseTimerForHc() 
* 功    能：关闭定时器
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法： 
****************************************************************************/
static void CloseTimerForHc()        //关闭定时器
{
        TIM_Cmd(TIM4, DISABLE);  //使能TIMx外设
}
 
 
//定时器4中断服务程序
void TIM4_IRQHandler(void)   //TIM4中断
{
        if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //检查TIM6更新中断发生与否
        {
                TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx更新中断标志 
                msHcCount++;
        }
}
 
/*************************************************************************
* 名    称：u32 GetEchoTimer(void)
* 功    能：获取延时时间值
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法： 
****************************************************************************/
u32 GetEchoTimer(void)
{
        u32 t = 0;
        t = msHcCount*1000;//得到MS
        t += TIM_GetCounter(TIM4);//得到US
        return t;
}
 


//一次获取超声波测距数据 两次测距之间需要相隔一段时间，隔断回响信号
/***************************************************************************
* 名    称：unsigned int Hcsr04GetLength()
* 功    能：获取距离值
* 入口参数：无
* 出口参数：距离（cm）
* 说    明：
* 调用方法： 
****************************************************************************/
u32 Hcsr04GetLength()
{
    u32 t = 0;
    float lengthTemp;
    TRIG_Send = 1;      //发送口高电平输出
	
    Delay_Us(10);
	
    TRIG_Send = 0;
    while(ECHO_Reci == 0);     //等待接收口高电平输出
		OpenTimerForHc();        //打开定时器
		while(ECHO_Reci == 1);
		CloseTimerForHc();        //关闭定时器
		t = GetEchoTimer();        //获取时间,分辨率为1US
		lengthTemp = ((float)t/58);//cm
    return (u32)lengthTemp; 
}
