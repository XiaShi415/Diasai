#include "car_communcation.h"

char  Car_CommuncateBuffer_RX[8] = {0};
char  Car_CommuncateBuffer_TX[8] = {0};
char  Car_Dat_temp[6] = {0};
unsigned int tmp;
char Receive_Flag = 0;
char Check_Flag = 0;
char iHead,iStack= 0;

/****************************************************************************
* 名    称：void Car_CommuncateInit(void)
* 功    能：通信口初始化
* 入口参数：无
* 出口参数：无
* 说    明，用到usart2，输出管脚为PA2，PA3
* 调用方法： 
****************************************************************************/
void Car_CommuncateInit(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		
		/* config USART2 clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

		/* USART2 GPIO config */
		 /* Configure USART2 Tx (PA.02) as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
				
		/* Configure USART2 Rx (PA.03) as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
			
		/* USART2 mode config */
		USART_InitStructure.USART_BaudRate = Car_Communcate_BaudRate;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

		USART_Init(USART2, &USART_InitStructure); 
		
		USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);	//开接收中断
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                                      //选择中断分组2
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;                                    //选择串口2中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;                            //抢占式中断优先级设置为0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                                   //响应式中断优先级设置为0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                      //使能中断
		NVIC_Init(&NVIC_InitStructure);

		USART_Cmd(USART2, ENABLE);	
		
}
void USART2_IRQHandler(void)
{
	  if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET)
		{
				Car_CommuncateBuffer_RX[iStack] = USART_ReceiveData(USART2);
				USART_SendData(USART1,Car_CommuncateBuffer_RX[iStack]);			//向串口1发送数据
			  iStack++;
		}
	if(iStack==8)
	 {		
			
		    Receive_Flag = 1;  
   } 
}

/****************************************************************************
* 名    称：void Car_CMDCheck()
* 功    能：对接受的数据流进行检查并返回命令帧的帧头在数组中的位置
* 入口参数：无
* 出口参数：无
* 说    明，
* 调用方法： 
****************************************************************************/
void Car_CMDCheck(void)
{	
	int iCheck,i = 0;
	char SumCheck = 0;

	if(Receive_Flag == 1) 
	{
			Receive_Flag = 0;
	
/********    判断是否为数据帧  **************/
		  for(iCheck = 0;iCheck <=8;iCheck++)
				{
					if(Car_CommuncateBuffer_RX[iCheck]==0x5e && Car_CommuncateBuffer_RX[iCheck+1]==0x5e)   //帧头确认
						{
							if(Car_CommuncateBuffer_RX[iCheck+7] ==0x4d)			//帧尾确认
								{
									iHead = iCheck;
									iStack = 0;									
									break;
								}
							else
								return;
						}
					 else 
							continue;
				}
				
/***********     检验误码     ***************/
		for(i=iHead+2;i<iHead+6;i++)
			{
				SumCheck = (char )(Car_CommuncateBuffer_RX[i]+SumCheck);
			}
	 if(SumCheck == Car_CommuncateBuffer_RX[iHead+6])
		 {
			 Check_Flag = 1;
		 }		 			
	}
	return;
}
/****************************************************************************
* 名    称：void Car_CMDProcess()
* 功    能：对接受的数据流进行处理
* 入口参数：无
* 出口参数：无
* 说    明，
* 调用方法： 
****************************************************************************/
void Car_CMDProcess(void)
{
	int iSpeed=0;
	int iDir = 0;
	Car_CMDCheck();
	if(Check_Flag != 1)
		return;
	
	Check_Flag = 0;
	/************************   小车动作处理    ****************************/
	if((Car_CommuncateBuffer_RX[iHead+2] & 0xC0) == 0x40)
		{
			iSpeed = SpeedDecode();
			iDir = DirDecode();
			if((Car_CommuncateBuffer_RX[iHead+2] & 0x30) == 0x00)   //前进
				{
					USART_SendData(USART2,0x31);
									while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
				 Car_Go(iSpeed,iDir,Forward);
				}
			if((Car_CommuncateBuffer_RX[iHead+2] & 0x30) == 0x10)	//后退
				{
					USART_SendData(USART2,0x32);
									while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
					Car_Go(iSpeed,iDir,Back);
				}
			if((Car_CommuncateBuffer_RX[iHead+2] & 0x30) == 0x20)  //右掉头
				{
					USART_SendData(USART2,0x33);
									while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
					Car_TurnRound(iSpeed,Left);
				}
			if((Car_CommuncateBuffer_RX[iHead+2] & 0x3F) == 0x3F)		//螺旋转圈开始
				{
					RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 ,ENABLE);	
					Car_SelectSignal();
				}
				if((Car_CommuncateBuffer_RX[iHead+2] & 0x3F) == 0x30)		//螺旋转圈停止
				{
					RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 ,DISABLE);	
					SelectTim=0;
					SelectDir =0;
					Car_Drive_TIM->CCR1 = 0;		//将计算出的占空比数值写入寄存器
					Car_Drive_TIM->CCR2 = 0;		
				}
		}			
}

/****************************************************************************
* 名    称：int SpeedDecode()
* 功    能：对接受的数据流解码速度值
* 入口参数：无
* 出口参数：无
* 说    明，
* 调用方法： 
****************************************************************************/
int SpeedDecode()
{
	u8 hundred,dec, unit = 0;
	hundred	 = Car_CommuncateBuffer_RX[iHead+3] >>4;
	dec			 = Car_CommuncateBuffer_RX[iHead+3] & 0x0F;
	unit		 = Car_CommuncateBuffer_RX[iHead+4] >>4;
	return	hundred * 100 + dec * 10 + unit;
}
/****************************************************************************
* 名    称：int DirDecode()
* 功    能：对接受的数据流解码方向参数
* 入口参数：无
* 出口参数：无
* 说    明，
* 调用方法： 
****************************************************************************/
int DirDecode()
{
	u8 hundred,dec, unit = 0;
	hundred	 = Car_CommuncateBuffer_RX[iHead+4] & 0x0F;
	dec			 = Car_CommuncateBuffer_RX[iHead+5] >>4;
	unit		 = Car_CommuncateBuffer_RX[iHead+5] & 0X0F;
	return	hundred * 100 + dec * 10 + unit;
}
/****************************************************************************
* 名    称：int DistanceDecode()
* 功    能：对接受的数据流解码速度值
* 入口参数：无
* 出口参数：无
* 说    明，
* 调用方法： 
****************************************************************************/
float DistanceDecode()
{
	u8 hundred,dec,unit,fractional = 0;
	float distance = 0.0;
	
	hundred	 		 = Car_CommuncateBuffer_RX[iHead+3] >>4;
	dec					 = Car_CommuncateBuffer_RX[iHead+3] & 0x0F;
	unit				 = Car_CommuncateBuffer_RX[iHead+4] >>4;
	fractional	 = Car_CommuncateBuffer_RX[iHead+4] & 0x0F;
	
	distance =  (float)hundred * 100 + dec * 10 + unit + fractional * 0.1;
	
	return distance;
}

/****************************************************************************
* 名    称：void SpeedEncode(int speed)
* 功    能: 编码速度参数
* 入口参数：速度参数
* 出口参数：无
* 说    明，
* 调用方法： 
****************************************************************************/
void SpeedEncode(int speed)
{
	u8 hundred,dec, unit = 0;
	
	hundred	 = speed / 100;
	dec			 = (speed/10)%10;
	unit		 = speed%1;
	
	Car_CommuncateBuffer_TX[3] = (char)(hundred<<4)|dec;
	
	Car_CommuncateBuffer_TX[4] &= 0x0f;   									//清高四位
	Car_CommuncateBuffer_TX[4] |=(char)unit<<4;
	
}


/****************************************************************************
* 名    称：void DireEncode(int dir)
* 功    能: 编码方向参数
* 入口参数：方向参数
* 出口参数：无
* 说    明，
* 调用方法： 
****************************************************************************/
void DirEncode(int dir)
{
	u8 hundred,dec, unit = 0;
	hundred	 = dir / 100;
	dec			 = (dir/10)%10;
	unit		 = dir%1;
	
	Car_CommuncateBuffer_TX[4] &= 0xf0;              //清低四位
	Car_CommuncateBuffer_TX[4] |= (char)hundred;
	
	Car_CommuncateBuffer_TX[5] =  (char)(dec<<4)|unit;
	
}

