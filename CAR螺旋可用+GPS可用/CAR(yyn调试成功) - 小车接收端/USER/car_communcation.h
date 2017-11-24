#ifndef _CAR_COMMUNCATION_H_
#define _CAR_COMMUNCATION_H_

#include "config.h"
/***********************************
CTR  7/6 位：	
	00  ——读取小车传感器数据 					（主机->小车）
	01  ——控制小车姿态  						（主机->小车）
	10  ——控制小车姿态同时读取传感器数据  	（主机->小车）
	11  ——返回数据 							（小车->主机）

CTR  5/ 4 位：
	此两位只有在CTR->7,6 为01或10时有效
	00 ——小车前进(此时，DAT1中为速度，DAT2中为转向参数)
	01 ——小车后退(此时，DAT1中为速度，DAT2中为转向参数)
	10 ——小车左调头(此时，DAT1中为速度)
	11 ——小车右调头(此时，DAT1中为速度)

CTR  3 2 位：
此两位只有在CTR->7,6 为00,01或11时有效

当CTR 7 6 = 00 或 10时：
00 —— 读距离
01 —— 读姿态数据
10 —— 读速度

当CTR 7 6 = 11时：
00 —— 返回的是距离(此时，DAT1，DAT2中为距离)
    01 —— 返回的是姿态数据(此时，DAT1，DAT2，DAT3 中为姿态)
10 —— 返回的是速度(此时，DAT1中为速度)
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
