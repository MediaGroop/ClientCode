#pragma once
#include "RakClient.h"

class InstUIRequest{
public:
	InstUIRequest(){};
	virtual ~InstUIRequest(){};
	virtual void instantiate(){};
};