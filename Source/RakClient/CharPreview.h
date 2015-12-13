#pragma once
#include "RakClient.h"
#include "Network.h"
#include "InstUIRequest.h"


class CharPreview : public Request{
private:
	FString name;
	FString server;
	int id;
public:
	CharPreview(FString c, FString s, int i) : Request(), name(c), server(s), id(i) {};

	~CharPreview(){};

	virtual void instantiate() override{
		UE_LOG(LogTemp, Warning, TEXT("Instantiate char preview"));
		ANetwork::instance->AddCharacterPreview(name, server, id);
	};
};