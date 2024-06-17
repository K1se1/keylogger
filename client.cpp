#include "client.h"

Client::Client(LPCSTR port, LPCSTR addr) :
	wsa{ 0 },
	hints{ 0 },
	client_socket(SOCKET_ERROR),
	server_addr(addr),
	server_port(port),
	buffer{0},
	server_info(0)
{


}

int Client::init()
{
	addrinfo* p;
	assert(WSAStartup(MAKEWORD(2, 2), &wsa) == 0); // запускаем wsa
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if (getaddrinfo("localhost", "8000", &hints, &server_info) != 0) {
		std::cout << "Couldn't get an address!!!";  return 1;
	}
	for (p = server_info; p != NULL; p = p->ai_next)
	{
		if ((client_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == SOCKET_ERROR) {
			std::cout << WSAGetLastError();
			continue;
		}
		if (connect(client_socket, p->ai_addr, p->ai_addrlen) == -1) {
			closesocket(client_socket);
			std::cout << WSAGetLastError();
			continue;
		}
		break;
	}
	if (!p) return 1;
	return 0;
}

int Client::proccess(std::string msg)
{
		message = msg;
		strncpy_s(buffer, message.c_str(), 4096);
		if (send(client_socket, buffer, 1024, 0) < 0) std::cout << "Can't send a message!" << WSAGetLastError();
		return 0;
}




Client::~Client()
{
	shutdown(client_socket, SD_BOTH);
	closesocket(client_socket);
	freeaddrinfo(server_info);
}