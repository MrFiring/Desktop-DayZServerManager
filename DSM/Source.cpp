#include <WinSock2.h>
#include <Windows.h>
#include "resource1.h"
#include <TlHelp32.h>
#include "localization.h"
#include <string>
#include <regex>
#include <io.h>
#include <iostream>



/* Processes.
Server proc arma2oaserver.exe
Apache httpd.exe  2 need kills


*/

//--Methods Begin--
LONG WINAPI WndProc(HWND hwnd,UINT Msg,WPARAM wParam,LPARAM lParam);
INT_PTR CALLBACK Options(HWND hwndDlg,UINT Msg,WPARAM wParam,LPARAM lParam);
INT_PTR CALLBACK About(HWND hwndDlg,UINT Msg,WPARAM wParam,LPARAM lParam);
INT_PTR CALLBACK RemoteControl(HWND hwndDlg,UINT Msg,WPARAM wParam,LPARAM lParam);
void ChangeFont(HWND hwnd);
DWORD GetProcessByExeName(char *ExeName);
char* ReadIni(char* section,char* key,char* FileName);
void DataBaseOff();
void ServerOff();
void Server_On();
void DataBase_On();
void Server_And_DataBaseOn();
void ReceiveMsg(int ID);
void ConnectCheck();
//--Methods End--

//--Macros--
#define BUF (LPCSTR)buffer
#define ERR_MSG MB_OK|MB_ICONERROR

//--Menu Macros Begin--
#define MM_OPTIONS 1
#define MM_INFORMATION 2
#define MM_RC 3

//--Menu Macros End--

//--Buttons&Windows Begin--
#define OffDataBase 1001
#define OffServer 1002
#define BTN_SET_LANGUAGE 1003
#define BTN_OFFSERVERDATABASE 1004

#define BTN_ONSERVER 1005
#define BTN_ONDATABASE 1006
#define BTN_ONSERVERDATABASE 1007

#define BTN_RESTARTSERVER 1008
#define BTN_RESTARTDATABASE 1009
#define BTN_RESTARTSERVERDATABASE 1010


	WSAData wsa;

	SOCKET Listen;
	SOCKET Client;
	SOCKET *Client_List;

	SOCKADDR_IN server;
	SOCKADDR_IN client;
	int Client_Count;

HWND TestWindow;
HWND GeneralWindow;

HWND b_SetLanguage;

HWND b_OffDataBase;
HWND b_OffServer;
HWND b_OffServerDataBase;

HWND b_OnServer;
HWND b_OnDataBase;
HWND b_OnServerDataBase;

HWND b_RestartServer;
HWND b_RestartDataBase;
HWND b_RestartServerDataBase;

//--Buttons&Windows End--

//--Others Begin--
HINSTANCE hInst;
//--Others End--

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Client_List = (SOCKET*)calloc(64,sizeof(SOCKET));
	Client_Count = 0;

	localization loc;
	loc.LocaleInit();		

	MSG msg;
	
	WNDCLASSEX cl;
	cl.cbSize = sizeof(WNDCLASSEX);
	cl.hInstance = hInstance;
	cl.lpfnWndProc = WndProc;
	cl.cbClsExtra = 0;
	cl.cbWndExtra = 0;
	cl.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);//(HBRUSH)GetStockObject(WHITE_BRUSH);
	cl.hCursor = LoadCursor(hInstance,MAKEINTRESOURCE(IDC_ARROW));
	cl.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
	cl.hIconSm = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
	cl.lpszMenuName = NULL;
	cl.style = CS_HREDRAW | CS_VREDRAW;
	cl.lpszClassName = "GeneralClass";
	

	if(!RegisterClassEx(&cl))
	{
		MessageBox(NULL,"Call to RegisterClassEx(cl) failed!","ERROR", MB_ICONERROR | MB_OK);
		return -1;
	}
	
	

	GeneralWindow = CreateWindowEx(0,"GeneralClass","DayZ Server Manager", WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,100,200,400,305,NULL,NULL,hInstance,NULL);
	if(!GeneralWindow)
		MessageBox(NULL,"Call to CreateWindow GeneralWindow failed!","ERROR", MB_OK | MB_ICONERROR);
	
	//--GeneralWindow Buttons Begin--
	b_OffDataBase = CreateWindowEx(NULL,"Button",loc.Text_b_offDataBase,WS_CHILD | WS_VISIBLE,10,20,170,35,GeneralWindow,(HMENU)OffDataBase,hInstance,NULL);
	b_OffServer = CreateWindowEx(NULL,"Button",loc.Text_b_offServer,WS_CHILD | WS_VISIBLE,212,20,170,35,GeneralWindow,(HMENU)OffServer,hInstance,NULL);

	//b_SetLanguage = CreateWindowEx(NULL,"Button","Rus\\Eng",WS_CHILD,335,0,60,30,GeneralWindow,(HMENU)BTN_SET_LANGUAGE,hInstance,NULL);
	b_OffServerDataBase = CreateWindowEx(NULL,"Button",loc.Text_b_offServerAndDataBase,WS_CHILD | WS_VISIBLE,10,65,170,35,GeneralWindow,(HMENU)BTN_OFFSERVERDATABASE,hInstance,NULL);
	b_OnServer = CreateWindowEx(NULL,"Button",loc.Text_b_onServer,WS_CHILD | WS_VISIBLE,212,65,170,35,GeneralWindow,(HMENU)BTN_ONSERVER,hInstance,NULL);

	b_OnDataBase = CreateWindowEx(NULL,"Button",loc.Text_b_onDataBase,WS_CHILD | WS_VISIBLE,10,110,170,35,GeneralWindow,(HMENU)BTN_ONDATABASE,hInstance,NULL);
	b_OnServerDataBase = CreateWindowEx(NULL,"Button",loc.Text_b_onServerAndDataBase,WS_CHILD | WS_VISIBLE,212,110,170,35,GeneralWindow,(HMENU)BTN_ONSERVERDATABASE,hInstance,NULL);

	b_RestartServer = CreateWindowEx(NULL,"Button",loc.Text_b_restartServer,WS_CHILD | WS_VISIBLE,10,155,170,35,GeneralWindow,(HMENU)BTN_RESTARTSERVER,hInstance,NULL);
	b_RestartDataBase = CreateWindowEx(NULL,"Button",loc.Text_b_restartDataBase,WS_CHILD | WS_VISIBLE,212,155,170,35,GeneralWindow,(HMENU)BTN_RESTARTDATABASE,hInstance,NULL);

	b_RestartServerDataBase = CreateWindowEx(NULL,"Button",loc.Text_b_restartServerAndDataBase,WS_CHILD | WS_VISIBLE,111,200,170,35,GeneralWindow,(HMENU)BTN_RESTARTSERVERDATABASE,hInstance,NULL);
	//--GeneralWindow Buttons End--
	
	/*
	
	//--GeneralWindow Buttons Begin--
	b_OffDataBase = CreateWindowEx(NULL,"Button",loc.Text_b_offDataBase,WS_CHILD | WS_VISIBLE,10,20,170,50,GeneralWindow,(HMENU)OffDataBase,hInstance,NULL);
	b_OffServer = CreateWindowEx(NULL,"Button",loc.Text_b_offServer,WS_CHILD | WS_VISIBLE,212,20,170,50,GeneralWindow,(HMENU)OffServer,hInstance,NULL);

	//b_SetLanguage = CreateWindowEx(NULL,"Button","Rus\\Eng",WS_CHILD,335,0,60,30,GeneralWindow,(HMENU)BTN_SET_LANGUAGE,hInstance,NULL);
	b_OffServerDataBase = CreateWindowEx(NULL,"Button",loc.Text_b_offServerAndDataBase,WS_CHILD | WS_VISIBLE,10,90,170,50,GeneralWindow,(HMENU)BTN_OFFSERVERDATABASE,hInstance,NULL);

	b_OnServer = CreateWindowEx(NULL,"Button",loc.Text_b_onServer,WS_CHILD | WS_VISIBLE,212,90,170,50,GeneralWindow,(HMENU)BTN_ONSERVER,hInstance,NULL);
	b_OnDataBase = CreateWindowEx(NULL,"Button",loc.Text_b_onDataBase,WS_CHILD | WS_VISIBLE,10,160,170,50,GeneralWindow,(HMENU)BTN_ONDATABASE,hInstance,NULL);
	b_OnServerDataBase = CreateWindowEx(NULL,"Button",loc.Text_b_onServerAndDataBase,WS_CHILD | WS_VISIBLE,212,160,170,50,GeneralWindow,(HMENU)BTN_ONSERVERDATABASE,hInstance,NULL);

	b_RestartServer = CreateWindowEx(NULL,"Button",loc.Text_b_restartServer,WS_CHILD | WS_VISIBLE,10,230,170,50,GeneralWindow,(HMENU)BTN_RESTARTSERVER,hInstance,NULL);
	b_RestartDataBase = CreateWindowEx(NULL,"Button",loc.Text_b_restartDataBase,WS_CHILD | WS_VISIBLE,212,230,170,50,GeneralWindow,(HMENU)BTN_RESTARTDATABASE,hInstance,NULL);
	b_RestartServerDataBase = CreateWindowEx(NULL,"Button",loc.Text_b_restartServerAndDataBase,WS_CHILD | WS_VISIBLE,10,300,170,50,GeneralWindow,(HMENU)BTN_RESTARTSERVERDATABASE,hInstance,NULL);
	//--GeneralWindow Buttons End--

	*/




	//--Change Font Begin--
	ChangeFont(GeneralWindow);

	ChangeFont(b_OffDataBase);
	ChangeFont(b_OffServer);
	ChangeFont(b_OffServerDataBase);

	ChangeFont(b_SetLanguage);

	ChangeFont(b_OnServer);
	ChangeFont(b_OnDataBase);
	ChangeFont(b_OnServerDataBase);

	ChangeFont(b_RestartServer);
	ChangeFont(b_RestartDataBase);
	ChangeFont(b_RestartServerDataBase);
	//--Change Font End--

	//--General Menu Begin--

	HMENU hMainMenu;
	hMainMenu = CreateMenu();
	
	AppendMenu(hMainMenu,MF_STRING,MM_OPTIONS,loc.Text_m_options);
	AppendMenu(hMainMenu,MF_STRING,MM_INFORMATION,loc.Text_m_about);
	AppendMenu(hMainMenu,MF_STRING,MM_RC,loc.Text_m_remoteControl);
	//--General Menu End--

	char buffer[512] = "\0";
	char path[20] = "ini";
	int lenOfBuffer = 0;
	GetModuleFileName(NULL,buffer,512);
	
	lenOfBuffer = strlen(buffer);
	buffer[lenOfBuffer - 3] = '\0';
					
	strcat_s(buffer,path);




	if(GetPrivateProfileInt("RemoteControl","Enabled",NULL,BUF) == 1)
	{
		if (AllocConsole()) 
		{ 
		int hCrt = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), 4); 
		
		*stdout = *(::_fdopen(hCrt, "w")); 
		::setvbuf(stdout, NULL, _IONBF, 0); 
		*stderr = *(::_fdopen(hCrt, "w")); 
		::setvbuf(stderr, NULL, _IONBF, 0);
		std::ios::sync_with_stdio();
		}
		std::cout << "[Server] Server running...Please Wait!" << std::endl;
	


	if(WSAStartup(MAKEWORD(2,2),&wsa) != 0)
		std::cout << "[Server] Startup Failed!" << std::endl;

	Listen = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(Listen == SOCKET_ERROR)
		std::cout << "[Server] Listen create failed!" << std::endl;

	

	 //GetPrivateProfileInt("RemoteControl","Port",NULL,BUF)

	server.sin_family = AF_INET;
	server.sin_port = htons(2715);
	server.sin_addr.s_addr = INADDR_ANY;

	

	if(bind(Listen,(sockaddr*)&server,sizeof(server)) != 0)
		std::cout << "[Server] Bind failed!" << std::endl;

	if(listen(Listen,SOMAXCONN) != 0)
		std::cout << "[Server] Listen failed!" << std::endl;

	std::cout << "[Server] Server started and wait for connection." << std::endl;
	CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)ConnectCheck,NULL,NULL,NULL);
	
	}
	
	SetMenu(GeneralWindow,hMainMenu);

	ShowWindow(GeneralWindow,nCmdShow);
	UpdateWindow(GeneralWindow);

	ShowWindow(TestWindow,nCmdShow);
	UpdateWindow(TestWindow);
	
	//MessageBox(NULL,loc.Text_b_offDataBase,"",NULL);
		
	hInst = hInstance;

		while(GetMessage(&msg,NULL,0,0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	return msg.wParam;
}

void ConnectCheck()
{
	while(true)
	{
		
		int Size = sizeof(client);
		if(Client = accept(Listen,(sockaddr*)&client,&Size))
		{
			Client_List[Client_Count] = Client;
			Client_Count++;
			std::cout << "[ Clients count: " << Client_Count << ']' << "[Server] Client Connected IP: " << inet_ntoa(client.sin_addr) << std::endl;
			
			CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)ReceiveMsg,(LPVOID)(Client_Count-1),NULL,NULL);
			
		}

	}
}

void ReceiveMsg(int ID)
{
	char* buf = new char[50];
	for(;;Sleep(75))
	{
		ZeroMemory(buf,sizeof(buf));
		if(recv(Client_List[ID],buf,50,0))
			if(buf != NULL)
			std::cout << buf << std::endl;
		
		if(strcmp(buf,"Off_Database") == 0)
			DataBaseOff();
		if(strcmp(buf,"Off_Server") == 0)
			ServerOff();
		if(strcmp(buf,"Off_Server_And_Database") == 0)
		{
			ServerOff();
			DataBaseOff();
		}
		if(strcmp(buf,"On_Server") == 0)
			Server_On();
		if(strcmp(buf,"On_Database") == 0)
			DataBase_On();
		if(strcmp(buf,"On_Server_And_Database") == 0)
		{
			DataBase_On();
			Server_On();
		}
		if(strcmp(buf,"Restart_Server") == 0)
		{
			ServerOff();
			Sleep(500);
			Server_On();
		}
		if(strcmp(buf,"Restart_Server_And_Database") == 0)
		{
			ServerOff();
			Sleep(75);
			DataBaseOff();
			Sleep(75);
			DataBase_On();
			Sleep(75);
			Server_On();
		}
		if(strcmp(buf,"Restart_Database") == 0)
		{
			DataBaseOff();
			Sleep(75);
			DataBase_On();
		}

	}
	delete [] buf;
}


LONG WINAPI WndProc(HWND hwnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	localization loc;
	loc.LocaleInit();
	
	switch(Msg)
	{
	case WM_CREATE:
		{
			char buffer[512] = "\0";
			char path[20] = "ini";
			int lenOfBuffer = 0;
			GetModuleFileName(NULL,buffer,512);
	
			lenOfBuffer = strlen(buffer);
			buffer[lenOfBuffer - 3] = '\0';
					
			strcat_s(buffer,path);

			if(_access(buffer,0))
			{
				MessageBox(NULL,"Settings file is not found.I'll open settings window.","Error",MB_OK|MB_ICONASTERISK);
				DialogBox(hInst,MAKEINTRESOURCE(IDD_DIALOG_OPTIONS),GeneralWindow,(DLGPROC)Options);
			}
			
		}
		break;

	case WM_COMMAND:
			switch(wParam)
			{
			case MM_OPTIONS:
				{
					DialogBox(hInst,MAKEINTRESOURCE(IDD_DIALOG_OPTIONS),GeneralWindow,(DLGPROC)Options);
				}
				break;
			case MM_INFORMATION:
				{
					DialogBox(hInst,MAKEINTRESOURCE(IDD_DIALOG_ABOUT),GeneralWindow,(DLGPROC)About);
				}
				break;
			case MM_RC:
				{
					DialogBox(hInst,MAKEINTRESOURCE(IDD_RC),GeneralWindow,(DLGPROC)RemoteControl);
				}
				break;
			case BTN_SET_LANGUAGE:
				{
					MessageBox(NULL,loc.Text_b_language,loc.Text_b_locale,MB_OK|MB_ICONINFORMATION);
					char buffer[512] = "\0";
					char path[20] = "localization.ini";
					int lenOfBuffer = 0;
					GetModuleFileName(NULL,buffer,512);
					lenOfBuffer = strlen(buffer);
					buffer[lenOfBuffer - 22] = '\0';
					
					strcat_s(buffer,path);
					if(loc.LocaleKey == 1)
						WritePrivateProfileString("locale","language","2",buffer);
					else if(loc.LocaleKey == 2)
						WritePrivateProfileString("locale","language","1",buffer);
				
					char CurDir[512] = "\0";
					int lenOfCurDir = 0;
					GetModuleFileName(NULL,CurDir,512);
					lenOfCurDir = strlen(CurDir);
					CurDir[lenOfCurDir - 22] = '\0';
 
					PostQuitMessage(0);
				}
				break;			

			case OffDataBase:
				{
					DataBaseOff();	
				}
				break;
			case OffServer:
				{
					ServerOff();
				}
				break;
			case BTN_OFFSERVERDATABASE:
				{
					ServerOff();
					Sleep(2000);
					DataBaseOff();
				}
				break;
			case BTN_ONSERVER:
				{
					Server_On();
				}
				break;
			case BTN_ONDATABASE:
				{
					DataBase_On();
				}
				break;
			case BTN_ONSERVERDATABASE:
				{
					Server_And_DataBaseOn();
				}
				break;
			case BTN_RESTARTSERVER:
				{
					ServerOff();
					Sleep(500);
					Server_On();
				}
				break;
			case BTN_RESTARTDATABASE:
				{
					DataBaseOff();
					Sleep(500);
					DataBase_On();
				}
				break;
			case BTN_RESTARTSERVERDATABASE:
				{
					ServerOff();
					Sleep(250);
					DataBaseOff();
					Sleep(250);
					Server_And_DataBaseOn();
				}
				break;
			}
		break;
	case WM_DESTROY:
		free(Client_List);
		WSACleanup();
			PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd,Msg,wParam,lParam);
		break;
	}
	return 0;
}

INT_PTR CALLBACK Options(HWND hwndDlg,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	localization loc;
	loc.LocaleInit();

	switch(Msg)
	{
	case WM_INITDIALOG:
		{
			char buffer[512] = "\0";
			char path[20] = "ini";
			int lenOfBuffer = 0;
			GetModuleFileName(NULL,buffer,512);
	
			lenOfBuffer = strlen(buffer);
			buffer[lenOfBuffer - 3] = '\0';
					
			strcat_s(buffer,path);
			char* Out = new char[512];
			if(!_access(buffer,0))
			{
				GetPrivateProfileString("Settings","ServerPath",NULL,Out,512,BUF);
				SetDlgItemText(hwndDlg,IDC_SERVER,Out);

				GetPrivateProfileString("Settings","MySqlPath",NULL,Out,512,BUF);
				SetDlgItemText(hwndDlg,IDC_MYSQL,Out);

				GetPrivateProfileString("Settings","ApachePath",NULL,Out,512,BUF);
				SetDlgItemText(hwndDlg,IDC_APACHE,Out);
			}
			delete [] Out;
		}
	break;
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case IDOK:
					{
					char buffer[512] = "\0";
					char path[20] = "ini";
					int lenOfBuffer = 0;
					GetModuleFileName(NULL,buffer,512);
	
					lenOfBuffer = strlen(buffer);
					buffer[lenOfBuffer - 3] = '\0';
					
					strcat_s(buffer,path);
					char* Out = new char[512];

						GetDlgItemText(hwndDlg,IDC_SERVER,Out,512);
						WritePrivateProfileString("Settings","ServerPath",Out,BUF);
					
						GetDlgItemText(hwndDlg,IDC_MYSQL,Out,512);
						WritePrivateProfileString("Settings","MySqlPath",Out,BUF);
					
						GetDlgItemText(hwndDlg,IDC_APACHE,Out,512);
						WritePrivateProfileString("Settings","ApachePath",Out,BUF);
					
					EndDialog(hwndDlg,wParam);
					delete [] Out;
					return (INT_PTR)TRUE;
					}
				break;

				case IDCANCEL:
					{
						EndDialog(hwndDlg,wParam);
						return (INT_PTR)TRUE;
					}
				break;
			
			}
		}
	break;
	
	}

	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK About(HWND hwndDlg,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	switch(Msg)
	{
	case WM_COMMAND:
		EndDialog(hwndDlg,wParam);
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK RemoteControl(HWND hwndDlg,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	switch(Msg)
	{
	case WM_INITDIALOG:
		{

			char buffer[512] = "\0";
			char path[20] = "ini";
			int lenOfBuffer = 0;
			GetModuleFileName(NULL,buffer,512);
	
			lenOfBuffer = strlen(buffer);
			buffer[lenOfBuffer - 3] = '\0';
					
			strcat_s(buffer,path);
			
			char* Out = new char[512];
			int Enabl =	GetPrivateProfileInt("RemoteControl","Enabled",NULL,BUF);

			if(Enabl == 1)
			{
				SetDlgItemText(hwndDlg,IDC_CHECK_ENABLED,"Disable");
			}
			

			if(!_access(buffer,0))
			{
				GetPrivateProfileString("RemoteControl","Username",NULL,Out,512,BUF);
				SetDlgItemText(hwndDlg,IDC_USERNAME,Out);

				GetPrivateProfileString("RemoteControl","Password",NULL,Out,512,BUF);
				SetDlgItemText(hwndDlg,IDC_PASSWORD,Out);
				
				GetPrivateProfileString("RemoteControl","Port",NULL,Out,512,BUF);
				SetDlgItemText(hwndDlg,IDC_PORT,Out);
			}
			delete [] Out;

		break;
		}
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDCANCEL:
				{
					EndDialog(hwndDlg,wParam);
					break;
				}
			case IDOK:
				{
					char buffer[512] = "\0";
					char path[20] = "ini";
					int lenOfBuffer = 0;
					GetModuleFileName(NULL,buffer,512);
	
					lenOfBuffer = strlen(buffer);
					buffer[lenOfBuffer - 3] = '\0';
					
					strcat_s(buffer,path);
					char* Out = new char[512];

						
						
						GetDlgItemText(hwndDlg,IDC_USERNAME,Out,512);
						WritePrivateProfileString("RemoteControl","Username",Out,BUF);
					
						GetDlgItemText(hwndDlg,IDC_PASSWORD,Out,512);
						WritePrivateProfileString("RemoteControl","Password",Out,BUF);
					
						GetDlgItemText(hwndDlg,IDC_PORT,Out,512);
						WritePrivateProfileString("RemoteControl","Port",Out,BUF);
					
					EndDialog(hwndDlg,wParam);
					delete [] Out;
					return (INT_PTR)TRUE;
					break;
				}
			case IDC_CHECK_ENABLED:
				{
					char buffer[512] = "\0";
					char path[20] = "ini";
					int lenOfBuffer = 0;
					GetModuleFileName(NULL,buffer,512);
	
					lenOfBuffer = strlen(buffer);
					buffer[lenOfBuffer - 3] = '\0';
					
					strcat_s(buffer,path);

					int Enb = GetPrivateProfileInt("RemoteControl","Enabled",NULL,BUF);
					if(Enb == 1)
					{
						SetDlgItemText(hwndDlg,IDC_CHECK_ENABLED,"Enable");
						WritePrivateProfileString("RemoteControl","Enabled","0",BUF);
					}
					else if(Enb == 0)
					{
						SetDlgItemText(hwndDlg,IDC_CHECK_ENABLED,"Disable");
						WritePrivateProfileString("RemoteControl","Enabled","1",BUF);
					}

				break;
				}
			}

			break;
		}
	}
	return(INT_PTR)FALSE;
}

void ChangeFont(HWND hwnd)
{
	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
 
	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
 
	GetVersionEx(&osvi);
 
	if(osvi.dwMajorVersion < 6)
		ncm.cbSize -= sizeof(ncm.iPaddedBorderWidth);
 
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);
 
	HFONT hFont = CreateFontIndirect(&ncm.lfMessageFont);
	SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));

}

DWORD GetProcessByExeName(char *ExeName)
{
//        DWORD Pid;
 
    PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);
 
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
    if( hProcessSnap == INVALID_HANDLE_VALUE)
    {
        MessageBox(NULL, "Error = " + GetLastError() ,"Error (GetProcessByExeName)", MB_OK|MB_ICONERROR);
        return false;
    }
 
    if ( Process32First(hProcessSnap, &pe32) )
        {
            do
             {
				 char* p32 = pe32.szExeFile;
                   if (_strcmpi(p32, ExeName) == 0)
                        {
                                CloseHandle(hProcessSnap);
                                return pe32.th32ProcessID;
                        }
             } while ( Process32Next(hProcessSnap, &pe32) );
        }
 
    CloseHandle(hProcessSnap);
        return 0;
}

char* ReadIni(char* section,char* key,char* FileName)
{
	char* OutData = new char[512];

	GetPrivateProfileString((LPCSTR)section,(LPCSTR)key,NULL,OutData,512,(LPCSTR)FileName);

	return OutData;
	delete []OutData;
}

void DataBaseOff()
{
	DWORD PID = GetProcessByExeName("mysqld.exe");
	if ( !PID ) MessageBox(NULL,"Process MySqld.exe not is found.I am looking for an alternative process.","ERROR",ERR_MSG);
	else
	{
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, PID );
		if (!TerminateProcess(hProcess, 0) ) 
			MessageBox(NULL,"Call to terminate MySqld.exe failed.","ERROR",ERR_MSG);
		CloseHandle(hProcess);
	}

	if(!PID)
	{
		DWORD PID = GetProcessByExeName("mysql.exe");
		if(!PID) MessageBox(NULL,(LPCSTR)"Process MySql.exe is not found","ERROR",ERR_MSG);
		else
		{
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, PID );
			if (!TerminateProcess(hProcess, 0) ) 
				MessageBox(NULL,"Call to terminate MySql.exe failed","ERROR",ERR_MSG);
			
			CloseHandle(hProcess);
		}
	}

	DWORD PIDApache = GetProcessByExeName("httpd.exe");
	if ( !PIDApache ) 
		MessageBox(NULL,"Apache process is not found.","ERROR",ERR_MSG);
	else
	{
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, PIDApache );
		if (!TerminateProcess(hProcess, 0) ) 
			MessageBox(NULL,"Call to terminate Apache failed.","ERROR",ERR_MSG);
		
		CloseHandle(hProcess);
	}
	
	DWORD PIDApache2 = GetProcessByExeName("httpd.exe");
	if ( !PIDApache2 )NULL;
	else
	{
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, PIDApache2 );
		if (!TerminateProcess(hProcess, 0) ) 
			MessageBox(NULL,"Call to terminate Apache failed.","ERROR",ERR_MSG);
		
		CloseHandle(hProcess);
	}
	
}

void ServerOff()
{
	DWORD PID = GetProcessByExeName("arma2oaserver.exe");
	if ( !PID ) MessageBox(NULL,"Server process is not found.","",NULL);
	else
	{
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, PID );
		if (!TerminateProcess(hProcess, 0) ) 
			MessageBox(NULL,"Call to terminate Server failed!","",NULL);
		CloseHandle(hProcess);
	}
}

void Server_On()
{
	char buffer[512] = "\0";
	char path[20] = "ini";
	int lenOfBuffer = 0;
	GetModuleFileName(NULL,buffer,512);
	
	lenOfBuffer = strlen(buffer);
	buffer[lenOfBuffer - 3] = '\0';
					
	strcat_s(buffer,path);

	char* Out = new char[512];

	GetPrivateProfileString("Settings","ServerPath",NULL,Out,512,BUF);
	//for(int i = 0;i < 512;i++)
		//if(OutData[i] == '\\')
			//OutData[i] = '0';

	STARTUPINFO si;
	ZeroMemory(&si,sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;
	
		if(!CreateProcess(Out,NULL,NULL,NULL,FALSE,NULL,NULL,NULL,&si,&pi))
			MessageBox(NULL,"Call to Server_On failed!","ERROR",MB_OK | MB_ICONERROR);
		

	delete [] Out;
}

void DataBase_On()
{
		char buffer[512] = "\0";
	char path[20] = "ini";
	int lenOfBuffer = 0;
	GetModuleFileName(NULL,buffer,512);
	
	lenOfBuffer = strlen(buffer);
	buffer[lenOfBuffer - 3] = '\0';
					
	strcat_s(buffer,path);

	char* Out = new char[512];

	GetPrivateProfileString("Settings","ApachePath",NULL,Out,512,BUF);
	//for(int i = 0;i < 512;i++)
		//if(OutData[i] == '\\')
			//OutData[i] = '0';

	STARTUPINFO si;
	ZeroMemory(&si,sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;
	
		if(!CreateProcess(Out,NULL,NULL,NULL,FALSE,NULL,NULL,NULL,&si,&pi))
			MessageBox(NULL,"Call to Apache_On failed!","ERROR",MB_OK | MB_ICONERROR);

	GetPrivateProfileString("Settings","MySqlPath",NULL,Out,512,BUF);
	//for(int i = 0;i < 512;i++)
		//if(OutData[i] == '\\')
			//OutData[i] = '0';

	STARTUPINFO si2;
	ZeroMemory(&si2,sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi2;
	
		if(!CreateProcess(Out,NULL,NULL,NULL,FALSE,NULL,NULL,NULL,&si2,&pi2))
			MessageBox(NULL,"Call to MySql_On failed!","ERROR",MB_OK | MB_ICONERROR);
		

	delete [] Out;
}

void Server_And_DataBaseOn()
{
	DataBase_On();
	Sleep(1000);
	Server_On();
}


/*

if(wParam == MM_OPTIONS)
		{
			ShowWindow(TestWindow,1);
			UpdateWindow(TestWindow);
		}
				

		if(wParam == OffDataBase)
		{
		DWORD PID = GetProcessByExeName("mysqld.exe");
					if ( !PID ) MessageBox(NULL,"Процесс не найден.Ищу альтернативный процесс.","",NULL);
					else
					{
						HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, PID );
						if ( TerminateProcess(hProcess, 0) ) 
							MessageBox(NULL,"Процесс убит","",NULL);
							else MessageBox(NULL,"Не могу убить процесс","",NULL);
							CloseHandle(hProcess);
					}

					char buffer[512] = "\0";
					char path[20] = "ins.ini";
					int lenOfBuffer = 0;
					GetModuleFileName(NULL,buffer,512);
					//lenAEN = strlen(ApplicationExeName);
					lenOfBuffer = strlen(buffer);
					buffer[lenOfBuffer - 7] = '\0';
					
					strcat_s(buffer,path);


					WritePrivateProfileString((LPCSTR)"testsect",(LPCSTR)"testapp",(LPCSTR)"tttt",(LPCSTR)buffer);
					if(!PID)
					{
						DWORD PID = GetProcessByExeName("mysql.exe");
						if(!PID) MessageBox(NULL,"Альтернативный процесс не найден.","",NULL);
						else
						{
						HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, PID );
						if ( TerminateProcess(hProcess, 0) ) 
							MessageBox(NULL,"Процесс убит","",NULL);
							else MessageBox(NULL,"Не могу убить процесс","",NULL);
							CloseHandle(hProcess);
						}
					}

					DWORD PIDApache = GetProcessByExeName("httpd.exe ");
					if ( !PIDApache ) MessageBox(NULL,"Процесс Apache не найден.","",NULL);
					else
					{
						HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, PIDApache );
						if ( TerminateProcess(hProcess, 0) ) 
							MessageBox(NULL,"Процесс Apache 1 убит","",NULL);
							else MessageBox(NULL,"Не могу убить процесс Apache 2","",NULL);
							CloseHandle(hProcess);
					}

					DWORD PIDApache2 = GetProcessByExeName("httpd.exe");
					if ( !PIDApache2 ) MessageBox(NULL,"Процесс Apache не найден.","",NULL);
					else
					{
						HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, PIDApache2 );
						if ( TerminateProcess(hProcess, 0) ) 
							MessageBox(NULL,"Процесс Apache 2 убит","",NULL);
							else MessageBox(NULL,"Не могу убить процесс Apache 2","",NULL);
							CloseHandle(hProcess);
					}
				
		}
		
		if(wParam == OffServer)
		{
		DWORD PID = GetProcessByExeName("arma2oaserver.exe");
					if ( !PID ) MessageBox(NULL,"Процесс не найден.","",NULL);
					else
					{
						HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, PID );
						if ( TerminateProcess(hProcess, 0) ) 
							MessageBox(NULL,"Процесс убит.","",NULL);
							else MessageBox(NULL,"Не могу убить процесс.","",NULL);
							CloseHandle(hProcess);
					}
		}


*/
