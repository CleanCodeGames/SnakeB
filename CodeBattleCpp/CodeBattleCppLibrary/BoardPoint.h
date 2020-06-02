#pragma once

class BoardPoint
{
public:
	BoardPoint(int x, int y);
	BoardPoint()
	{
		x = 0; y = 0;
	}
	~BoardPoint();

	BoardPoint shiftLeft(int delta);
	BoardPoint shiftLeft();
	BoardPoint shiftRight(int delta);
	BoardPoint shiftRight();
	BoardPoint shiftBottom(int delta);
	BoardPoint shiftBottom();
	BoardPoint shiftTop(int delta);
	BoardPoint shiftTop();
	friend bool operator==(const BoardPoint& leftPoint, const BoardPoint& rightPoint);
	int getX();
	int getY();
	bool isOutOfBoard(int size);
	void print();
private:
	int x;
	int y;
};