//
// MainPage.xaml.cpp
// Implementierung der MainPage-Klasse
//

#pragma comment(lib,"Ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include <Winsock2.h>
#include <Windows.h>
#include <thread>

#include <time.h>

#include "pch.h"
#include "MainPage.xaml.h"
#include "MainPage.g.h"

using namespace IotTemperatureWatcher;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::System::Threading;

// Die Vorlage "Leere Seite" ist unter http://go.microsoft.com/fwlink/?LinkId=402352clcid=0x409 dokumentiert.


char sendbuffer[256];
char recvbuffer[256];
float floatBuffer = 0;
long rc = 0;

void sendDataToServer(std::string str);

MainPage::MainPage()
{
	InitializeComponent();

	TimeSpan period;
	period.Duration = 10000000; // 10,000,000 ticks per second - 

	ThreadPoolTimer ^ PeriodicTimer = ThreadPoolTimer::CreatePeriodicTimer(
		ref new TimerElapsedHandler([this](ThreadPoolTimer^ source)
	{
		Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::High,
			ref new Windows::UI::Core::DispatchedHandler([this]()
		{
			// 
			// UI components can be accessed within this scope.
			// 

			IotTemperatureWatcher::MainPage::updateAll();

		}));
	}), period);
	

}


void IotTemperatureWatcher::MainPage::toggledToggleSwitchCustomMode(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	ToggleSwitch^ ts = (ToggleSwitch^)sender;
	ToggleSwitch^ tsv1 = toggleSwitchVentilator1;
	ToggleSwitch^ tsv2 = toggleSwitchVentilator2;
	if (ts->IsOn) {
		tsv1->IsEnabled = true;
		tsv2->IsEnabled = true;
		sendDataToServer(_CUSTOMMODEON);
		if (tsv1->IsOn) {
			sendDataToServer(_SETVENT1ON);
		}
		else {
			sendDataToServer(_SETVENT1OFF);
		}
		if (tsv2->IsOn) {
			sendDataToServer(_SETVENT2ON);
		}
		else {
			sendDataToServer(_SETVENT2OFF);
		}
	}
	else {
		tsv1->IsEnabled = false;
		tsv2->IsEnabled = false;
		sendDataToServer(_CUSTOMMODEOFF);
	}
}


void IotTemperatureWatcher::MainPage::toggledToggleSwitchVentilator1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	ToggleSwitch^ ts = (ToggleSwitch^)sender;
	TextBlock^ tbv1 = textBlockVentilator1;
	if (ts->IsOn) {
		tbv1->Text = _TEXTVENT1ON->ToString();
		sendDataToServer(_SETVENT1ON);
	}
	else {
		tbv1->Text = _TEXTVENT1OFF->ToString();
		sendDataToServer(_SETVENT1OFF);
	}
}


void IotTemperatureWatcher::MainPage::toggledToggleSwitchVentilator2(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	ToggleSwitch^ ts = (ToggleSwitch^)sender;
	TextBlock^ tbv2 = textBlockVentilator2;
	if (ts->IsOn) {
		tbv2->Text = _TEXTVENT2ON->ToString();
		sendDataToServer(_SETVENT2ON);
	}
	else {
		tbv2->Text = _TEXTVENT2OFF->ToString();
		sendDataToServer(_SETVENT2OFF);
	}
}

void IotTemperatureWatcher::MainPage::sendDataToServer(std::string str) {
	//
	long rc;

	//
	WSADATA wsaData;
	SOCKET sConnect;
	sockaddr_in conpar;

	//
	rc = WSAStartup(MAKEWORD(2, 0), &wsaData);
	if (rc != 0) {

	}

	sConnect = socket(AF_INET, SOCK_STREAM, 0);
	if (sConnect == INVALID_SOCKET) {
		//t->Text = "socket failed";
	}

	//conpar.sin_addr.s_addr = inet_addr("127.0.0.1");
	conpar.sin_addr.s_addr = inet_addr(_ARDUINOIP.c_str());
	conpar.sin_family = AF_INET;
	//conpar.sin_port = htons(1337);
	conpar.sin_port = htons(_ETHERNETPORT);
	int conparlen = sizeof conpar;

	rc = connect(sConnect, (struct sockaddr*)&conpar, conparlen);
	if (rc == SOCKET_ERROR) {
		//t2->Text = "connect failed";
	}


	memset(&sendbuffer, 0, sizeof sendbuffer);
	memset(&recvbuffer, 0, sizeof recvbuffer);

	const char *data = str.c_str();

	rc = send(sConnect, data, strlen(data), 0);
	if (rc == SOCKET_ERROR) {

		//t3->Text = "error send";

	}

	closesocket(sConnect);

	WSACleanup();
}

float IotTemperatureWatcher::MainPage::getDataFromServer(std::string str) {
	//
	//long rc;

	//
	WSADATA wsaData;
	SOCKET sConnect;
	sockaddr_in conpar;

	//
	rc = WSAStartup(MAKEWORD(2, 0), &wsaData);
	if (rc != 0) {

	}

	sConnect = socket(AF_INET, SOCK_STREAM, 0);
	if (sConnect == INVALID_SOCKET) {
		//t->Text = "socket failed";
	}

	//conpar.sin_addr.s_addr = inet_addr("127.0.0.1");
	conpar.sin_addr.s_addr = inet_addr(_ARDUINOIP.c_str());
	conpar.sin_family = AF_INET;
	//conpar.sin_port = htons(1337);
	conpar.sin_port = htons(_ETHERNETPORT);
	int conparlen = sizeof conpar;

	rc = connect(sConnect, (struct sockaddr*)&conpar, conparlen);
	if (rc == SOCKET_ERROR) {
		//t2->Text = "connect failed";
	}


	memset(&sendbuffer, 0, sizeof sendbuffer);
	memset(&recvbuffer, 0, sizeof recvbuffer);

	const char *data = str.c_str();

	rc = send(sConnect, data, strlen(data), 0);
	if (rc == SOCKET_ERROR) {

		//t3->Text = "error send";

	}
	rc = recv(sConnect, recvbuffer, 256, 0);


	closesocket(sConnect);

	WSACleanup();

	//float tmpFloat = atof(recvbuffer);

	return atof(recvbuffer);
}

std::string IotTemperatureWatcher::MainPage::getDataFromServerToString(std::string str) {
	//
	long rc;

	//
	WSADATA wsaData;
	SOCKET sConnect;
	sockaddr_in conpar;

	//
	rc = WSAStartup(MAKEWORD(2, 0), &wsaData);
	if (rc != 0) {

	}

	sConnect = socket(AF_INET, SOCK_STREAM, 0);
	if (sConnect == INVALID_SOCKET) {
		//t->Text = "socket failed";
	}

	//conpar.sin_addr.s_addr = inet_addr("127.0.0.1");
	conpar.sin_addr.s_addr = inet_addr(_ARDUINOIP.c_str());
	conpar.sin_family = AF_INET;
	//conpar.sin_port = htons(1337);
	conpar.sin_port = htons(_ETHERNETPORT);
	int conparlen = sizeof conpar;

	rc = connect(sConnect, (struct sockaddr*)&conpar, conparlen);
	if (rc == SOCKET_ERROR) {
		//t2->Text = "connect failed";
	}


	memset(&sendbuffer, 0, sizeof sendbuffer);
	memset(&recvbuffer, 0, sizeof recvbuffer);

	const char *data = str.c_str();

	rc = send(sConnect, data, strlen(data), 0);
	if (rc == SOCKET_ERROR) {

		//t3->Text = "error send";

	}
	rc = recv(sConnect, recvbuffer, 256, 0);


	closesocket(sConnect);

	WSACleanup();

	std::string tmpString = std::string(recvbuffer);

	return tmpString;
}

void IotTemperatureWatcher::MainPage::updateAll() {
	TextBlock^ tbt1 = textBlockTemperature1;
	TextBlock^ tbt2 = textBlockTemperature2;
	TextBlock^ tbv1 = textBlockVentilator1;
	TextBlock^ tbv2 = textBlockVentilator2;
	tbt1->Text = "Temperature 1: " + getDataFromServer(_GETTEMP1);
	tbt2->Text = "Temperature 2: " + getDataFromServer(_GETTEMP2);
	std::string tmpStringV1 = getDataFromServerToString(_GETVENT1);
	std::string tmpStringV2 = getDataFromServerToString(_GETVENT2);

	if (tmpStringV1.compare(_V1ON) == 0) {
		tbv1->Text = _TEXTVENT1ON->ToString();
	}
	else if (tmpStringV1.compare(_V1OFF) == 0) {
		tbv1->Text = _TEXTVENT1OFF->ToString();
	}
	if (tmpStringV2.compare(_V2ON) == 0) {
		tbv2->Text = _TEXTVENT2ON->ToString();
	}
	else if (tmpStringV2.compare(_V2OFF) == 0) {
		tbv2->Text = _TEXTVENT2OFF->ToString();
	}
}