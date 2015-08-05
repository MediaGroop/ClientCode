#pragma once
#include "RakClient.h"

class AuthPacket :
	public ClientPacket
{
private:
	RakNet::RakString accountName;
	unsigned char password[20];
	int serverId;
public:
	AuthPacket(RakNet::RakString, unsigned char h[20], int);
	~AuthPacket();
	virtual void send(Client* c) override;
};

