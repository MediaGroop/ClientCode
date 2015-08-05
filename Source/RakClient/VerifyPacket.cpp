#include "RakClient.h"
#include "VerifyPacket.h"
#include "PacketTypes.h"

VerifyPacket::VerifyPacket(const char * acc, unsigned char h[20], unsigned char s[20]) : accountName(acc){
	for (int i = 0; i < 20; ++i)
		passHash[i] = h[i];

	for (int i = 0; i < 20; ++i)
		session[i] = s[i];
};

VerifyPacket::~VerifyPacket(){};

void VerifyPacket::send(Client* c){

	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)VERIFY_ACCOUNT);

	//Hashing pass hash(login + . + password) and session, result: passHash + . + session
	//Yes, there's a lot of credentials was sent, but it was hashed two times!
	uint8 data[20];
	unsigned char* toHash = new unsigned char[41];
	
	for (int i = 0; i < 20; ++i)
		toHash[i] = passHash[i];
	
	toHash[20] = '.';

	for (int i = 0; i < 20; ++i)
		toHash[i + 21] = session[i];

	FSHA1::HashBuffer(toHash, 41, data);
	//**********************


	//Writing login for easier identify
	RakNet::StringCompressor::Instance()->EncodeString(RakNet::RakString(RakNet::RakString(this->accountName)), 256, &bsOut);
	
	for (int i = 0; i < 20; ++i){
		bsOut.Write(data[i]);
		UE_LOG(LogTemp, Warning, TEXT("VERIFY HASH: %i"), data[i]);
	}

	c->peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, c->serverRemote, false);
	UE_LOG(LogTemp, Warning, TEXT("Verifying account..."));
};