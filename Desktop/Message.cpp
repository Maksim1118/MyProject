#include "Message.h"

Message message;

Message::Message()
{

}

Message::~Message()
{
	
}

nlohmann::json Message::process(nlohmann::json request)
{
	sf::TcpSocket socket;
	sf::Socket::Status status = socket.connect("127.0.0.1", 2040);
	if (status != sf::Socket::Done)
	{
		
	}

	string data = request.dump();

	if (socket.send(data.c_str(), data.size() + 1) != sf::Socket::Done)
	{
		cerr << "client send message error" << endl;
	}

	char data2[131072];
	std::size_t received = 0;

	if (socket.receive(data2, 131071, received) != sf::Socket::Done)
	{
		cerr << "Socket failed recived" << endl;
	}
	
	if (!received)
	{
		data2[0] = 0;
	}
	
	socket.disconnect();
	nlohmann::json response;
	try
	{
		response = nlohmann::json::parse(data2);
	}
	catch (const nlohmann::json::parse_error& e)
	{
		cerr << "Error parsing JSON: " << e.what() << endl;
		cerr << data2 << endl;
	}

	return response;
}
