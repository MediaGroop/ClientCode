#pragma once
#include "RakClient.h"
#include "Network.h"
#include "InstUIRequest.h"

//ANetwork* ANetwork::instance;

class CharPreview : public InstUIRequest{
private:
	FString name;
	FString server;
	int id;
public:
	CharPreview(FString c, FString s, int i) : InstUIRequest(), name(c), server(s), id(i) {};

	~CharPreview(){};

	virtual void instantiate() override{
		UE_LOG(LogTemp, Warning, TEXT("Instantiate char preview"));
		ANetwork::instance->AddCharacterPreview(name, server, id);
	};
};