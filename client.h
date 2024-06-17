#pragma once
#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<cassert>
#pragma comment(lib, "ws2_32.lib")



class Client
{
private:
	WSADATA wsa;
	addrinfo hints, * server_info;
	SOCKET client_socket;
	char buffer[1024];
	LPCSTR server_port;
	std::string message;
	LPCSTR server_addr;


public:
	Client(LPCSTR port, LPCSTR addr);
	~Client();
	int init();
	int proccess(std::string msg);


};