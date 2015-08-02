#pragma once
#include "RakClient.h"
#include "AllowWindowsPlatformTypes.h"
#include "ThirdParty/RakNet/RakNet-master/Source/MessageIdentifiers.h"
#include "ThirdParty/RakNet/RakNet-master/Source/RakPeerInterface.h"
#include "ThirdParty/RakNet/RakNet-master/Source/RakNetTypes.h"
#include "ThirdParty/RakNet/RakNet-master/Source/BitStream.h"
#include "ThirdParty/RakNet/RakNet-master/Source/StringCompressor.h"
#include "ThirdParty/RakNet/RakNet-master/Source/SecureHandshake.h"
#include "ThirdParty/RakNet/RakNet-master/Source/NativeFeatureIncludes.h"

#include "HideWindowsPlatformTypes.h"

class Client: public FRunnable
{
public:

	Client(std::string host, unsigned short port);
	~Client();
	
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

	void configure(std::string h, unsigned short port);
	void setSleep(double);
	void launch();

	RakNet::AddressOrGUID serverRemote;
	
	bool connected = false;
	bool running;
	bool secure = false;

	char* pkey;
	
	RakNet::RakPeerInterface* peer;
	double sleep = 0.1;

	//Call before launch, if you want to init secure connection
	void initSecurity();

	std::map<short, std::function<void(RakNet::Packet*)>> _handlers;
	
	void add(short, std::function<void(RakNet::Packet* p)>);
	void handle(RakNet::Packet *);

private:
	std::string host;
	unsigned short port;
};