#ifndef _CAR_COMMUNCATION_H_
#define _CAR_COMMUNCATION_H_

#include "config.h"
/***********************************
CTR  7/6 λ��	
	00  ������ȡС������������ 					������->С����
	01  ��������С����̬  						������->С����
	10  ��������С����̬ͬʱ��ȡ����������  	������->С����
	11  ������������ 							��С��->������

CTR  5/ 4 λ��
	����λֻ����CTR->7,6 Ϊ01��10ʱ��Ч
	00 ����С��ǰ��(��ʱ��DAT1��Ϊ�ٶȣ�DAT2��Ϊת�����)
	01 ����С������(��ʱ��DAT1��Ϊ�ٶȣ�DAT2��Ϊת�����)
	10 ����С�����ͷ(��ʱ��DAT1��Ϊ�ٶ�)
	11 ����С���ҵ�ͷ(��ʱ��DAT1��Ϊ�ٶ�)

CTR  3 2 λ��
����λֻ����CTR->7,6 Ϊ00,01��11ʱ��Ч

��CTR 7 6 = 00 �� 10ʱ��
00 ���� ������
01 ���� ����̬����
10 ���� ���ٶ�

��CTR 7 6 = 11ʱ��
00 ���� ���ص��Ǿ���(��ʱ��DAT1��DAT2��Ϊ����)
    01 ���� ���ص�����̬����(��ʱ��DAT1��DAT2��DAT3 ��Ϊ��̬)
10 ���� ���ص����ٶ�(��ʱ��DAT1��Ϊ�ٶ�)
*******************************************************/


#define  Car_Communcate_BaudRate  115200

void Car_CommuncateInit(void);
void Car_CMDCheck(void);
void DirEncode(int dir);
void SpeedEncode(int speed);
float DistanceDecode(void);
int DirDecode(void);
int SpeedDecode(void);
void Car_CMDProcess(void);

#endif
