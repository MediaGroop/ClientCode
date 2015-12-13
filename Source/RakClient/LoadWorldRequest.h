#pragma once
#include "RakClient.h"
#include "InstUIRequest.h"

class LoadWorldRequest :
	public Request
{
private:
	RakNet::RakString _name;
public:
	LoadWorldRequest(RakNet::RakString name);
	~LoadWorldRequest();
	virtual void instantiate() override{
		ANetwork::instance->LoadWorld(FString(_name.C_String()));
	};
};

