#include "RakClient.h"
#include "Client.h"

void Client::launch(){
	FRunnableThread *trd = FRunnableThread::Create(this, TEXT("Client"));
}

Client::Client(std::string host, unsigned short port)
{
	UE_LOG(LogTemp, Warning, TEXT("Network client init..."));
	this->host = host;
	this->port = port;
	pkey = new char[64]{
		-88, -87, 47, 23,
		14, -11, -104, 124,
		8, -12, 98, 111,
		-53, 56, 100, -110,

		46, -79, 14, -102,
		-126, 117, 15, -71,
		-23, 118, 96, -102,
		0, 84, -113, -19,

		-113, 62, -16, -15,
		64, -94, -72, -78,
		89, -101, -5, 32,
		-56, 122, -101, -101,

		-114, -105, -36, 76,
		94, 18, 97, 50,
		21, -66, 109, 11,
		-81, -74, 17, 110 };
}

void Client::Stop()
{
	running = false;
}

void Client::Exit()
{
	Stop();
}

void Client::configure(std::string h, unsigned short port)
{
	this->host = h;
	this->port = port;
}

void Client::setSleep(double t)
{
	this->sleep = t;
}

//Reads public key
void Client::initSecurity()
{
	/*
	TArray<int8> arr;
	arr.Init(64);
	FString path = "C:\\Users\\Alastar\\Documents\\Unreal Projects\\RakClient\\Source\\RakClient\\public";
	//TODO: change for packaged game!
	FArchive* Reader = IFileManager::Get().CreateFileReader(*path, 0);
	if (!Reader)
	{
		UE_LOG(LogStreaming, Warning, TEXT("Failed to read file '%s' error."), *path);
	}
	else{
		arr.Reset();
		arr.AddUninitialized(Reader->TotalSize());
		Reader->Serialize(arr.GetData(), arr.Num());
		bool Success = Reader->Close();
		delete Reader;

		if (Success){
			for (int i = 0; i < 64; ++i)
			{
				UE_LOG(LogTemp, Warning, TEXT("%i) %i"), i, arr[i]);
				this->pkey[i] = arr[i];
			}
			this->secure = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No file!"));
		}
	}
	*/
	this->secure = true;
}

uint32 Client::Run(){
	peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor sd;
	sd.port = 0;
	peer->Startup(1, &sd, 1);
	UE_LOG(LogTemp, Warning, TEXT("Connecting..."));

	if (secure){
		RakNet::PublicKey key;
		key.remoteServerPublicKey = pkey;
		key.publicKeyMode = RakNet::PKM_USE_KNOWN_PUBLIC_KEY;

		UE_LOG(LogTemp, Warning, TEXT("Running secured client..."));
		if (RakNet::CONNECTION_ATTEMPT_STARTED != peer->Connect(host.c_str(), port, 0, 0, &key))
		{
			UE_LOG(LogTemp, Error, TEXT("ERROR: Connect() returned false - invalid public key most likely"));
		}
	}
	else{
		UE_LOG(LogTemp, Warning, TEXT("Running simple client..."));
		peer->Connect(host.c_str(), port, 0, 0);
	}

	running = true;
	RakNet::Packet *packet;

	while (running)
	{
		//	UE_LOG(LogTemp, Warning, TEXT("Running thread..."));
		FPlatformProcess::Sleep(sleep);
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			handle(packet);
		}
	}
	RakNet::RakPeerInterface::DestroyInstance(peer);
	UE_LOG(LogTemp, Warning, TEXT("End of thread!"));
	return 0;
}
Client::~Client()
{
}


void Client::handle(RakNet::Packet *packet){

	short s = packet->data[0];
	UE_LOG(LogTemp, Warning, TEXT("A message with ID: %i has been arrived!"), s);

	//LOG(INFO) << "A message with ID: " << s << " has been arrived!";
	for (std::map<short, std::function<void(RakNet::Packet*)>>::iterator ii = this->_handlers.begin(); ii != this->_handlers.end(); ++ii)
	{
		if ((*ii).first == packet->data[0])
		{
			(*ii).second(packet);
			break;
		}
	}
}

void Client::add(short id, std::function<void(RakNet::Packet*)> exec){
	this->_handlers.insert(std::pair<short, std::function<void(RakNet::Packet*)>>(id, exec));
}