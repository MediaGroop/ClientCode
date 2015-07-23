#pragma once
#include "RakClient.h"

class AuthPacket :
	public ClientPacket
{
private:
	const char* accountName;
	const char* password;
	int serverId;
public:
	AuthPacket(const char*, const char*, int);
	~AuthPacket();
	virtual void send(Client* c) override;
};

