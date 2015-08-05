// Fill out your copyright notice in the Description page of Project Settings.

#include "RakClient.h"
#include "Network.h"
#include "PacketTypes.h"
#include "AuthPacket.h"
#include "VerifyPacket.h"

TSharedPtr<Client> ANetwork::authClient;
TSharedPtr<Client> ANetwork::serverClient;
unsigned char ANetwork::session[20];
RakNet::RakString  ANetwork::login;
unsigned char ANetwork::passHash[20];

// Sets default values
ANetwork::ANetwork()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ANetwork::sendCredentialsToAuth(FString acc, FString pass, uint8 serverId)
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

		AuthPacket packet(RakNet::RakString(accS.c_str()), data, serverId);
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

void onAuthResponse(RakNet::Packet* p)
{
	RakNet::BitStream bsIn(p->data, p->length, false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	unsigned char respId;
	RakNet::RakString address;
	int port;
	bsIn.Read(respId);
	switch (respId)
	{
	case 0:
		//no user
		UE_LOG(LogTemp, Warning, TEXT("There's no user!"));

		break;
	case 1:
		//wrong pass
		UE_LOG(LogTemp, Warning, TEXT("Wrong pass!"));

		break;
	case 2:
		//not premium
		UE_LOG(LogTemp, Warning, TEXT("Not premium!"));

		break;
	case 3:
		//success
		for (int i = 0; i < 20; ++i)
		bsIn.Read(ANetwork::session[i]);

		bsIn.Read(address);
		bsIn.Read(port);
		
		ANetwork::serverClient = TSharedPtr<Client>(new Client(std::string(address.C_String()), port));
	
		//init handlers...
		ANetwork::serverClient.Get()->add((short)ID_CONNECTION_REQUEST_ACCEPTED, onGameServerConnect);
		ANetwork::serverClient.Get()->add((short)VERIFY_RESPONSE, onAccountVerifyResponse);

		ANetwork::serverClient.Get()->launch();
		UE_LOG(LogTemp, Warning, TEXT("Success!"));

		break;
	case 4:
		//no server
		UE_LOG(LogTemp, Warning, TEXT("No server!"));

		break;
	}
	//Show to user
}

void connectToAuth(ANetwork* i)
{
	ANetwork::authClient = TSharedPtr<Client>(new Client(i->authAddress, i->authPort));
	ANetwork::authClient.Get()->add((short)ID_CONNECTION_REQUEST_ACCEPTED, handleAuthConnect);
	ANetwork::authClient.Get()->add((short)ID_CONNECTION_LOST, onLostConn);
	ANetwork::authClient.Get()->add((short)AUTH_RESPONCE, onAuthResponse);

	ANetwork::authClient.Get()->initSecurity();
	ANetwork::authClient.Get()->launch();
}


// Called when the game starts or when spawned
void ANetwork::BeginPlay()
{
	Super::BeginPlay();
	connectToAuth(this);
}

void ANetwork::connectToServer(std::string host, int port)
{

}

// Called every frame
void ANetwork::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

