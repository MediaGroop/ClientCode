// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#ifndef __RAKCLIENT_H__
#define __RAKCLIENT_H__

#include "EngineMinimal.h"

#include <functional>
#include <string>
#include <map>

#include "ClientPacket.h"
#include "PacketTypes.h"
#include "InstUIRequest.h"
#include "CharPreview.h"
#include "ServerInfo.h"

#include "AllowWindowsPlatformTypes.h"
#include "ThirdParty/RakNet/RakNet-master/Source/MessageIdentifiers.h"
#include "ThirdParty/RakNet/RakNet-master/Source/RakPeerInterface.h"
#include "ThirdParty/RakNet/RakNet-master/Source/RakNetTypes.h"
#include "ThirdParty/RakNet/RakNet-master/Source/BitStream.h"
#include "ThirdParty/RakNet/RakNet-master/Source/StringCompressor.h"
#include "ThirdParty/RakNet/RakNet-master/Source/SecureHandshake.h"
#include "HideWindowsPlatformTypes.h"

DECLARE_LOG_CATEGORY_EXTERN(LogRakClient, Log, All);

#endif
