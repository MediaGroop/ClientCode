// Fill out your copyright notice in the Description page of Project Settings.

#include "RakClient.h"
#include "Network.h"
#include "PacketTypes.h"
#include "AuthPacket.h"
#include "VerifyPacket.h"
#include "AddServerHandler.h"

TSharedPtr<Client> ANetwork::authClient;
TSharedPtr<Client> ANetwork::serverClient;
unsigned char ANetwork::session[20];
RakNet::RakString  ANetwork::login;
unsigned char ANetwork::passHash[20];
ANetwork* ANetwork::instance;
TSet<InstUIRequest*> ANetwork::requests;

void handleAddCharacter(RakNet::Packet * p)
{

	RakNet::BitStream bsIn(p->data, p->length, false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	RakNet::RakString rak_name, rak_serv_name;

	RakNet::StringCompressor::Instance()->DecodeString(&rak_name, 256, &bsIn);
	RakNet::StringCompressor::Instance()->DecodeString(&rak_serv_name, 256, &bsIn);
	//UE_LOG(LogTemp, Warning, TEXT("HandleAddChar"));
	ANetwork::requests.Add(new CharPreview(*new FString(rak_name.C_String()), *FString(rak_serv_name.C_String())));

};

// Sets default values
ANetwork::ANetwork()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ANetwork::instance = this;
}

void ANetwork::sendCredentialsToAuth(FString acc, FString pass)
{
	if (ANetwork::authClient.Get()->connected){
		std::string accS = TCHAR_TO_UTF8(*acc);
		std::string passS = TCHAR_TO_UTF8(*pass);

		//Storing login for future verification
		ANetwork::login = *(new RakNet::RakString(accS.c_str()));
		//****************

		//Hashing and saving password(with salt) for future verification
		unsigned char data[20];
		char* toHash = new char[1 + passS.length() + accS.length()];

		for (int i = 0; i < accS.length(); ++i)
			toHash[i] = accS.c_str()[i];

		toHash[accS.length()] = '.';

		for (int i = 0; i < accS.length(); ++i)
			toHash[i + accS.length() + 1] = passS.c_str()[i];

		FSHA1::HashBuffer(toHash, 1 + passS.length() + accS.length(), data);
		for (int i = 0; i < 20; ++i)
			ANetwork::passHash[i] = data[i];
		//****************

		AuthPacket packet(RakNet::RakString(accS.c_str()), data);
		packet.send(ANetwork::authClient.Get());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Client are not connected!"));
	}
	//TODO: Show loading box...
}

void handleAuthConnect(RakNet::Packet* p)
{
	ANetwork::authClient.Get()->serverRemote = p->systemAddress;
	UE_LOG(LogTemp, Warning, TEXT("We are connected to the auth server!"));
	ANetwork::authClient.Get()->connected = true;
	//Handle connect, show it to the user
}

void onLostConn(RakNet::Packet* p)
{
	ANetwork::authClient.Get()->connected = false;
	UE_LOG(LogTemp, Warning, TEXT("Connection to auth server has been lost."));
	//Show to user
}

void onGameServerConnect(RakNet::Packet* p)
{
	//Send veirfication data(Login and hash of password's hash(with salt) and session)
	ANetwork::serverClient.Get()->serverRemote = p->systemAddress;
	VerifyPacket packet(ANetwork::login.C_String(), ANetwork::passHash, ANetwork::session);
	packet.send(ANetwork::serverClient.Get());
	UE_LOG(LogTemp, Warning, TEXT("We are connected to game server."));

	//Now waiting for a response....
}

void onAccountVerifyResponse(RakNet::Packet* packet)
{
	RakNet::BitStream bsIn(packet->data, packet->length, false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

	unsigned char result;
	bsIn.Read(result);
	UE_LOG(LogTemp, Warning, TEXT("Verification response: %i"), result);
};

/*
void ANetwork::AddCharacterPreview_Implementation(const FString& c, const FString& n)
{

}

void ANetwork::ShowAuthResponse_Implementation(const FString& n)
{

}*/

void onAuthResponse(RakNet::Packet* p)
{
	RakNet::BitStream bsIn(p->data, p->length, false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	
	unsigned char respId;
	RakNet::RakString mail;
	bool prem, beta;
	
	bsIn.Read(respId);

	switch (respId)
	{
	case 0:
		//no user
		UE_LOG(LogTemp, Warning, TEXT("There's no user!"));
		ANetwork::instance->ShowAuthResponse(FString("Wrong login or pass!"));
		break;
	case 1:
		//wrong pass
		UE_LOG(LogTemp, Warning, TEXT("Wrong pass!"));
		ANetwork::instance->ShowAuthResponse(FString("Wrong login or pass!"));
		break;
	case 2:
		//not premium
		UE_LOG(LogTemp, Warning, TEXT("Not premium!"));
		ANetwork::instance->ShowAuthResponse(FString("Account not premium!"));

		break;
	case 3:
		//success
		for (int i = 0; i < 20; ++i)
			bsIn.Read(ANetwork::session[i]);

		bsIn.Read(mail);
		bsIn.Read(prem);
		bsIn.Read(beta);

		UE_LOG(LogTemp, Warning, TEXT("Success!"));
		ANetwork::instance->OpenPersonalCabinet(FString(mail.C_String()), prem, beta);
		//Request personal info here
		//...
		//
		//ANetwork::instance->ShowAuthResponse(FString("Success!"));

		break;
	case 4:
		//already logged
		UE_LOG(LogTemp, Warning, TEXT("Already logged!"));
		ANetwork::instance->ShowAuthResponse(FString("Already logged in!"));
		break;
	}
	//Show to user
}

void handleNicknameCheck(RakNet::Packet* p){

	RakNet::BitStream bsIn(p->data, p->length, false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	bool allowed = false;
	bsIn.Read(allowed);
	ANetwork::instance->SetNickAvailability(allowed);
};


void handleServerInfo(RakNet::Packet* p){

	RakNet::BitStream bsIn(p->data, p->length, false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	RakNet::RakString addr;
	int port;
	bsIn.Read(addr);
	bsIn.Read(port);
	ANetwork::instance->SetServerInfo(FString(addr.C_String()), port);
};

void connectToAuth(ANetwork* i)
{
	ANetwork::authClient = TSharedPtr<Client>(new Client(i->authAddress, i->authPort));
	ANetwork::authClient.Get()->add((short)ID_CONNECTION_REQUEST_ACCEPTED, handleAuthConnect);
	ANetwork::authClient.Get()->add((short)ID_CONNECTION_LOST, onLostConn);
	ANetwork::authClient.Get()->add((short)AUTH_RESPONCE, onAuthResponse);
	ANetwork::authClient.Get()->add((short)ADD_SERVER, addServerHandler);
	ANetwork::authClient.Get()->add((short)CREATE_CHARACTER, handleAddCharacter);
	ANetwork::authClient.Get()->add((short)CHECK_NICKNAME, handleNicknameCheck);
	ANetwork::authClient.Get()->add((short)SERVER_INFO, handleServerInfo);


	ANetwork::authClient.Get()->initSecurity();
	ANetwork::authClient.Get()->launch();
}

int ANetwork::characters_count;

// Called when the game starts or when spawned
void ANetwork::BeginPlay()
{
	Super::BeginPlay();
	connectToAuth(this);
	characters_count = 0;
	
}

void ANetwork::connectToServer(const FString& host, int32 port)
{

	ANetwork::serverClient = TSharedPtr<Client>(new Client(std::string(TCHAR_TO_UTF8(*host)), port));

	//init handlers...
	ANetwork::serverClient.Get()->add((short)ID_CONNECTION_REQUEST_ACCEPTED, onGameServerConnect);
	ANetwork::serverClient.Get()->add((short)VERIFY_RESPONSE, onAccountVerifyResponse);

	ANetwork::serverClient.Get()->launch();
}

void ANetwork::sendToAuth(const uint8& ID, const FString& arg1, const FString& arg2)
{
	UE_LOG(LogTemp, Warning, TEXT("Sending packet: %i"), ID);
	RakNet::BitStream bsOut;
	bsOut.Write(ID);
	bsOut.Write(RakNet::RakString(TCHAR_TO_UTF8(*arg1)));
	bsOut.Write(RakNet::RakString(TCHAR_TO_UTF8(*arg2)));
	ANetwork::authClient.Get()->peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, ANetwork::authClient.Get()->serverRemote, false);
}

// Called every frame
void ANetwork::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TSet<InstUIRequest*>::TIterator i = ANetwork::requests.CreateIterator();
	while (i)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Instantiate"));
		(*i)->instantiate();
		i.RemoveCurrent();
		++i;
	}
}

