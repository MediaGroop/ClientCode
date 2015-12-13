#include "RakClient.h"
#include "LoadWorldRequest.h"


LoadWorldRequest::LoadWorldRequest(RakNet::RakString name)
{
	_name = name + ".umap";
}


LoadWorldRequest::~LoadWorldRequest()
{
}
