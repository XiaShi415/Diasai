#include "car_communcation.h"

char  Car_CommuncateBuffer_RX[8] = {0};
char  Car_CommuncateBuffer_TX[8] = {0};
char  Car_Dat_temp[6] = {0};
unsigned int tmp;
char Receive_Flag = 0;
char Check_Flag = 0;
char iHead,iStack= 0;

/****************************************************************************
* ��    �ƣ�void Car_CommuncateInit(void)
* ��    �ܣ�ͨ�ſڳ�ʼ��
* ��ڲ�������
* ���ڲ�������
* ˵    �����õ�usart2������ܽ�ΪPA2��PA3
* ���÷����� 
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
		
		USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);	//�������ж�
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                                      //ѡ���жϷ���2
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;                                    //ѡ�񴮿�2�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;                            //��ռʽ�ж����ȼ�����Ϊ0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                                   //��Ӧʽ�ж����ȼ�����Ϊ0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                      //ʹ���ж�
		NVIC_Init(&NVIC_InitStructure);

		USART_Cmd(USART2, ENABLE);	
		
}
void USART2_IRQHandler(void)
{
	  if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET)
		{
				Car_CommuncateBuffer_RX[iStack] = USART_ReceiveData(USART2);
				USART_SendData(USART1,Car_CommuncateBuffer_RX[iStack]);			//�򴮿�1��������
			  iStack++;
		}
	if(iStack==8)
	 {		
			
		    Receive_Flag = 1;  
   } 
}

/****************************************************************************
* ��    �ƣ�void Car_CMDCheck()
* ��    �ܣ��Խ��ܵ����������м�鲢��������֡��֡ͷ�������е�λ��
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷����� 
****************************************************************************/
void Car_CMDCheck(void)
{	
	int iCheck,i = 0;
	char SumCheck = 0;

	if(Receive_Flag == 1) 
	{
			Receive_Flag = 0;
	
/********    �ж��Ƿ�Ϊ����֡  **************/
		  for(iCheck = 0;iCheck <=8;iCheck++)
				{
					if(Car_CommuncateBuffer_RX[iCheck]==0x5e && Car_CommuncateBuffer_RX[iCheck+1]==0x5e)   //֡ͷȷ��
						{
							if(Car_CommuncateBuffer_RX[iCheck+7] ==0x4d)			//֡βȷ��
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
				
/***********     ��������     ***************/
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
* ��    �ƣ�void Car_CMDProcess()
* ��    �ܣ��Խ��ܵ����������д���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷����� 
****************************************************************************/
void Car_CMDProcess(void)
{
	int iSpeed=0;
	int iDir = 0;
	Car_CMDCheck();
	if(Check_Flag != 1)
		return;
	
	Check_Flag = 0;
	/************************   С����������    ****************************/
	if((Car_CommuncateBuffer_RX[iHead+2] & 0xC0) == 0x40)
		{
			iSpeed = SpeedDecode();
			iDir = DirDecode();
			if((Car_CommuncateBuffer_RX[iHead+2] & 0x30) == 0x00)   //ǰ��
				{
					USART_SendData(USART2,0x31);
									while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
				 Car_Go(iSpeed,iDir,Forward);
				}
			if((Car_CommuncateBuffer_RX[iHead+2] & 0x30) == 0x10)	//����
				{
					USART_SendData(USART2,0x32);
									while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
					Car_Go(iSpeed,iDir,Back);
				}
			if((Car_CommuncateBuffer_RX[iHead+2] & 0x30) == 0x20)  //�ҵ�ͷ
				{
					USART_SendData(USART2,0x33);
									while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
					Car_TurnRound(iSpeed,Left);
				}
			if((Car_CommuncateBuffer_RX[iHead+2] & 0x3F) == 0x3F)		//����תȦ��ʼ
				{
					RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 ,ENABLE);	
					Car_SelectSignal();
				}
				if((Car_CommuncateBuffer_RX[iHead+2] & 0x3F) == 0x30)		//����תȦֹͣ
				{
					RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 ,DISABLE);	
					SelectTim=0;
					SelectDir =0;
					Car_Drive_TIM->CCR1 = 0;		//���������ռ�ձ���ֵд��Ĵ���
					Car_Drive_TIM->CCR2 = 0;		
				}
		}			
}

/****************************************************************************
* ��    �ƣ�int SpeedDecode()
* ��    �ܣ��Խ��ܵ������������ٶ�ֵ
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷����� 
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
* ��    �ƣ�int DirDecode()
* ��    �ܣ��Խ��ܵ����������뷽�����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷����� 
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
* ��    �ƣ�int DistanceDecode()
* ��    �ܣ��Խ��ܵ������������ٶ�ֵ
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷����� 
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
* ��    �ƣ�void SpeedEncode(int speed)
* ��    ��: �����ٶȲ���
* ��ڲ������ٶȲ���
* ���ڲ�������
* ˵    ����
* ���÷����� 
****************************************************************************/
void SpeedEncode(int speed)
{
	u8 hundred,dec, unit = 0;
	
	hundred	 = speed / 100;
	dec			 = (speed/10)%10;
	unit		 = speed%1;
	
	Car_CommuncateBuffer_TX[3] = (char)(hundred<<4)|dec;
	
	Car_CommuncateBuffer_TX[4] &= 0x0f;   									//�����λ
	Car_CommuncateBuffer_TX[4] |=(char)unit<<4;
	
}


/****************************************************************************
* ��    �ƣ�void DireEncode(int dir)
* ��    ��: ���뷽�����
* ��ڲ������������
* ���ڲ�������
* ˵    ����
* ���÷����� 
****************************************************************************/
void DirEncode(int dir)
{
	u8 hundred,dec, unit = 0;
	hundred	 = dir / 100;
	dec			 = (dir/10)%10;
	unit		 = dir%1;
	
	Car_CommuncateBuffer_TX[4] &= 0xf0;              //�����λ
	Car_CommuncateBuffer_TX[4] |= (char)hundred;
	
	Car_CommuncateBuffer_TX[5] =  (char)(dec<<4)|unit;
	
}

