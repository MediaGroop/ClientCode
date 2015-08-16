#pragma once
#include "RakClient.h"
#include "Network.h"
#include "InstUIRequest.h"

//ANetwork* ANetwork::instance;

class CharPreview : public InstUIRequest{
private:
	FString name;
	FString server;
public:
	CharPreview(FString c, FString s) : InstUIRequest(), name(c), server(s) {};

	~CharPreview(){};

	virtual void instantiate() override{
		UE_LOG(LogTemp, Warning, TEXT("Instantiate char preview"));
		ANetwork::instance->AddCharacterPreview(name, server);
	};
};