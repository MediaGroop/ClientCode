#pragma once
#include "RakClient.h"
#include "Client.h"

class ClientPacket
{
public:
	ClientPacket(){};
	~ClientPacket(){};
	virtual void send(Client* client){};
};