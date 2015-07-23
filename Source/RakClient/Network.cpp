// Fill out your copyright notice in the Description page of Project Settings.

#include "RakClient.h"
#include "Network.h"
#include "PacketTypes.h"
#include "AuthPacket.h"

TSharedPtr<Client> ANetwork::authClient;
TSharedPtr<Client> ANetwork::serverClient;

// Sets default values
ANetwork::ANetwork()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ANetwork::sendCredentialsToAuth(FString acc, FString pass, int32 serverId)
{
	if (ANetwork::authClient.Get()->connected){
		std::string accS = TCHAR_TO_UTF8(*acc);
		std::string passS = TCHAR_TO_UTF8(*pass);
		AuthPacket packet(accS.c_str(), passS.c_str(), serverId);
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

void connectToAuth(ANetwork* i)
{
	ANetwork::authClient = TSharedPtr<Client>(new Client(i->authAddress, i->authPort));
	ANetwork::authClient.Get()->add((short)ID_CONNECTION_REQUEST_ACCEPTED, handleAuthConnect);
	ANetwork::authClient.Get()->add((short)ID_CONNECTION_LOST, onLostConn);
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

