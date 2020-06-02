#pragma once

#include<cstdint>

enum class SnakeAction : uint8_t
{
	None = 0,
	BeforeTurn = 1,
	AfterTurn = 2
};