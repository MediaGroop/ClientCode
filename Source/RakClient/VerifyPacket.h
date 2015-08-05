#pragma once
#include "RakClient.h"
#include "ClientPacket.h"

class VerifyPacket : public ClientPacket
{
private:
	const char* accountName;
	unsigned char passHash[20];
	unsigned char session[20];
public:
	//acc, pass hash, session
	VerifyPacket(const char*, unsigned char p[20], unsigned char s[20]);
	~VerifyPacket();
	virtual void send(Client* c) override;
};