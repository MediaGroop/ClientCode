#pragma once
#include "RakClient.h"

enum GameMessages
{
	ID_GAME_MESSAGE_1 = 135,
	AUTH_TO_POOLER =  136,
	ACCOUNT_AUTH = 137,
	AUTH_RESPONCE = ACCOUNT_AUTH + 1,
	VERIFY_ACCOUNT = AUTH_RESPONCE + 1,
	VERIFY_RESPONSE = VERIFY_ACCOUNT + 1

};