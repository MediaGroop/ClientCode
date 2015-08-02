#pragma once
#include "RakClient.h"
#include "ClientPacket.h"

class VerifyPacket : public ClientPacket
{
private:
	const char* accountName;
	const char* passHash;
	const char* session;
public:
	//acc, pass hash, session
	VerifyPacket(const char*, const char*, const char*);
	~VerifyPacket();
	virtual void send(Client* c) override;
};