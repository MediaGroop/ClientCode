#pragma once
#include "RakClient.h"

class AuthPacket :
	public ClientPacket
{
private:
	RakNet::RakString accountName;
	unsigned char password[20];
public:
	AuthPacket(RakNet::RakString, unsigned char h[20]);
	~AuthPacket();
	virtual void send(Client* c) override;
};

