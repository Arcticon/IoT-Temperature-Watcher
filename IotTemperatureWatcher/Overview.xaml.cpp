#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib, "user32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Winsock2.h>
#include <Windows.h>
#include <thread>

#include <time.h>

#include "pch.h"
#include "Overview.xaml.h"
#include "Config.xaml.h"
#include "wtypes.h"

using namespace IotTemperatureWatcher;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::System::Threading;

char sendbuffer2[256];
char recvbuffer2[256];
float floatBuffer2 = 0;
long rc2 = 0;

float testtemp1 = 0;

Overview::Overview()
{
	InitializeComponent();

	TimeSpan period;
	period.Duration = ticksMultiplier2 * ticks2; // 10,000,000 ticks per second - 

	ThreadPoolTimer ^ PeriodicTimer = ThreadPoolTimer::CreatePeriodicTimer(
		ref new TimerElapsedHandler([this](ThreadPoolTimer^ source)
	{
		Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::High,
			ref new Windows::UI::Core::DispatchedHandler([this]()
		{
			// 
			// UI components can be accessed within this scope.
			// 

			IotTemperatureWatcher::Overview::updateAll();

		}));
	}), period);
}

void IotTemperatureWatcher::Overview::clickButtonRackConfig1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (this->Frame != nullptr)
	{
		this->Frame->Navigate(Config::typeid);
	}
}

float IotTemperatureWatcher::Overview::getDataFromServer(std::string str) {
	//
	//long rc2;

	//
	WSADATA wsaData;
	SOCKET sConnect;
	sockaddr_in conpar;

	//
	rc2 = WSAStartup(MAKEWORD(2, 0), &wsaData);
	if (rc2 != 0) {

	}

	sConnect = socket(AF_INET, SOCK_STREAM, 0);
	if (sConnect == INVALID_SOCKET) {
		//t->Text = "socket failed";
		textBoxRackInfo1->Text = "ERROR";
		/*if (!popupTest->IsOpen) {
			popupTest->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESOCKETFAILED + " --> check connection/arduino";
		}*/
		return 0;
	}

	//conpar.sin_addr.s_addr = inet_addr("127.0.0.1");
	conpar.sin_addr.s_addr = inet_addr(_ARDUINOIP.c_str());
	conpar.sin_family = AF_INET;
	//conpar.sin_port = htons(1337);
	conpar.sin_port = htons(_ETHERNETPORT);
	int conparlen = sizeof conpar;

	rc2 = connect(sConnect, (struct sockaddr*)&conpar, conparlen);
	if (rc2 == SOCKET_ERROR) {
		//t2->Text = "connect failed";
		textBoxRackInfo1->Text = "ERROR";
		/*if (!popupTest->IsOpen) {
			popupTest->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGECONNECTFAILED + " --> check IP/PORT";
		}*/
		return 0;
	}


	memset(&sendbuffer2, 0, sizeof sendbuffer2);
	memset(&recvbuffer2, 0, sizeof recvbuffer2);

	const char *data = str.c_str();

	rc2 = send(sConnect, data, strlen(data), 0);
	if (rc2 == SOCKET_ERROR) {

		//t3->Text = "error send";
		textBoxRackInfo1->Text = "ERROR";

		/*if (!popupTest->IsOpen) {
			popupTest->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENDFAILED;
		}*/
		return 0;
	}
	rc2 = recv(sConnect, recvbuffer2, 256, 0);


	closesocket(sConnect);

	WSACleanup();

	//float tmpFloat = atof(recvbuffer2);

	return atof(recvbuffer2);
}

std::string IotTemperatureWatcher::Overview::getDataFromServerToString(std::string str) {
	//
	long rc2;

	//
	WSADATA wsaData;
	SOCKET sConnect;
	sockaddr_in conpar;

	//
	rc2 = WSAStartup(MAKEWORD(2, 0), &wsaData);
	if (rc2 != 0) {

	}

	sConnect = socket(AF_INET, SOCK_STREAM, 0);
	if (sConnect == INVALID_SOCKET) {
		//t->Text = "socket failed";
		/*if (!popupTest->IsOpen) {
			popupTest->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESOCKETFAILED + " --> check connection/arduino";
		}*/
		return "";
	}

	//conpar.sin_addr.s_addr = inet_addr("127.0.0.1");
	conpar.sin_addr.s_addr = inet_addr(_ARDUINOIP.c_str());
	conpar.sin_family = AF_INET;
	//conpar.sin_port = htons(1337);
	conpar.sin_port = htons(_ETHERNETPORT);
	int conparlen = sizeof conpar;

	rc2 = connect(sConnect, (struct sockaddr*)&conpar, conparlen);
	if (rc2 == SOCKET_ERROR) {
		//t2->Text = "connect failed";
		/*if (!popupTest->IsOpen) {
			popupTest->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGECONNECTFAILED + " --> check IP/PORT";
		}*/
		return "";
	}


	memset(&sendbuffer2, 0, sizeof sendbuffer2);
	memset(&recvbuffer2, 0, sizeof recvbuffer2);

	const char *data = str.c_str();

	rc2 = send(sConnect, data, strlen(data), 0);
	if (rc2 == SOCKET_ERROR) {

		//t3->Text = "error send";

		/*if (!popupTest->IsOpen) {
			popupTest->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENDFAILED;
		}*/
		return "";
	}
	rc2 = recv(sConnect, recvbuffer2, 256, 0);


	closesocket(sConnect);

	WSACleanup();

	std::string tmpString = std::string(recvbuffer2);

	return tmpString;
}

void IotTemperatureWatcher::Overview::updateAll() {
	testtemp1 = getDataFromServer("dc01ro01ra01");
	textBoxRackInfo1->Text = "Datacenter: 01 | Room: 01 | Rack: 01\nTemperature: " + testtemp1;
}