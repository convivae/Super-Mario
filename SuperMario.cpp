#include "stdafx.h"
#include "SuperMario.h"
#include <math.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qpainter.h>
#include <qvector.h>
#include <qdebug.h>

RollScreen f;
QVector<QPixmap> objectCell;	//所有图片
QVector<QPixmap> mirrorCell;	//所有图片的镜像
QImage mirroredImage;	//图片的镜像（临时变量）
QPointF V_MAX_NORMAL = QPointF(4, 12);		//正常速度的最大值
QPointF V_MAX_ACCELT = QPointF(6, 12);		//加速状态的最大值

//创建马里奥对象的状态
status status_short(247, 274, 160, 163, 302, 0, 291, 0, 215, 1);
status status_middle(188, 229, 225, 131, 258, 139, 287, 0, 215, 2);
status status_super(5, 137, 13, 148, 23, 286, 20, 224, 215, 2);
status *ptrStatus = &status_short;	//默认状态为1

//创建各种怪物的状态
Monster_Pattern	turtle_green(49, 92, 52, 72, 75, 7);
Monster_Pattern turtle_red(165, 42, 250, 265, 196, 264);
Monster_Pattern mushroom_brown(147, 0, 187, 0, 0, 0);
Monster_Pattern mushroom_blue(26, 0, 86, 0, 0, 0);

//主角马里奥
CharacterMario mario;

//创建云对象(18个)
Cloud cloud[18]{
	{ 4,5 },{ 8,2 },{ 12,1 },{ 16,3 },{ 20,2 },{ 26,3 },{ 30,1 },{ 36,2 },{ 44,1 },{ 50,4 },{ 56,3 },{ 68,2 },{ 80,1 },{ 84,3 },{ 100,1 },{ 120,3 },{ 146,2 },{ 168,3 }
};

//创建乌龟对象
MonsterTurtle turtle[8]{
	{12, 12, 1},{36,12,2},{139,12,3},{115,12,3},{83,2,1},{170,12,3},{49,7,0},{187,8,1}
};

//花
Flower flower1(9, 9);

//问号,构造14个对象
QuestionMark question[14] = {
	{ 74,9 },{ 14,9 },{ 15,5 },{ 16,9 },{ 139,1 },{ 142,1 },{ 114,8 },{ 115,8 },{ 116,8 },{ 117,8 },{ 77,9 },{ 159,9 },{ 160,9 },{ 161,9 }
};

//金币
Coin coins[9] = {
	{ 14,2 },{ 15,2 },{ 16,2 },{ 76,2 },{ 77,2 },{ 78,2 },{ 156,5 },{ 157,5 },{ 158,5 }
};

HideCoin hidecoin[12] = {
	{ 15,4 },{ 16,8 },{ 139,0 },{ 142,0 },{ 114,7 },{ 115,7 },{ 116,7 },{ 117,7 },{ 77,8 },{ 159,8 },{ 160,8 },{ 161,8 }
};

Mushroom mushroom1(74, 9);
Mushroom mushroom2(14, 9);

//地图，该数组控制网格中的所有物体的填充，数组中的数字对应Image中的数字
static int mapLevel_2[15][203] = {
{ 74,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	74 },
{ 74,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	77,	255,77,	77,	255,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	74 },
{ 74,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	74 },
{ 74,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	74 },
{ 74,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	74,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	74 },
{ 74,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	77,	255,77,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	77,	77,	77,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	74 },
{ 74,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	74,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	74 },
{ 74,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	74 },
{ 74,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	77,	255,255,255,255,77,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	74,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	74 },
{ 74,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	77,	255,77,	255,77,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	77,	77,	255,77,	77,	255,77,	77,	77,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	77,	77,	77,	77,	77,	255,255,255,77,	77,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	74 },
{ 74,	74,	74,	74,	0,	0,	0,	0,	0,	114,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	114,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	114,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	114,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	74,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	114,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	114,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	114,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	74 },
{ 74,	74,	74,	74,	74,	0,	0,	0,	0,	114,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	114,0,	0,	0,	0,	0,	0,	0,	0,	0,	114,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	114,0,	0,	0,	0,	0,	0,	0,	0,	114,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	114,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	74,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	74,	0,	0,	0,	0,	0,	0,	0,	0,	0,	114,114,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	114,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	114,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	74 },
{ 74,	74,	74,	74,	74,	74,	0,	0,	0,	114,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	74,	0,	0,	0,	74,	0,	0,	114,0,	0,	0,	0,	0,	0,	0,	0,	0,	114,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	114,0,	0,	0,	0,	0,	0,	0,	0,	114,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	114,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	74,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	74,	0,	0,	0,	0,	0,	0,	0,	0,	0,	114,114,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	114,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	114,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	74 },
{ 211,	195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,0,	0,	0,	195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,0,	0,	0,	195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,0,	0,	0,	195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,0,	0,	0,	195,195,195,195,195,195,195,195,195,195,195,195,0,	0,	0,	195,195,0,	0,	0,	195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,0,	0,	195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,0,	0,	0,	195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,182 },
{ 119,	152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,0,	0,	0,	152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,0,	0,	0,	152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,0,	0,	0,	152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,0,	0,	0,	152,152,152,152,152,152,152,152,152,152,152,152,0,	0,	0,	152,152,0,	0,	0,	152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,0,	0,	152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,0,	0,	0,	152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,152,155 }
};


//（碰撞判断）判断能否穿过的数组(使用c语言生成)
bool collion_judge2[15][203] = {
{ 1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1 },
{ 1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,1,	1,	1,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1 },
{ 1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1 },
{ 1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1 },
{ 1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1 },
{ 1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1 },
{ 1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1 },
{ 1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1 },
{ 1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,1,1,1,1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1 },
{ 1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,1,	1,1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	1,1,	1,	1,1,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	1,	1,	1,	1,1,1,1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1 },
{ 1,	1,	1,	1,	0,	0,	0,	0,	0,	1,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1 },
{ 1,	1,	1,	1,	1,	0,	0,	0,	0,	1,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,0,	0,	0,	0,	0,	0,	0,	0,	0,	1,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,0,	0,	0,	0,	0,	0,	0,	0,	1,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,1,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1 },
{ 1,	1,	1,	1,	1,	1,	0,	0,	0,	1,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	1,	0,	0,	1,0,	0,	0,	0,	0,	0,	0,	0,	0,	1,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,0,	0,	0,	0,	0,	0,	0,	0,	1,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,1,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1 },
{ 1,	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,	0,	0,	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,	0,	0,	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,	0,	0,	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,	0,	0,	1,1,1,1,1,1,1,1,1,1,1,1,0,	0,	0,	1,1,0,	0,	0,	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,	0,	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,	0,	0,	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
{ 1,	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,	0,	0,	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,	0,	0,	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,	0,	0,	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,	0,	0,	1,1,1,1,1,1,1,1,1,1,1,1,0,	0,	0,	1,1,0,	0,	0,	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,	0,	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,	0,	0,	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
};

double min(double a, double b)
{
	return(a > b ? b : a);
}

double max(double a, double b)
{
	return(a < b ? b : a);
}

int my_coin = 0;
int my_score = 0;
int left_lives = 1;

SuperMario::SuperMario(QWidget *parent)
	: QWidget(parent)
{// 在构造函数中的代码都是在整个程序运行过程中只需要做一次的代码
	ui.setupUi(this);
	setFixedSize(SIDE_LEN * 20, SIDE_LEN * 15);	//固定窗口的大小,共20 * 15 个方格
	objectCell.resize(311);
	mirrorCell.resize(311);


	for (int i = 1; i < 311; i++) {
		objectCell[i].load(QString("../Image/image%1.png").arg(i));
		mirroredImage = objectCell[i].toImage().mirrored(TRUE, FALSE);	//镜像图片
		mirrorCell[i] = QPixmap::fromImage(mirroredImage);
	}
	//timerId为标识符
	id1 = this->startTimer(TIME_STEP);	//以毫秒为单位
	id2 = this->startTimer(TIME_STEP / FRAME);	//60HZ

	//键盘焦点设置，没有的话接收不到方向键和空格键
	this->setFocus();

	QString text_worlds = QString("<center><h1>WORLDS</h1></center><center><h1> 1 - 1</h1></center>").arg(my_coin);
	ui.WORLD_label->setText(text_worlds);
}


void SuperMario::game_over()
{
	int choose = QMessageBox::information(this, "提示","Game Over!", QMessageBox::Ok);
	if (left_lives >= 0) {
		left_lives--;
		start_game();
	}
	else {
		this->close();
	}
}

void SuperMario::you_win()
{
	int choose = QMessageBox::warning(this, "You Win!", "Do you want to start another game?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
	//如果用户点击了No，那么窗口就会关闭
	if (choose == QMessageBox::Yes)
		start_game();
	else
		this->close();
}

void SuperMario::start_game()
{
	QProcess::startDetached(QApplication::applicationFilePath());
	exit(12);
}

void SuperMario::paintEvent(QPaintEvent * pt)	//背景绘制
{
	//1.渐变色蓝天
	QPainter p(this);	//创建画家对象并指定当前窗口为绘图设备
	QLinearGradient linearGradient(0, 0, 0, height());		//从左上角开始，到左下角结束
	linearGradient.setColorAt(0, QColor(37, 114, 218));		//RGB从原游戏中取色得到
	linearGradient.setColorAt(1, QColor(225, 236, 250));
	p.setBrush(linearGradient);
	p.drawRect(0, 0, width(), height());

	//2.背景中的土地，道路，山，砖块等

	f.b.rx() = mario.r.x() > 10 ? 420 - mario.r.x() * SIDE_LEN : 0;
	p.drawPixmap(f(QPoint(0, 9)).x(), f(QPoint(0, 9)).y(), 10 * SIDE_LEN, 4 * SIDE_LEN, objectCell[251]);
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 203; j++)
			if (mapLevel_2[i][j] != 0)
			{
				QPointF r = f(QPoint(j, i));
				p.drawPixmap(r.x(), r.y(), SIDE_LEN, SIDE_LEN, objectCell[mapLevel_2[i][j]]);
			}

	//3.白云
	for (int i = 0; i < 18; i++)
		cloud[i].paint(p);

	//4.主角mario
	mario.paint(p);

	//5.乌龟
	for (int i = 0; i < 8; i++) {
		if (turtle->r.x() - mario.r.x() <= DISTANCE)
			turtle[i].paint(p);
	}
		

	//6.花
	flower1.paint(p);

	//蘑菇
	if (question[0].die_contral == 1 && mushroom1.die_contral == 0)
		mushroom1.paint(p);
	if (question[1].die_contral == 1 && mushroom2.die_contral == 0)
		mushroom2.paint(p);

	//7.问号
	question[0].paint(p);
	question[1].paint(p);

	for (int i = 2; i < 14; i++) {
		question[i].paint(p);
		if (question[i].die_contral == 1 && hidecoin[i - 2].die_contral == 0)
			hidecoin[i - 2].paint(p);
	}
	


	//8.coin
	for (int i = 0; i < 9; i++)
		coins[i].paint(p);

	p.drawPixmap(6.2 * SIDE_LEN, 1.6 * SIDE_LEN, 0.7 * SIDE_LEN, 0.8 * SIDE_LEN, objectCell[8]);
}



//关闭窗口事件
void SuperMario::closeEvent(QCloseEvent *e)
{
	int choose = QMessageBox::warning(this, "Warning!", "Are you sure you want to quit?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

	//如果用户点击了Yes，那么窗口就会关闭
	if (choose == QMessageBox::Yes)
		e->accept();
	else
		e->ignore();	//ignore后传递给父组件QWidget
}


//键盘按下
void SuperMario::keyPressEvent(QKeyEvent * ev)
{
	mario.keyPressEvent(ev);
}

void SuperMario::keyReleaseEvent(QKeyEvent * ev)
{
	mario.keyReleaseEvent(ev);
}

void judge_questionmark(QuestionMark &q)
{
	if (QRect(q.r.x() * SIDE_LEN, q.r.y() * SIDE_LEN + 2, q.width * SIDE_LEN, q.height * SIDE_LEN).contains(QPoint((mario.r.x() + 0.5) * SIDE_LEN, (mario.r.y() + 2 - ptrStatus->height) * SIDE_LEN), FALSE)
		|| QRect(q.r.x() * SIDE_LEN, q.r.y() * SIDE_LEN + 2, q.width * SIDE_LEN, q.height * SIDE_LEN).contains(QPoint((mario.r.x()) * SIDE_LEN, (mario.r.y() + 2 - ptrStatus->height) * SIDE_LEN), FALSE)
		|| QRect(q.r.x() * SIDE_LEN, q.r.y() * SIDE_LEN + 2, q.width * SIDE_LEN, q.height * SIDE_LEN).contains(QPoint((mario.r.x() + 1) * SIDE_LEN, (mario.r.y() + 2 - ptrStatus->height) * SIDE_LEN), FALSE)) {
		q.die_contral = 1;
		mario.r.ry() = int(mario.r.ry()) + 0.1;
		mario.v.ry() = 0;
	}
}

void judge_coin(Coin &c)
{
	if (QRect(c.r.x() * SIDE_LEN, c.r.y() * SIDE_LEN, c.width * SIDE_LEN, c.height * SIDE_LEN).intersects(QRect(mario.r.x() * SIDE_LEN, (mario.r.y() + 2 - ptrStatus->height) * SIDE_LEN, SIDE_LEN, ptrStatus->height * SIDE_LEN))){
		if (c.die_contral == 0) {
			my_coin++;
			my_score += 100;
		}
		c.die_contral = 1;
	}
}

void judge_mushroom(Mushroom &m) 
{
	if (m.die_contral == 0) {
		if (QRect(m.r.x() * SIDE_LEN, m.r.y() * SIDE_LEN, m.width * SIDE_LEN, m.height * SIDE_LEN).intersects(QRect(mario.r.x() * SIDE_LEN, (mario.r.y() + 2 - ptrStatus->height) * SIDE_LEN - 1, SIDE_LEN, ptrStatus->height * SIDE_LEN))) {
			m.die_contral = 1;
			if (ptrStatus == &status_short) {
				ptrStatus = &status_middle;
				left_lives++;
			}

			else {
				ptrStatus = &status_super;
				left_lives++;
			}

		}
	}
}

void judge_flower(Flower &flower1)
{
	if (flower1.die_contral == 0) {
		if (QRect(flower1.r.x() * SIDE_LEN, flower1.r.y() * SIDE_LEN, flower1.width * SIDE_LEN, flower1.height * SIDE_LEN).intersects(QRect(mario.r.x() * SIDE_LEN, (mario.r.y() + 2 - ptrStatus->height) * SIDE_LEN, SIDE_LEN, ptrStatus->height * SIDE_LEN))) {
			flower1.die_contral = 1;
			if (ptrStatus == &status_short) {
				ptrStatus = &status_middle;
				left_lives++;
			}

			else {
				ptrStatus = &status_super;
				left_lives++;
			}

		}
	}
}

//倒计时控制
void SuperMario::timerEvent(QTimerEvent * ev)
{
	if (ev->timerId() == id1) {
		static int sec = MAX_TIME;
		ui.TIME_label->setText(QString("<center><h1>time left</h1></center>\n<center><h1>%1</h1></center>").arg(sec--));
		if (0 > sec)
		{
			this->killTimer(this->id1);
			mario.die_contral = 1;	//时间到了,游戏结束
			left_lives--;
		}
		my_score += fabs(mario.r.x() - mario.last_r.x()) * 0.8 + (400 - sec) * 0.0001;	//计算分数的函数
	}
	else if (ev->timerId() == id2) {
		//检测是否通关
		if (mario.r.x() >= 187)
			you_win();

		QString text_lives = QString("<center><h1>MARIO × %1</h1></center><center><h1>   %2</h1></center>").arg(left_lives).arg(my_score);
		ui.MARIO_label->setText(text_lives);

		QString text_coins = QString("<center><h1>COINS</h1></center><center><h1>    ×  %1</h1></center>").arg(my_coin);
		ui.COIN_label->setText(text_coins);

		static int cloud_time_contral = 0;//获取当前时间，用函数控制云的运动
		if (cloud_time_contral == 360)
			cloud_time_contral = 0;
		else
			cloud_time_contral++;

		mario.move(0);
		for (int i = 0; i < 18; i++)
			cloud[i].move(cloud_time_contral + cloud[i].r.x() * 0.05 - cloud[i].r.y());

		for (int i = 0; i < 8; i++) {
			if (turtle[i].r.x() - mario.r.x() <= DISTANCE)
				turtle[i].move(0);
		}
			

		flower1.move(0);

		question[0].move(0);
		question[1].move(0);

		//判断问号
		for (int i = 0; i < 14; i++)
			judge_questionmark(question[i]);

		if (question[0].die_contral == 1 && mushroom1.die_contral == 0)
			mushroom1.move(0);
		if (question[1].die_contral == 1 && mushroom2.die_contral == 0)
			mushroom2.move(0);

		judge_flower(flower1);
		judge_mushroom(mushroom1);
		judge_mushroom(mushroom2);

		//判断金币
		for (int i = 0; i < 9; i++) {
			judge_coin(coins[i]);
		}


		for (int i = 2; i < 14; i++) {
			question[i].move(0);
			if (question[i].die_contral == 1 && hidecoin[i - 2].die_contral == 0) {
				my_score += 200;
				hidecoin[i - 2].move(0);
			}
			else if (hidecoin[i - 2].die_contral == 1 && hidecoin[i - 2].has_add == 0) {
				hidecoin[i - 2].has_add = 1;
				my_coin++;
			}
		}
			
		for (int i = 0; i < 9; i++)
			coins[i].move(0);

		//死亡检测(优先级最高)
		if (mario.die_contral == 2)
			game_over();
		if (mario.start_time == 1)	//如果进入无敌状态
		{
			mario.invin_time++;
			if (mario.invin_time >= INVINCIBLE_TIME) {
				mario.invin_time = 0;
				mario.start_time = 0;
			}
		}
		else {
			for (int i = 0; i < 8; i++) {
				if (mario.die_contral == 0 && turtle[i].die_contral == 0) {//马里奥没死才做碰撞判定
					if (QRect(mario.r.x() * SIDE_LEN, (mario.r.y() + 2 - ptrStatus->height) * SIDE_LEN, SIDE_LEN, ptrStatus->height * SIDE_LEN).intersects(QRect(turtle[i].r.x() * SIDE_LEN, turtle[i].r.y() * SIDE_LEN, turtle[i].width * SIDE_LEN, turtle[i].height * SIDE_LEN))) {
						if (mario.last_r.y() + 2 <= turtle[i].last_r.y()) {	//如果上一帧马里奥的脚在怪物头的上方,怪物死，否则马里奥死
							turtle[i].die_contral = 1;
							turtle[i].v.rx() = 0;
							turtle[i].v.ry() = DIE_SPEED;
						}
						else {
							left_lives--;
							if (ptrStatus == &status_short) {
								mario.die_contral = 1;
								mario.v.ry() = DIE_SPEED;
							}
							else if (ptrStatus == &status_middle) {
								ptrStatus = &status_short;
								mario.start_time = 1;
							}
							else if (ptrStatus == &status_super) {
								ptrStatus = &status_middle;
								mario.start_time = 1;
							}

						}
					}
				}
				else if (turtle[i].die_contral == 1) {//乌龟被踩死
					if (QRect(mario.r.x() * SIDE_LEN, (mario.r.y() + 2 - ptrStatus->height) * SIDE_LEN, SIDE_LEN, ptrStatus->height * SIDE_LEN).intersects(QRect(turtle[i].r.x() * SIDE_LEN, turtle[i].r.y() * SIDE_LEN, turtle[i].width * SIDE_LEN, turtle[i].height * SIDE_LEN))) {
						if (fabs(turtle[i].v.rx()) > MINIMUM_VALUE
							&& (turtle[i].monster_pattern_contral == 0 || turtle[i].monster_pattern_contral == 1)
							&& turtle[i].is_kicked == 1) {
							if (ptrStatus == &status_short) {
								mario.die_contral = 1;
								left_lives--;
								mario.v.ry() = DIE_SPEED;
							}
							else if (ptrStatus == &status_middle) {
								left_lives--;
								ptrStatus = &status_short;
								mario.start_time = 1;
							}
							else if (ptrStatus == &status_super) {
								left_lives--;
								ptrStatus = &status_middle;
								mario.start_time = 1;
							}
							turtle[i].is_kicked = 0;
						}

						if (fabs(mario.last_r.y() + 2 - turtle[i].last_r.y() - 1) <= MINIMUM_VALUE) {
							if (mario.v.x() < 0)
								turtle[i].v.rx() = -6;
							else
								turtle[i].v.rx() = 6;
						}

						else
							mario.r.ry() = turtle[i].r.y() - 2;
						mario.fly_contral = 0;
						mario.dont_move = 0;
					}
				}
			}
		}

	}
	update();
}