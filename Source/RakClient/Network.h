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

	UFUNCTION(BlueprintImplementableEvent, Category = "Auth Response", meta = (DisplayName = "OnServerInfoResponse"))
	virtual void SetServerInfo(const FString& address, int32 port);
	
	UFUNCTION(BlueprintCallable, Category = "RakNet")
	void connectToServer(const FString& address, int32 port);

	UFUNCTION(BlueprintCallable, Category="RakNet")
	static void sendCredentialsToAuth(FString acc, FString pass);

	UFUNCTION(BlueprintImplementableEvent, Category = "Auth Response", meta = (DisplayName = "OnAuthResponse"))
	virtual void ShowAuthResponse(const FString& text);

	UFUNCTION(BlueprintImplementableEvent, Category = "Auth Response", meta = (DisplayName = "OnAddServer"))
	virtual void OnAddServer(const FString& text);

	UFUNCTION(BlueprintImplementableEvent, Category = "Character management", meta = (DisplayName = "OnAddCharacterPreview"))
	virtual void AddCharacterPreview(const FString& name, const FString& server, int32 id);

	UFUNCTION(BlueprintImplementableEvent, Category = "Character management", meta = (DisplayName = "OnNickAvailChange"))
	virtual void SetNickAvailability(const bool avail);

	UFUNCTION(BlueprintImplementableEvent, Category = "Auth Response", meta = (DisplayName = "OnAuthSuccess"))
	virtual void OpenPersonalCabinet(const FString& mail, const bool premium, const bool beta);

	UFUNCTION(BlueprintCallable, Category = "Auth send", meta = (DisplayName = "Send Packet to Auth"))
	void sendToAuth(const uint8& ID, const FString& arg1, const FString& arg2);

	UFUNCTION(BlueprintCallable, Category = "Auth send", meta = (DisplayName = "Send Packet to Auth(1 arg)"))
	void sendToAuth1(const uint8& ID, const FString& arg1);

	UFUNCTION(BlueprintCallable, Category = "Auth send", meta = (DisplayName = "Send Packet to Auth(1 arg int)"))
	void sendToAuth1int(const uint8& ID, int32 arg1);

	UFUNCTION(BlueprintCallable, Category = "Server send", meta = (DisplayName = "Send Packet to Server(1 arg string)"))
	void sendToServer1Str(const uint8& ID, const FString& arg1);

	UFUNCTION(BlueprintCallable, Category = "Native setters", meta = (DisplayName = "Set selected char name"))
	void setCharName(const FString& name);

	static TSharedPtr<Client> authClient;
	static TSharedPtr<Client> serverClient;

	static unsigned char session[20];
	static RakNet::RakString login;
	static unsigned char passHash[20];
	static ANetwork* instance;
	static RakNet::RakString charName;
	static TSet<InstUIRequest*> requests;

	static int characters_count;
	bool authlaunched = false;
	const unsigned short authPort = 25565;
	const std::string authAddress = "localhost";
};
