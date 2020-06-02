#pragma once

#include "BoardElement.h"
#include "BoardPoint.h"
#include <list>

class GameBoard
{
public:
	GameBoard(BoardElement** map, int map_size);
	bool hasElementAt(BoardPoint point, BoardElement element);
	BoardElement getElementAt(BoardPoint point);
	void printBoard();
	std::list<BoardPoint> findAllElements(BoardElement element);
	std::list<BoardPoint> findAllElements();
	BoardPoint getMyHead();
	std::list<BoardPoint> getWalls();
	bool isBarrierAt(BoardPoint point);
	std::list<BoardPoint> getStones();
	std::list<BoardPoint> getApples();
	bool amIEvil();
	bool amIFlying();
	std::list<BoardPoint> getFlyingPills();
	std::list<BoardPoint> getFuryPills();
	std::list<BoardPoint> getGold();
	std::list<BoardPoint> getStartPoints();
	std::list<BoardPoint> getBarriers();
	std::list<BoardPoint> getEnemyHead();
	~GameBoard();

	BoardElement** map;
	const int map_size;

private:
	
};