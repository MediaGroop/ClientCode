// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "RakClient.h"
#include "GameFramework/Actor.h"
#include "Network.generated.h"

UCLASS()
class RAKCLIENT_API ANetwork : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANetwork();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//void connectToAuth();
	void connectToServer(std::string, int);

	UFUNCTION(BlueprintCallable, Category="RakNet")
	static void sendCredentialsToAuth(FString acc, FString pass, uint8 serverId);

	static TSharedPtr<Client> authClient;
	static TSharedPtr<Client> serverClient;

	static RakNet::RakString session;
	static RakNet::RakString login;
	static RakNet::RakString passHash;

	bool authlaunched = false;
	const unsigned short authPort = 25565;
	const std::string authAddress = "localhost";
};
