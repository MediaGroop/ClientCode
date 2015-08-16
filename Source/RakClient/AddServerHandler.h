#pragma once
#include "RakClient.h"
#include "Network.h"

TMap<int, RakNet::RakString> ANetwork::servers;

void addServerHandler(RakNet::Packet* p)
{
	RakNet::BitStream bsIn(p->data, p->length, false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	RakNet::RakString name;
	int id;
	bsIn.Read(name);
	bsIn.Read(id);
	ANetwork::servers.Add(id, name);
	ANetwork::instance->OnAddServer(FString(name.C_String()));
	//TODO: add to servers list in UI

};