#pragma once
#include "Spirit.h"
class QuestionMark :
	public Spirit
{
public:
	QuestionMark(int i, int j)		//初始位置
	{
		r.rx() = i;
		r.ry() = j;
		a = QPointF(0, 0);
		v = QPointF(0, 0);
		frame_contral = 0;
		die_contral = 0;	//默认是活着
		width = 1;
		height = 1;
		move_contral = 0;
	}
	void paint(QPainter & painter) override;
	void move(int a) override;
	int width, height, move_contral;
	~QuestionMark();
};