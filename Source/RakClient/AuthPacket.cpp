#include "RakClient.h"
#include "AuthPacket.h"


AuthPacket::AuthPacket(const char* acc, const char* pass, int serverId)
{
	this->accountName = acc;
	this->password = pass;
	this->serverId = serverId;
}


AuthPacket::~AuthPacket()
{
}

void AuthPacket::send(Client* c)
{
	if (c->connected){
		RakNet::BitStream bsOut;

		bsOut.Write((RakNet::MessageID)ACCOUNT_AUTH);
		RakNet::StringCompressor::Instance()->EncodeString(RakNet::RakString(this->accountName), 256, &bsOut);
		RakNet::RakString hash = accountName;
		hash += '.';
		hash += this->password;
		uint8 data[20];
		FSHA1::HashBuffer(hash.C_String(), hash.GetLength(), data);
		//RakNet::StringCompressor::Instance()->EncodeString(RakNet::RakString(this->password), 256, &bsOut);
		bsOut.Write(data);
		bsOut.Write(this->serverId);
		c->peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, c->serverRemote, false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Client aren't connected to the remote server!"));
	}
}