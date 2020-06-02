#include <iostream>
#include <random>

#include "GameClientSnake.h"

using namespace std;

const bool IsHeadUp(GameClientSnake& gcb)		{ return !gcb.get_GameBoard()->findAllElements(BoardElement::HEAD_UP).empty(); }
const bool IsHeadRight(GameClientSnake& gcb)	{ return !gcb.get_GameBoard()->findAllElements(BoardElement::HEAD_RIGHT).empty(); }
const bool IsHeadDown(GameClientSnake& gcb)		{ return !gcb.get_GameBoard()->findAllElements(BoardElement::HEAD_DOWN).empty(); }
const bool IsHeadLeft(GameClientSnake& gcb)		{ return !gcb.get_GameBoard()->findAllElements(BoardElement::HEAD_LEFT).empty(); }

float GetDistance(BoardPoint start_p, BoardPoint end_p)
{
	return sqrt(pow(start_p.getX() - end_p.getX(), 2) + pow(start_p.getY() - end_p.getY(), 2));
}

enum class DIR { UP, RIGHT, DOWN, LEFT };

bool IsBarierRight(GameBoard& gb, int distance = 1) {
	bool isDanger = false;
	BoardPoint& head = gb.getMyHead();
	for (auto& barier : gb.getBarriers()) {
		if(head.getY() == barier.getY())
			for (int i = distance; i > 0; i--) {
				if (((head.getX() + i) == barier.getX()))
					isDanger = true;
			}
	}
	return isDanger;
}
bool IsBarierLeft(GameBoard& gb, int distance = 1) {
	bool isDanger = false;
	BoardPoint& head = gb.getMyHead();
	for (auto& barier : gb.getBarriers()) {
		if (head.getY() == barier.getY())
			for (int i = distance; i > 0; i--) {
				if ((head.getX() - i) == barier.getX())
					isDanger = true;
			}
	}
	return isDanger;
}
bool IsBarierUp(GameBoard& gb, int distance = 1) {
	bool isDanger = false;
	BoardPoint& head = gb.getMyHead();
	for (auto& barier : gb.getBarriers()) {
		if (head.getX() == barier.getX())
			for (int i = distance; i > 0; i--) {
				if ((head.getY() - i) == barier.getY())
					isDanger = true;
			}
	}
	return isDanger;
}
bool IsBarierDown(GameBoard& gb, int distance = 1) {
	bool isDanger = false;
	BoardPoint& head = gb.getMyHead();
	for (auto& barier : gb.getBarriers()) {
		if (head.getX() == barier.getX())
			for (int i = distance; i > 0; i--) {
				if ((head.getY() + i) == barier.getY())
					isDanger = true;
			}
	}
	return isDanger;
}

bool IsBarierDownLeft(GameBoard& gb, int distance = 1) {
	bool isDanger = false;
	BoardPoint& head = gb.getMyHead();
	for (auto& barier : gb.getBarriers()) {
		if (head.getX() == barier.getX() + 1)
			for (int i = distance; i > 0; i--) {
				if ((head.getY() + i) == barier.getY())
					isDanger = true;
			}
	}
	return isDanger;
}
bool IsBarierDownRight(GameBoard& gb, int distance = 1) {
	bool isDanger = false;
	BoardPoint& head = gb.getMyHead();
	for (auto& barier : gb.getBarriers()) {
		if (head.getX() == barier.getX() - 1)
			for (int i = distance; i > 0; i--) {
				if ((head.getY() + i) == barier.getY())
					isDanger = true;
			}
	}
	return isDanger;
}
bool IsBarierUpRight(GameBoard& gb, int distance = 1) {
	bool isDanger = false;
	BoardPoint& head = gb.getMyHead();
	for (auto& barier : gb.getBarriers()) {
		if (head.getY() == barier.getY() + 1)
			for (int i = distance; i > 0; i--) {
				if (((head.getX() + i) == barier.getX()))
					isDanger = true;
			}
	}
	return isDanger;
}
bool IsBarierUpLeft(GameBoard& gb, int distance = 1) {
	bool isDanger = false;
	BoardPoint& head = gb.getMyHead();
	for (auto& barier : gb.getBarriers()) {
		if (head.getY() == barier.getY() + 1)
			for (int i = distance; i > 0; i--) {
				if (((head.getX() - i) == barier.getX()))
					isDanger = true;
			}
	}
	return isDanger;
}

bool IsAppleBlocked(GameBoard& gb, BoardPoint& apple)
{
	int is_blocked = 0;
	for (auto& barier : gb.getBarriers())
	{
		if (gb.getMyHead() == barier) continue;
		else
		{
			if (apple.getY() == barier.getY()) {
				if (barier.getX() == (apple.getX() - 1)) is_blocked++;
				if (barier.getX() == (apple.getX() + 1)) is_blocked++;
			}
			if (apple.getX() == barier.getX()) {
				if (barier.getY() == (apple.getY() - 1)) is_blocked++;
				if (barier.getY() == (apple.getY() + 1)) is_blocked++;

			}
		}
	}
	//cout << is_blocked << endl;
	return is_blocked > 2;
}

void main()
{
	srand(time(0));
	
	GameClientSnake gcb = GameClientSnake("http://codebattle-pro-2020s1.westeurope.cloudapp.azure.com/codenjoy-contest/board/player/6de13mjoib9z0ajmlw55?code=8503450151131199096&gameName=snakebattle");
	static DIR dir = DIR::RIGHT;
	static int evil_steps = 0;

	gcb.Run([&]()
	{
		GameBoard& gb = *gcb.get_GameBoard();
		bool done = false;

		// ѕозици€ ближайшего €блока
		BoardPoint end_point(999, 999);

		// »щем ближайшее €блоко
		for (auto& apple : gb.getApples()) {
			if (!IsAppleBlocked(gb, apple)) {
				float my_dist = GetDistance(gb.getMyHead(), apple);
				if (my_dist < GetDistance(gb.getMyHead(), end_point)) {
					if (!gb.amIEvil()) {
						for (auto& enemy_head : gb.getEnemyHead()) {
							if (my_dist < GetDistance(enemy_head, apple))
								end_point = BoardPoint(apple.getY(), apple.getX());
						}
					}
					else end_point = BoardPoint(apple.getY(), apple.getX());
				}
			}
		}

		if (gb.amIEvil()) {
			if (dir == DIR::UP || dir == DIR::DOWN) {
				if (gb.getMyHead().getX() > end_point.getX()) {
					if (!IsBarierLeft(gb)) {
						gcb.Left();
						dir = DIR::LEFT;
					}
				}
				else if (gb.getMyHead().getX() < end_point.getX()) {
					if (!IsBarierRight(gb)) {
						gcb.Right();
						dir = DIR::RIGHT;
					}
				}
				else if ((dir == DIR::UP && gb.getMyHead().getY() < end_point.getY()) 
					|| (dir == DIR::DOWN && gb.getMyHead().getY() > end_point.getY())) {
					if (!IsBarierLeft(gb)) {
						gcb.Left();
						dir = DIR::LEFT;
					}
					else if (!IsBarierRight(gb)) {
						gcb.Right();
						dir = DIR::RIGHT;
					}
				}
			}
			else if (dir == DIR::LEFT || dir == DIR::RIGHT) {
				if (gb.getMyHead().getY() > end_point.getY()) {
					if (!IsBarierUp(gb)) {
						gcb.Up();
						dir = DIR::UP;
					}
				}
				else if (gb.getMyHead().getY() < end_point.getY()) {
					if (!IsBarierDown(gb)) {
						gcb.Down();
						dir = DIR::DOWN;
					}
				}
				else if ((dir == DIR::RIGHT && gb.getMyHead().getX() > end_point.getX()) 
					|| (dir == DIR::LEFT && gb.getMyHead().getX() < end_point.getX())) {
					if (!IsBarierDown(gb)) {
						gcb.Down();
						dir = DIR::DOWN;
					}
					else if (!IsBarierUp(gb)) {
						gcb.Up();
						dir = DIR::UP;
					}
				}
			}
		}
		else {
			if (IsHeadDown(gcb) || IsHeadUp(gcb)) {
				if (gb.getMyHead().getX() > end_point.getX()) {
					if (!IsBarierLeft(gb)) {
						gcb.Left();
						dir = DIR::LEFT;
					}
				}
				else if (gb.getMyHead().getX() < end_point.getX()) {
					if (!IsBarierRight(gb)) {
						gcb.Right();
						dir = DIR::RIGHT;
					}
				}
				else if ((dir == DIR::UP && gb.getMyHead().getY() < end_point.getY()) 
					|| (dir == DIR::DOWN && gb.getMyHead().getY() > end_point.getY())) {
					if (!IsBarierLeft(gb)) {
						gcb.Left();
						dir = DIR::LEFT;
					}
					else if (!IsBarierRight(gb)) {
						gcb.Right();
						dir = DIR::RIGHT;
					}
				}
			}
			else if(IsHeadLeft(gcb) || IsHeadRight(gcb)) {
				if (gb.getMyHead().getY() > end_point.getY()) {
					if (!IsBarierUp(gb)) {
						gcb.Up();
						dir = DIR::UP;
					}
				}
				else if (gb.getMyHead().getY() < end_point.getY()) {
					if (!IsBarierDown(gb)) {
						gcb.Down();
						dir = DIR::DOWN;
					}
				}
				else if ((dir == DIR::RIGHT && gb.getMyHead().getX() > end_point.getX()) 
					|| dir == DIR::LEFT && gb.getMyHead().getX() < end_point.getX()) {
					if (!IsBarierDown(gb)) {
						gcb.Down();
						dir = DIR::DOWN;
					}
					else if (!IsBarierUp(gb)) {
						gcb.Up();
						dir = DIR::UP;
					}
				}
			}
		}

		if (!done) {
			gcb.Blank();
		}
	});

	getchar();
}
