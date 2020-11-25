#include <iostream>
#include <WinSock2.h>



	WSAData Wsa;
	SOCKET Listen;
	SOCKET Client;
	SOCKET Client2;
	SOCKADDR_IN local;
	SOCKADDR_IN client;


void SendIt()
{
	char *Out = new char[50];
	for(;;Sleep(75))
	{
		
		ZeroMemory(Out,sizeof(Out));
		if(std::cin >> Out)
			if(Out != NULL || Out != ZeroMemory(Out,sizeof(Out)))
			{
				send(Listen,Out+'\0',50,0);
			}
	}
	shutdown(Listen,SD_BOTH);
	delete [] Out;
}

int main(int argc,char* argv)
{


	WSAStartup(MAKEWORD(2,2),&Wsa);

	local.sin_family = AF_INET;
	local.sin_port = htons(2715);
	local.sin_addr.s_addr = inet_addr("192.168.1.3");

	Listen = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

	connect(Listen,(SOCKADDR*)&local,sizeof(local));
char* buf;
int df = 0;

		int Size = sizeof(client);
		CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)SendIt,NULL,NULL,NULL);
		for(;;Sleep(1000))
		{
		}
	
	closesocket(Client2);
	closesocket(Listen);
	closesocket(Client);
	WSACleanup();


	return 0;
}