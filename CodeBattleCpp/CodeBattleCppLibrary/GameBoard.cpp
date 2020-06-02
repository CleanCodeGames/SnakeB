#include "GameBoard.h"
#include <iostream>
#include <string>

GameBoard::GameBoard(BoardElement** map, int map_size) : map_size(map_size)
{
	this->map = map;
}


bool GameBoard::hasElementAt(BoardPoint point, BoardElement element) {
	if (point.isOutOfBoard(map_size)) {
		return false;
	}
	return getElementAt(point) == element;
}
BoardElement GameBoard::getElementAt(BoardPoint point) {
	return map[point.getX()][point.getY()];
}

std::list<BoardPoint> GameBoard::findAllElements(BoardElement element) {
	std::list<BoardPoint> result;
	for (uint32_t j = 0; j < map_size; j++) {
		for (uint32_t i = 0; i < map_size; i++) {
			if (map[j][i] == element) {
				result.push_back(BoardPoint(j, i));
			}
		}
	}
	return result;
}
std::list<BoardPoint> GameBoard::findAllElements() {
	std::list<BoardPoint> result;
	for (uint32_t j = 0; j < map_size; j++) {
		for (uint32_t i = 0; i < map_size; i++) {
			result.push_back(BoardPoint(j, i));
		}
	}
	return result;
}

void GameBoard::printBoard() {
	
}

std::list<BoardPoint> GameBoard::getEnemyHead()
{
	std::list<BoardPoint> result = findAllElements(BoardElement::ENEMY_HEAD_DOWN);
	result.splice(result.end(), findAllElements(BoardElement::ENEMY_HEAD_UP));
	result.splice(result.end(), findAllElements(BoardElement::ENEMY_HEAD_LEFT));
	result.splice(result.end(), findAllElements(BoardElement::ENEMY_HEAD_RIGHT));
	result.splice(result.end(), findAllElements(BoardElement::ENEMY_HEAD_EVIL));
	return result;
}

BoardPoint GameBoard::getMyHead() {
	static BoardPoint bp;
	std::list<BoardPoint> result = findAllElements(BoardElement::HEAD_DEAD);
	result.splice(result.end(), findAllElements(BoardElement::HEAD_DOWN));
	result.splice(result.end(), findAllElements(BoardElement::HEAD_UP));
	result.splice(result.end(), findAllElements(BoardElement::HEAD_LEFT));
	result.splice(result.end(), findAllElements(BoardElement::HEAD_RIGHT));
	result.splice(result.end(), findAllElements(BoardElement::HEAD_EVIL));
	result.splice(result.end(), findAllElements(BoardElement::HEAD_FLY));
	result.splice(result.end(), findAllElements(BoardElement::HEAD_SLEEP));
	if (!result.empty()) { bp = result.front(); return result.front(); }
	else return bp;
}
std::list<BoardPoint> GameBoard::getWalls() {
	return findAllElements(BoardElement::WALL);
}
bool GameBoard::isBarrierAt(BoardPoint point) {
	std::list<BoardPoint> result = getBarriers();
	BoardPoint resultPoint = result.front();
	return resultPoint == point;
}
std::list<BoardPoint> GameBoard::getStones() {
	return findAllElements(BoardElement::STONE);
}
std::list<BoardPoint> GameBoard::getApples() {
	std::list<BoardPoint> result = findAllElements(BoardElement::APPLE);
	result.splice(result.end(), findAllElements(BoardElement::GOLD));
	result.splice(result.end(), findAllElements(BoardElement::FURY_PILL));
	if (amIEvil())
	{
		result.splice(result.end(), findAllElements(BoardElement::STONE));
		result.splice(result.end(), findAllElements(BoardElement::ENEMY_BODY_HORIZONTAL));
		result.splice(result.end(), findAllElements(BoardElement::ENEMY_BODY_LEFT_DOWN));
		result.splice(result.end(), findAllElements(BoardElement::ENEMY_BODY_LEFT_UP));
		result.splice(result.end(), findAllElements(BoardElement::ENEMY_BODY_RIGHT_DOWN));
		result.splice(result.end(), findAllElements(BoardElement::ENEMY_BODY_RIGHT_UP));
		result.splice(result.end(), findAllElements(BoardElement::ENEMY_BODY_VERTICAL));
		result.splice(result.end(), findAllElements(BoardElement::ENEMY_HEAD_DOWN));
		result.splice(result.end(), findAllElements(BoardElement::ENEMY_HEAD_LEFT));
		result.splice(result.end(), findAllElements(BoardElement::ENEMY_HEAD_RIGHT));
		result.splice(result.end(), findAllElements(BoardElement::ENEMY_HEAD_UP));
		result.splice(result.end(), findAllElements(BoardElement::ENEMY_HEAD_LEFT));
		result.splice(result.end(), findAllElements(BoardElement::ENEMY_HEAD_DEAD));
	}
	return result;
}
bool GameBoard::amIEvil() {
	std::list<BoardPoint> headEvil = findAllElements(BoardElement::HEAD_EVIL);
	if (!headEvil.empty()) {
		return true;
	}
	return false;
}
bool GameBoard::amIFlying() {
	std::list<BoardPoint> headFly = findAllElements(BoardElement::HEAD_FLY);
	if (!headFly.empty()) {
		return true;
	}
	return false;
}
std::list<BoardPoint> GameBoard::getFlyingPills() {
	return findAllElements(BoardElement::FLYING_PILL);
}
std::list<BoardPoint> GameBoard::getFuryPills() {
	return findAllElements(BoardElement::FURY_PILL);
}
std::list<BoardPoint> GameBoard::getGold() {
	return findAllElements(BoardElement::GOLD);
}
std::list<BoardPoint> GameBoard::getStartPoints() {
	return findAllElements(BoardElement::START_FLOOR);
}
std::list<BoardPoint> GameBoard::getBarriers() 
{
	std::list<BoardPoint> result = findAllElements(BoardElement::WALL);
	result.splice(result.end(), findAllElements(BoardElement::START_FLOOR));
	result.splice(result.end(), findAllElements(BoardElement::ENEMY_HEAD_SLEEP));
	result.splice(result.end(), findAllElements(BoardElement::ENEMY_HEAD_EVIL));
	result.splice(result.end(), findAllElements(BoardElement::TAIL_INACTIVE));
	result.splice(result.end(), findAllElements(BoardElement::BODY_HORIZONTAL));
	result.splice(result.end(), findAllElements(BoardElement::BODY_LEFT_DOWN));
	result.splice(result.end(), findAllElements(BoardElement::BODY_LEFT_UP));
	result.splice(result.end(), findAllElements(BoardElement::BODY_RIGHT_DOWN));
	result.splice(result.end(), findAllElements(BoardElement::BODY_RIGHT_UP));
	result.splice(result.end(), findAllElements(BoardElement::BODY_VERTICAL));
	result.splice(result.end(), findAllElements(BoardElement::TAIL_END_DOWN));
	result.splice(result.end(), findAllElements(BoardElement::TAIL_END_LEFT));
	result.splice(result.end(), findAllElements(BoardElement::TAIL_END_RIGHT));
	result.splice(result.end(), findAllElements(BoardElement::TAIL_END_UP));
	if (!amIEvil())
	{
		result.splice(result.end(), findAllElements(BoardElement::STONE));
		result.splice(result.end(), findAllElements(BoardElement::ENEMY_BODY_HORIZONTAL));
		result.splice(result.end(), findAllElements(BoardElement::ENEMY_BODY_LEFT_DOWN));
		result.splice(result.end(), findAllElements(BoardElement::ENEMY_BODY_LEFT_UP));
		result.splice(result.end(), findAllElements(BoardElement::ENEMY_BODY_RIGHT_DOWN));
		result.splice(result.end(), findAllElements(BoardElement::ENEMY_BODY_RIGHT_UP));
		result.splice(result.end(), findAllElements(BoardElement::ENEMY_BODY_VERTICAL));
		result.splice(result.end(), findAllElements(BoardElement::ENEMY_HEAD_DOWN));
		result.splice(result.end(), findAllElements(BoardElement::ENEMY_HEAD_LEFT));
		result.splice(result.end(), findAllElements(BoardElement::ENEMY_HEAD_RIGHT));
		result.splice(result.end(), findAllElements(BoardElement::ENEMY_HEAD_UP));
		result.splice(result.end(), findAllElements(BoardElement::ENEMY_HEAD_LEFT));
		result.splice(result.end(), findAllElements(BoardElement::ENEMY_HEAD_DEAD));
		result.splice(result.end(), findAllElements(BoardElement::ENEMY_TAIL_END_DOWN));
		result.splice(result.end(), findAllElements(BoardElement::ENEMY_TAIL_END_LEFT));
		result.splice(result.end(), findAllElements(BoardElement::ENEMY_TAIL_END_RIGHT));
		result.splice(result.end(), findAllElements(BoardElement::ENEMY_TAIL_END_UP));
		result.splice(result.end(), findAllElements(BoardElement::ENEMY_TAIL_INACTIVE));
	}
	return result;
}

GameBoard::~GameBoard()
{
	delete map;
}

