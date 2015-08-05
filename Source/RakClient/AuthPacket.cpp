#include "RakClient.h"
#include "AuthPacket.h"


AuthPacket::AuthPacket(RakNet::RakString acc, unsigned char pass[20], int serverId)
{
	this->accountName = acc;
	for (int i = 0; i < 20; ++i){
		this->password[i] = pass[i];
		UE_LOG(LogTemp, Warning, TEXT("HASH: %i"), password[i]);

	}
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
		RakNet::StringCompressor::Instance()->EncodeString(accountName, 256, &bsOut);
		for (int i = 0; i < 20; ++i)
			bsOut.Write(password[i]);

		bsOut.Write(serverId);
		c->peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, c->serverRemote, false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Client aren't connected to the remote server!"));
	}
}