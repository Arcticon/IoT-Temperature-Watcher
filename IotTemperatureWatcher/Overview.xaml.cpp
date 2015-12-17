#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib, "user32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Winsock2.h>
#include <Windows.h>
#include <thread>

#include <time.h>

#include "pch.h"
#include "wtypes.h"
#include "Config.xaml.h"
#include "Overview.xaml.h"

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

float avgTemp1 = 0;
float avgTemp2 = 0;
float avgTemp3 = 0;
float avgTemp4 = 0;
float avgTemp5 = 0;
float avgTemp6 = 0;

int thresholdSensorTopHigh = 40;
int thresholdSensorTopLow = 30;
int thresholdSensorCenterHigh = 40;
int thresholdSensorCenterLow = 30;
int thresholdSensorBottomHigh = 40;
int thresholdSensorBottomLow = 30;

Platform::String^ errorString2 = "";

Overview::Overview()
{
	InitializeComponent();
	StoryboardText1->Begin();
	StoryboardText2->Begin();
	updateAll();

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

void IotTemperatureWatcher::Overview::clickButtonRackConfig1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e){
	if (this->Frame != nullptr)
	{
		this->Frame->Navigate(Config::typeid);
	}
}

void  IotTemperatureWatcher::Overview::clickButtonmsgLogo(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
	StoryboardFlyout1->Begin();
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
		if (!errorString2->IsEmpty()) {
			errorString2 += "\n";
		}
		errorString2 += _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESOCKETFAILED + " --> check connection/arduino";
		textBoxRackErrors1->Text = errorString2;
		closesocket(sConnect);

		WSACleanup();
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
		if (!errorString2->IsEmpty()) {
			errorString2 += "\n";
		}
		errorString2 += _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGECONNECTFAILED + " --> check IP/PORT";
		textBoxRackErrors1->Text = errorString2;
		closesocket(sConnect);

		WSACleanup();
		return 0;
	}


	memset(&sendbuffer2, 0, sizeof sendbuffer2);
	memset(&recvbuffer2, 0, sizeof recvbuffer2);

	const char *data = str.c_str();

	rc2 = send(sConnect, data, strlen(data), 0);
	if (rc2 == SOCKET_ERROR) {
		if (!errorString2->IsEmpty()) {
			errorString2 += "\n";
		}
		errorString2 += _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENDFAILED;
		textBoxRackErrors1->Text = errorString2;
		closesocket(sConnect);

		WSACleanup();
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
		if (!errorString2->IsEmpty()) {
			errorString2 += "\n";
		}
		errorString2 += _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESOCKETFAILED + " --> check connection/arduino";
		textBoxRackErrors1->Text = errorString2;
		closesocket(sConnect);

		WSACleanup();
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
		if (!errorString2->IsEmpty()) {
			errorString2 += "\n";
		}
		errorString2 += _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGECONNECTFAILED + " --> check IP/PORT";
		textBoxRackErrors1->Text = errorString2;
		closesocket(sConnect);

		WSACleanup();
		return "";
	}


	memset(&sendbuffer2, 0, sizeof sendbuffer2);
	memset(&recvbuffer2, 0, sizeof recvbuffer2);

	const char *data = str.c_str();

	rc2 = send(sConnect, data, strlen(data), 0);
	if (rc2 == SOCKET_ERROR) {
		if (!errorString2->IsEmpty()) {
			errorString2 += "\n";
		}
		errorString2 += _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENDFAILED;
		textBoxRackErrors1->Text = errorString2;
		closesocket(sConnect);

		WSACleanup();
		return "";
	}
	rc2 = recv(sConnect, recvbuffer2, 256, 0);


	closesocket(sConnect);

	WSACleanup();

	std::string tmpString = std::string(recvbuffer2);

	return tmpString;
}

void IotTemperatureWatcher::Overview::updateAll() {
	avgTemp1 = getDataFromServer("dc01ro01ra01");
	/*avgTemp2 = getDataFromServer("dc01ro01ra02");
	avgTemp3 = getDataFromServer("dc01ro01ra03");
	avgTemp4 = getDataFromServer("dc01ro01ra04");
	avgTemp5 = getDataFromServer("dc01ro01ra05");
	avgTemp6 = getDataFromServer("dc01ro01ra06");*/

	textBlock1->Text = "Temp. Rack 1: " + avgTemp1 + " | Status: " + Config::getStatus(avgTemp1, 0) + " || " + "Temp. Rack 2: " + avgTemp2 + " | Status: " + Config::getStatus(avgTemp2, 0) + " || " + "Temp. Rack 3: " + avgTemp3 + " | Status: " + Config::getStatus(avgTemp3, 0) + " || " + "Temp. Rack 4: " + avgTemp4 + " | Status: " + Config::getStatus(avgTemp4, 0) + " || " + "Temp. Rack 5: " + avgTemp5 + " | Status: " + Config::getStatus(avgTemp5, 0) + " || " + "Temp. Rack 6: " + avgTemp6 + " | Status: " + Config::getStatus(avgTemp6, 0);
	textBlock2->Text = textBlock1->Text;
	textBoxRackInfo1->Text = "Datacenter: 01 | Room: 01 | Rack: 01\nTemperature: " + avgTemp1 + "\nStatus: " + Config::getStatus(avgTemp1, 0);
	/*textBoxRackInfo2->Text = "Datacenter: 01 | Room: 01 | Rack: 02\nTemperature: " + avgTemp2 + "\nStatus: " + getStatus(avgTemp2);
	textBoxRackInfo3->Text = "Datacenter: 01 | Room: 01 | Rack: 03\nTemperature: " + avgTemp3 + "\nStatus: " + getStatus(avgTemp3);
	textBoxRackInfo4->Text = "Datacenter: 01 | Room: 01 | Rack: 04\nTemperature: " + avgTemp4 + "\nStatus: " + getStatus(avgTemp4);
	textBoxRackInfo5->Text = "Datacenter: 01 | Room: 01 | Rack: 05\nTemperature: " + avgTemp5 + "\nStatus: " + getStatus(avgTemp5);
	textBoxRackInfo6->Text = "Datacenter: 01 | Room: 01 | Rack: 06\nTemperature: " + avgTemp6 + "\nStatus: " + getStatus(avgTemp6);*/
}

void IotTemperatureWatcher::Overview::getThresholds() {
	thresholdSensorTopHigh = getDataFromServer("gettsth");
	thresholdSensorTopLow = getDataFromServer("gettstl");
	thresholdSensorCenterHigh = getDataFromServer("gettsch");
	thresholdSensorCenterLow = getDataFromServer("gettscl");
	thresholdSensorBottomHigh = getDataFromServer("gettsbh");
	thresholdSensorBottomLow = getDataFromServer("gettsbl");
}