#include "RakClient.h"
#include "VerifyPacket.h"
#include "PacketTypes.h"

VerifyPacket::VerifyPacket(const char * acc, const char * h, const char * s) : accountName(acc), passHash(h), session(s){};

VerifyPacket::~VerifyPacket(){};

void VerifyPacket::send(Client* c){

	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)VERIFY_ACCOUNT);

	//Hashing pass hash(login + . + password) and session, result: passHash + . + session
	//Yes, there's a lot of credentials was sent, but it was hashed two times!
	uint8 data[20];
	std::string toHash = passHash;
	toHash += '.';
	toHash += session;
	FSHA1::HashBuffer(toHash.c_str(), toHash.length(), data);
	//**********************


	//Writing login for easier identify
	RakNet::StringCompressor::Instance()->EncodeString(RakNet::RakString(RakNet::RakString(this->accountName)), 256, &bsOut);
	bsOut.Write(data);

	c->peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, c->serverRemote, false);
	UE_LOG(LogTemp, Warning, TEXT("Verifying account..."));
};