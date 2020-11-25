#include "localization.h"

//For Example D:/Games/Day/arma2oaserver.exe
localization::localization(void)
{

	Text_b_offServer = "\0";
	Text_b_offDataBase = "\0";
	Text_b_offServerAndDataBase = "\0";
	Text_b_restartDataBase = "\0";
	Text_b_restartServer = "\0";
	Text_b_restartServerAndDataBase = "\0";
	Text_b_onDataBase = "\0";
	Text_b_onServer = "\0";
	Text_b_onServerAndDataBase = "\0";
	Text_b_onDart = "\0";
	Text_m_options = "\0";
	Text_m_about = "\0";
	Text_b_language = "\0";
	Text_b_locale = "\0";
	Text_s_server = "\0";
	Text_s_apache = "\0";
	Text_s_mySql = "\0";
	Text_s_example = "\0";

	LocaleKey = 0;
}


localization::~localization(void)
{
		
}

char* localization::ReadIni(char* section,char* key,char* fileName)
{
	char buffer[512] = "\0";
	char path[20] = "localization.ini";
	int lenOfBuffer = 0;
	GetModuleFileName(NULL,buffer,512);
	//lenAEN = strlen(ApplicationExeName);
	lenOfBuffer = strlen(buffer);
	buffer[lenOfBuffer - 22] = '\0';
					
	strcat_s(buffer,path);
	
	//char* OutData = new char[512];
	char* OutData = new char[512];
	GetPrivateProfileString((LPCSTR)section,(LPCSTR)key,NULL,OutData,512,(LPCSTR)buffer);

	//char* Out[512] = {"\0"};
	//Out = OutData;
	
	//Memory leak.Утечка памяти!
	return OutData;
	delete []OutData;
}

void localization::LocaleInit()
{
	//--Localizs Begin--\\

	char buffer[512] = "\0";
	char path[20] = "localization.ini";
	int lenOfBuffer = 0;
	GetModuleFileName(NULL,buffer,512);
	//lenAEN = strlen(ApplicationExeName);
	lenOfBuffer = strlen(buffer);
	buffer[lenOfBuffer - 22] = '\0';
					
	strcat_s(buffer,path);

	bool RusInit = false;
	bool EnInit = false;

	int loc = 2;

	//loc = GetPrivateProfileInt("locale","language",NULL,buffer);
	LocaleKey = loc;
	//Russian Loc
		if(loc == 1)
		{
			Text_b_offServer = ReadIni("russian","b_offServer",buffer);
			Text_b_offDataBase = ReadIni("russian","b_offDataBase",buffer);
			Text_b_offServerAndDataBase = ReadIni("russian","b_offServerAndDataBase",buffer);
			Text_b_restartDataBase = ReadIni("russian","b_restartDataBase",buffer);
			Text_b_restartServer = ReadIni("russian","b_restartServer",buffer);
			Text_b_restartServerAndDataBase = ReadIni("russian","b_restartServerAndDataBase",buffer);
			Text_b_onDataBase = ReadIni("russian","b_onDataBase",buffer);
			Text_b_onServer = ReadIni("russian","b_onServer",buffer);
			Text_b_onServerAndDataBase = ReadIni("russian","b_onServerAndDataBase",buffer);
			Text_b_onDart = ReadIni("russian","b_onDart",buffer);
			Text_m_options = ReadIni("russian","m_options",buffer);
			Text_m_about = ReadIni("russian","m_information",buffer);
			Text_b_locale = ReadIni("english","b_locale",buffer);
			Text_b_language = ReadIni("english","b_language",buffer);
			Text_s_server = ReadIni("russian","s_server",buffer);
			Text_s_apache = ReadIni("russian","s_apache",buffer);
			Text_s_mySql = ReadIni("russian","s_mysql",buffer);
			Text_s_example = ReadIni("russian","s_example",buffer);
		}
		
	//English loc
		if(loc == 2)
		{
			Text_b_offDataBase="Shutdown DataBase";
			Text_b_offServer="Shutdown Server";
			Text_b_offServerAndDataBase="Shutdown Server and DataBase";
			Text_b_restartDataBase="Restart DataBase";
			Text_b_restartServer="Restart Server";
			Text_b_restartServerAndDataBase="Restart Server and DataBase";
			Text_b_onDataBase="Enable DataBase";
			Text_b_onServer="Enable Server";
			Text_b_onServerAndDataBase="Enable Server and DataBase";
			Text_b_onDart="On DaRT";
			Text_m_options="Settings ...";
			Text_m_about="About ...";
			Text_m_remoteControl="Remote Control ...";
			Text_b_locale="Language";
			Text_b_language="Language of program will be changed after program restart.Language of program after restart: English.";
			Text_s_server="File server startup:";
			Text_s_apache="File Apache startup:";
			Text_s_mySql="File MySQL startup:";
			Text_s_example="Example: D:\\Games\\Day\\arma2oaserver.exe";
			
		}

			
		
}
