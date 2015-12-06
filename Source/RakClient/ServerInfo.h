#pragma once
#include "RakClient.h"

class ServerInfo{
public:
	RakNet::RakString addr;
	int port;
	ServerInfo(RakNet::RakString s, int p) : port(p), addr(s){};
	~ServerInfo(){};
};