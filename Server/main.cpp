#include <SFML/Network.hpp>
#include <thread>
#include <iostream>
#include <string.h>

#include "GameEngine.h"
#include "MessageServer.h"
using namespace std;


void listen()
{
	sf::TcpListener listener;
	if (listener.listen(2040) != sf::Socket::Done)
	{
		std::cout << "can't create socket" << std::endl;
		exit(-1);
	}

	while (1)
	{
		try
		{
		
		sf::TcpSocket socket;
		if (listener.accept(socket) != sf::Socket::Done)
		{
			printf("Error\n");
		}

		char data[65536];
		std::size_t received = 0;

		if (socket.receive(data, 65536, received) != sf::Socket::Done)
		{
			
		}

		{
			lock_guard<mutex> lock(m);
			try
			{
				messageServer.request = nlohmann::json::parse(data);
			}
			catch(const nlohmann::json::parse_error& e)
			{
				cerr << "Error parsing JSON: " << e.what() << endl;
			}
			
			messageServer.isResponse = false;
		}

		while (1)
		{
			this_thread::sleep_for(chrono::milliseconds(10));
			{
				lock_guard<mutex> lock(m);
				if (messageServer.isResponse)
				{
					string data2 = messageServer.response.dump();	

					if (socket.send(data2.c_str(), data2.size() + 1) != sf::Socket::Done)
					{
					
					}
					else
					{
						cout << data2 << endl;
					}
					break;
				}
			}

		}

		socket.disconnect();
		}
		catch (const exception& e)
		{
			cerr << "Error socket thread: = " << e.what() << endl;
		}
		catch (...)
		{
			cerr << "Error" << endl;
		}
	}
}

int main() {
	
	Server::GameEngine _Engine;
	thread server(Server::runServer, std::ref(_Engine));
	thread t(listen);
	server.join();
	t.detach();

	return 0;
}