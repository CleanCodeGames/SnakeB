#include <iostream>
#include <random>

#include "GameClientSnake.h"

int map[32][32];

using namespace std;
enum class DIR { UP, RIGHT, DOWN, LEFT };
const bool IsHeadUp(GameClientSnake& gcb) { 
	return !gcb.get_GameBoard()->findAllElements(BoardElement::HEAD_UP).empty(); 
}
const bool IsHeadRight(GameClientSnake& gcb) { 
	return !gcb.get_GameBoard()->findAllElements(BoardElement::HEAD_RIGHT).empty(); 
}
const bool IsHeadDown(GameClientSnake& gcb)	{ 
	return !gcb.get_GameBoard()->findAllElements(BoardElement::HEAD_DOWN).empty(); 
}
const bool IsHeadLeft(GameClientSnake& gcb)	{ 
	return !gcb.get_GameBoard()->findAllElements(BoardElement::HEAD_LEFT).empty(); 
}
const float GetDistance(BoardPoint& start_p, BoardPoint& end_p)
{
	return sqrt(pow(start_p.getX() - end_p.getX(), 2) + pow(start_p.getY() - end_p.getY(), 2));
}
const bool IsBarierRight(GameBoard& gb, int distance = 1) {
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
const bool IsBarierLeft(GameBoard& gb, int distance = 1) {
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
const bool IsBarierUp(GameBoard& gb, int distance = 1) {
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
const bool IsBarierDown(GameBoard& gb, int distance = 1) {
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
const bool IsBarierDownLeft(GameBoard& gb, int distance = 1) {
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
const bool IsBarierDownRight(GameBoard& gb, int distance = 1) {
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
const bool IsBarierUpRight(GameBoard& gb, int distance = 1) {
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
const bool IsBarierUpLeft(GameBoard& gb, int distance = 1) {
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
const bool IsAppleBlocked(GameBoard& gb, BoardPoint& apple)
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
	return is_blocked > 2;
}
BoardPoint CheckNearApple(GameBoard& gb)
{
	BoardPoint end_point(9999, 9999);
	for (auto& apple : gb.getApples())
	{
		if (!IsAppleBlocked(gb, apple)) {
			float my_dist = GetDistance(gb.getMyHead(), apple);
			if (my_dist <= GetDistance(gb.getMyHead(), end_point)) {
				end_point = BoardPoint(apple.getY(), apple.getX());
			}
		}
	}
	return end_point;
}

struct Cell
{
	BoardPoint p;
	BoardElement e = BoardElement::NONE;
	Cell(BoardPoint p, BoardElement e) : p(p), e(e) { }
	void Draw()
	{
		if (e == BoardElement::NONE) cout << "-";
		else if (e == BoardElement::APPLE) cout << "O";
		else if (e == BoardElement::WALL) cout << "#";
		else cout << "$";
	}
	Cell() {}
	~Cell(){}
};

struct Step
{
	BoardPoint p;
	Step(BoardPoint p) : p(p) {}
	~Step() {}
};

Step SearchWay(BoardPoint apple_position, BoardPoint head_position)
{
	int matrix[32][32][3];
	int step;
	bool added = true, result = true;
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 32; j++) {
			if (map[i][j] != 0) matrix[i][j][0] = -2;
			else matrix[i][j][0] = -1;
		}
	}
	matrix[apple_position.getY()][apple_position.getX()][0] = 0;

	step = 0;
	while (added && matrix[head_position.getY()][head_position.getX()][0] == -1) {
		added = false;
		step++;
		for (int i = 0; i < 32; i++) {
			for (int j = 0; j < 32; j++) {
				if (matrix[i][j][0] == step - 1) {
					int _i, _j;
					_i = i + 1; _j = j;
					if (_i >= 0 && _j >= 0 && _i < 32 && _j < 32) {
						if (matrix[_i][_j][0] == -1 && matrix[_i][_j][0] != -2) {
							matrix[_i][_j][0] = step;
							matrix[_i][_j][1] = i;
							matrix[_i][_j][2] = j;
							added = true;
						}
					}
					_i = i - 1; _j = j;
					if (_i >= 0 && _j >= 0 && _i < 32 && _j < 32) {
						if (matrix[_i][_j][0] == -1 && matrix[_i][_j][0] != -2) {
							matrix[_i][_j][0] = step;
							matrix[_i][_j][1] = i;
							matrix[_i][_j][2] = j;
							added = true;
						}
					}
					_i = i; _j = j + 1;
					if (_i >= 0 && _j >= 0 && _i < 32 && _j < 32) {
						if (matrix[_i][_j][0] == -1 && matrix[_i][_j][0] != -2) {
							matrix[_i][_j][0] = step;
							matrix[_i][_j][1] = i;
							matrix[_i][_j][2] = j;
							added = true;
						}
					}
					_i = i; _j = j - 1;
					if (_i >= 0 && _j >= 0 && _i < 32 && _j < 32) {
						if (matrix[_i][_j][0] == -1 && matrix[_i][_j][0] != -2) {
							matrix[_i][_j][0] = step;
							matrix[_i][_j][1] = i;
							matrix[_i][_j][2] = j;
							added = true;
						}
					}
				}
			}
		}
	}
	if (matrix[head_position.getY()][head_position.getX()][0] == -1) {
		result = false;
	}
	if (result) {
		int _i = head_position.getY(), _j = head_position.getX();
		int count = 0;
		while (matrix[_i][_j][0] != 0) {
			if (count != 0) return Step(BoardPoint(_i, _j));
			int li = matrix[_i][_j][1];
			int lj = matrix[_i][_j][2];
			_i = li; _j = lj;
			count++;
		}
	}
	return Step(BoardPoint());
}

void main()
{
	srand(time(0));
	GameClientSnake* gcb = new GameClientSnake("http://codebattle-pro-2020s1.westeurope.cloudapp.azure.com/codenjoy-contest/board/player/wnn4elsjhpzz9tk95fk1?code=3113067072158635833&gameName=snakebattle");
	static DIR dir = DIR::RIGHT;

	gcb->Run([&]()
	{
		GameBoard& gb = *gcb->get_GameBoard();
		bool done = false;
		BoardPoint apple_position = CheckNearApple(gb);
		const int apple_X = apple_position.getX();
		const int apple_Y = apple_position.getY();
		BoardPoint head_position = gb.getMyHead();
		const int head_X = head_position.getX();
		const int head_Y = head_position.getY();
		int left = 0;
		int right = 0;
		int up = 0;
		int down = 0;
		auto IsAppleLeft([&]() { return apple_X < head_X; });
		auto IsAppleRight([&]() { return apple_X > head_X; });
		auto IsAppleUp([&]() { return apple_Y < head_Y; });
		auto IsAppleDown([&]() { return apple_Y > head_Y; });

		if(!gb.findAllElements(BoardElement::HEAD_DEAD).empty()) 
			dir = DIR::RIGHT;

		enum class CELL_TYPE { BARIER, APPLE, NONE, MY_HEAD } cell_type = CELL_TYPE::NONE;
		Cell cell[32][32];
		list<Step> vec_Step;

		for (int j = 0; j < gb.map_size; j++) {
			for (int i = 0; i < gb.map_size; i++) {
				cell_type = CELL_TYPE::NONE;
				for(auto barier : gb.getBarriers()) if(BoardPoint(barier.getY(), barier.getX()) == BoardPoint(j, i)) cell_type = CELL_TYPE::BARIER;
				for (auto apple : gb.getApples()) if (BoardPoint(apple.getY(), apple.getX()) == BoardPoint(j, i)) cell_type = CELL_TYPE::APPLE;
				if (BoardPoint(gb.getMyHead().getY(), gb.getMyHead().getX()) == BoardPoint(j, i)) cell_type = CELL_TYPE::MY_HEAD;

				if (cell_type == CELL_TYPE::NONE) { cell[j][i] = Cell(BoardPoint(j, i), BoardElement::NONE); map[j][i] = 0; }
				else if (cell_type == CELL_TYPE::APPLE) { cell[j][i] = Cell(BoardPoint(j, i), BoardElement::APPLE); map[j][i] = 0; }
				else if (cell_type == CELL_TYPE::BARIER) { cell[j][i] = Cell(BoardPoint(j, i), BoardElement::WALL); map[j][i] = 1; }
				else if (cell_type == CELL_TYPE::MY_HEAD) { 
					BoardElement el = gb.getElementAt(gb.getMyHead());
					cell[j][i] = Cell(BoardPoint(j, i), el);
					map[j][i] = 0;
				}
			}
		}
		
		BoardPoint s = SearchWay(apple_position, head_position).p;
		BoardPoint h = head_position;

		cout << "HEAD: " << h.getX() << " : " << h.getY() << endl;
		cout << "STEP: " << s.getX() << " : " << s.getY() << endl;
		cout << "APPL: " << apple_position.getX() << " : " << apple_position.getY() << endl;

		if (dir == DIR::RIGHT || dir == DIR::LEFT) 
		{
			if (h.getX() == s.getX()) {
				if (h.getY() > s.getY()) { gcb->Up(); dir = DIR::UP; }
				else if (h.getY() < s.getY()) { gcb->Down(); dir = DIR::DOWN; }
			}
		}
		else if (dir == DIR::DOWN || dir == DIR::UP)
		{
			if (h.getY() == s.getY()) {
				if (h.getX() > s.getX()) { gcb->Left(); dir = DIR::LEFT; }
				else if (h.getX() < s.getX()) { gcb->Right(); dir = DIR::RIGHT; }
			}
		}
		gcb->Act();
		done = true;
		vec_Step.clear();
		if (!done) {
			gcb->Blank();
		}
	});

	getchar();
}