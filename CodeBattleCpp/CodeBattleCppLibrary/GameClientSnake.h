#pragma once

#include <string>
#include <thread>
#include "easywsclient\easywsclient.hpp"
#ifdef _WIN32
#pragma comment( lib, "ws2_32" )
#include <WinSock2.h>
#endif
#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <memory>
#include <functional>

#include "BoardElement.h"
#include "SnakeAction.h"
#include "GameBoard.h"

class GameClientSnake
{
	BoardElement **map;
	GameBoard *board;
	uint32_t map_size, player_x, player_y;

	easywsclient::WebSocket *web_socket;
	std::string path;

	bool is_running;
	std::thread *work_thread;
	void update_func(std::function<void()> _message_handler);

public:
	GameClientSnake(std::string _server);
	~GameClientSnake();

	void Run(std::function<void()> _message_handler);

	void Up(SnakeAction _action = SnakeAction::None)
	{
		send(std::string(_action == SnakeAction::BeforeTurn ? "ACT," : "") + "UP" + std::string(_action == SnakeAction::AfterTurn ? ",ACT" : ""));
	}
	void Down(SnakeAction _action = SnakeAction::None)
	{
		send(std::string(_action == SnakeAction::BeforeTurn ? "ACT," : "") + "DOWN" + std::string(_action == SnakeAction::AfterTurn ? ",ACT" : ""));
	}
	void Right(SnakeAction _action = SnakeAction::None)
	{
		send(std::string(_action == SnakeAction::BeforeTurn ? "ACT," : "") + "RIGHT" + std::string(_action == SnakeAction::AfterTurn ? ",ACT" : ""));
	}
	void Left(SnakeAction _action = SnakeAction::None)
	{
		send(std::string(_action == SnakeAction::BeforeTurn ? "ACT," : "") + "LEFT" + std::string(_action == SnakeAction::AfterTurn ? ",ACT" : ""));
	}
	void Act() {
		send("ACT");
	}
	void Blank() { send(""); }


	GameBoard* get_GameBoard() { return board; }

private:

	void send(std::string msg)
	{
		std::cout << "Sending: " << msg << std::endl;
		web_socket->send(msg);
	}
};
