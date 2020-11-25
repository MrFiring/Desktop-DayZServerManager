#pragma once
#include <Windows.h>


class localization
{
public:
	localization(void);
	~localization(void);
	char* ReadIni(char* section,char* key,char* fileName);
	void LocaleInit();
	int LocaleKey;
	
	char*	Text_b_offServer;
	char*	Text_b_offDataBase;
	char*	Text_b_offServerAndDataBase;
	char*	Text_b_restartDataBase;
	char*	Text_b_restartServer;
	char*	Text_b_restartServerAndDataBase;
	char*	Text_b_onDataBase;
	char*	Text_b_onServer;
	char*	Text_b_onServerAndDataBase;
	char*	Text_b_onDart;
	char*	Text_m_options;
	char*	Text_m_about;
	char*	Text_m_remoteControl;
	char*	Text_b_russian;
	char*	Text_b_english;
	char*	Text_b_locale;
	char*	Text_b_language;
	char*	Text_s_server;
	char*	Text_s_apache;
	char*	Text_s_mySql;
	char*	Text_s_example;
	

};

