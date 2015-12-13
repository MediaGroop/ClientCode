#pragma once
#include "RakClient.h"

class Request{
public:
	Request(){};
	virtual ~Request(){};
	virtual void instantiate(){};
};