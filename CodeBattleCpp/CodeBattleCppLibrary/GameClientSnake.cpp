#include "GameClientSnake.h"
#include <iostream>

GameClientSnake::GameClientSnake(std::string _server)
{
	map = nullptr;
	board = nullptr;
	map_size = 0;

	path = _server.replace(_server.find("http"), sizeof("http")-1, "ws");
	path = path.replace(path.find("board/player/"),sizeof("board/player/")-1,"ws?user=");
	path = path.replace(path.find("?code="),sizeof("?code=")-1,"&code=");

	is_running = false;
}

GameClientSnake::~GameClientSnake()
{
	is_running = false;
	work_thread->join();
}

void GameClientSnake::Run(std::function<void()> _message_handler)
{
	is_running = true;
	work_thread = new std::thread(&GameClientSnake::update_func, this, _message_handler);
}

void GameClientSnake::update_func(std::function<void()> _message_handler)
{
#ifdef _WIN32
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
		throw new std::exception("WSAStartup Failed.\n");
	else
		std::cout << "Connection established" << std::endl;
#endif

	web_socket = easywsclient::WebSocket::from_url(path);
	if (web_socket == nullptr)is_running = false;
	while (is_running)
	{
		web_socket->poll();
		web_socket->dispatch([&](const std::string &message)
		{
			int size_needed = MultiByteToWideChar(CP_UTF8, 0, &message[0], (int)message.size(), NULL, 0);
			std::wstring wmessage(size_needed, 0);
			MultiByteToWideChar(CP_UTF8, 0, &message[0], (int)message.size(), &wmessage[0], size_needed);

			uint32_t size = sqrt(wmessage.size() - 6);
			if (map_size != size)
			{
				if (map_size != 0)
				{
					for (uint32_t j = 0; j < map_size; j++)
						delete[] map[j];
					delete[] map;
				}
				map_size = size;

				map = new BoardElement*[map_size];
				for (uint32_t j = 0; j < map_size; j++)
				{
					map[j] = new BoardElement[map_size];
					for (uint32_t i = 0; i < map_size; i++)
					{
						map[j][i] = BoardElement::NONE;
					}
				}
			}
			
			uint32_t chr = 6;
			for (uint32_t j = 0; j < map_size; j++)
			{
				for (uint32_t i = 0; i < map_size; i++)
				{
					map[j][i] = (BoardElement)wmessage[chr];
					chr++;
					
				}
			}
			board = new GameBoard(map, map_size);
			std::cout << board << '\n';
			_message_handler();
		});
	}
	if (web_socket)web_socket->close();

#ifdef _WIN32
	WSACleanup();
#endif
}
