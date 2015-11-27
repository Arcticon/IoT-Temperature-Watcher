﻿#pragma comment(lib,"Ws2_32.lib")
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
using namespace Windows::UI;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::System::Threading;

char sendbuffer[256];
char recvbuffer[256];
float floatBuffer = 0;
long rc = 0;
float tmpFloat1 = 0;
float tmpFloat2 = 0;

void sendDataToServer(std::string str);
std::string sliderValToString(int val);

MainPage::MainPage()
{
	InitializeComponent();

	TimeSpan period;
	period.Duration = ticksMultiplier * ticks; // 10,000,000 ticks per second - 

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

	sliderUpperTemp1->Value = 38;
	sliderLowerTemp1->Value = 30;
	sliderUpperTemp2->Value = 30;
	sliderLowerTemp2->Value = 27;
	checkBoxAdvancedCustomMode->IsChecked = false;
}

void IotTemperatureWatcher::MainPage::toggledToggleSwitchCustomMode(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (toggleSwitchCustomMode->IsOn) {
		sendDataToServer(_CUSTOMMODEON);

		if (toggleSwitchVentilator1->IsOn) {
			sendDataToServer(_SETVENT1ON);
		}
		else {
			sendDataToServer(_SETVENT1OFF);
		}
		if (toggleSwitchVentilator2->IsOn) {
			sendDataToServer(_SETVENT2ON);
		}
		else {
			sendDataToServer(_SETVENT2OFF);
		}

		toggleSwitchVentilator1->IsEnabled = true;
		toggleSwitchVentilator2->IsEnabled = true;
		checkBoxAdvancedCustomMode->IsEnabled = true;
	}
	else {
		textBlockUpperTemp1->Opacity = 0.4;
		textBlockLowerTemp1->Opacity = 0.4;
		textBlockUpperTemp2->Opacity = 0.4;
		textBlockLowerTemp2->Opacity = 0.4;

		sliderUpperTemp1->IsEnabled = false;
		sliderLowerTemp1->IsEnabled = false;
		sliderUpperTemp2->IsEnabled = false;
		sliderLowerTemp2->IsEnabled = false;
		checkBoxAdvancedCustomMode->IsEnabled = false;
		checkBoxAdvancedCustomMode->IsChecked = false;
		toggleSwitchVentilator1->IsEnabled = false;
		toggleSwitchVentilator2->IsEnabled = false;

		sendDataToServer(_CUSTOMMODEOFF);
	}
}

void IotTemperatureWatcher::MainPage::toggledToggleSwitchVentilator1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (toggleSwitchVentilator1->IsOn) {
		textBlockVentilator1->Text = _TEXTVENT1ON->ToString();
		sendDataToServer(_SETVENT1ON);
	}
	else {
		textBlockVentilator1->Text = _TEXTVENT1OFF->ToString();
		sendDataToServer(_SETVENT1OFF);
	}
}

void IotTemperatureWatcher::MainPage::toggledToggleSwitchVentilator2(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (toggleSwitchVentilator2->IsOn) {
		textBlockVentilator2->Text = _TEXTVENT2ON->ToString();
		sendDataToServer(_SETVENT2ON);
	}
	else {
		textBlockVentilator2->Text = _TEXTVENT2OFF->ToString();
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
		if (!popupTest->IsOpen) {
			popupTest->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESOCKETFAILED + " --> check connection/arduino";
		}
		return;
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
		if (!popupTest->IsOpen) {
			popupTest->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGECONNECTFAILED + " --> check IP/PORT";
		}
		return;
	}


	memset(&sendbuffer, 0, sizeof sendbuffer);
	memset(&recvbuffer, 0, sizeof recvbuffer);

	const char *data = str.c_str();

	rc = send(sConnect, data, strlen(data), 0);
	if (rc == SOCKET_ERROR) {

		//t3->Text = "error send";

		if (!popupTest->IsOpen) {
			popupTest->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENDFAILED;
		}
		return;
	}

	closesocket(sConnect);

	WSACleanup();
}

void IotTemperatureWatcher::MainPage::sendDataToServerFromInt(int i) {
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
		if (!popupTest->IsOpen) {
			popupTest->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESOCKETFAILED + " --> check connection/arduino";
		}
		return;
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
		if (!popupTest->IsOpen) {
			popupTest->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGECONNECTFAILED + " --> check IP/PORT";
		}
		return;
	}


	memset(&sendbuffer, 0, sizeof sendbuffer);
	memset(&recvbuffer, 0, sizeof recvbuffer);

	//const char *data = f;

	rc = send(sConnect, (const char *)&i, sizeof(i), 0);
	if (rc == SOCKET_ERROR) {

		//t3->Text = "error send";

		if (!popupTest->IsOpen) {
			popupTest->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENDFAILED;
		}
		return;
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
		if (!popupTest->IsOpen) {
			popupTest->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESOCKETFAILED + " --> check connection/arduino";
		}
		return 0;
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
		if (!popupTest->IsOpen) {
			popupTest->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGECONNECTFAILED + " --> check IP/PORT";
		}
		return 0;
	}


	memset(&sendbuffer, 0, sizeof sendbuffer);
	memset(&recvbuffer, 0, sizeof recvbuffer);

	const char *data = str.c_str();

	rc = send(sConnect, data, strlen(data), 0);
	if (rc == SOCKET_ERROR) {

		//t3->Text = "error send";

		if (!popupTest->IsOpen) {
			popupTest->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENDFAILED;
		}
		return 0;
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
		if (!popupTest->IsOpen) {
			popupTest->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESOCKETFAILED + " --> check connection/arduino";
		}
		return "";
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
		if (!popupTest->IsOpen) {
			popupTest->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGECONNECTFAILED + " --> check IP/PORT";
		}
		return "";
	}


	memset(&sendbuffer, 0, sizeof sendbuffer);
	memset(&recvbuffer, 0, sizeof recvbuffer);

	const char *data = str.c_str();

	rc = send(sConnect, data, strlen(data), 0);
	if (rc == SOCKET_ERROR) {

		//t3->Text = "error send";

		if (!popupTest->IsOpen) {
			popupTest->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENDFAILED;
		}
		return "";
	}
	rc = recv(sConnect, recvbuffer, 256, 0);


	closesocket(sConnect);

	WSACleanup();

	std::string tmpString = std::string(recvbuffer);

	return tmpString;
}

void IotTemperatureWatcher::MainPage::updateAll() {
	tmpFloat1 = getDataFromServer(_GETTEMP1);

	if (tmpFloat1 < 2) {
		if (!popupTest->IsOpen) {
			popupTest->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENSOR1NOTCONNECTED;
		}
		return;
	}

	tmpFloat2 = getDataFromServer(_GETTEMP2);

	if (tmpFloat2 < 2) {
		if (!popupTest->IsOpen) {
			popupTest->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENSOR2NOTCONNECTED;
		}
		return;
	}

	textBlockTemperature1->Text = "Temperature 1: " + tmpFloat1;
	textBlockTemperature2->Text = "Temperature 2: " + tmpFloat2;
	std::string tmpStringV1 = getDataFromServerToString(_GETVENT1);
	std::string tmpStringV2 = getDataFromServerToString(_GETVENT2);	

	if (tmpStringV1.compare(_V1ON) == 0) {
		textBlockVentilator1->Text = _TEXTVENT1ON->ToString();
	}
	else if (tmpStringV1.compare(_V1OFF) == 0) {
		textBlockVentilator1->Text = _TEXTVENT1OFF->ToString();
	}
	if (tmpStringV2.compare(_V2ON) == 0) {
		textBlockVentilator2->Text = _TEXTVENT2ON->ToString();
	}
	else if (tmpStringV2.compare(_V2OFF) == 0) {
		textBlockVentilator2->Text = _TEXTVENT2OFF->ToString();
	}

	progressBar1->Value = tmpFloat1;
	progressBar2->Value = tmpFloat2;

}

void IotTemperatureWatcher::MainPage::valueChangedSliderUpperTemp1(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	sendDataToServer("slider1:" + sliderValToString((int)sliderUpperTemp1->Value));
	textBlockUpperTemp1->Text = _TEXTUPPERTEMP1 + (int)sliderUpperTemp1->Value;
}

void IotTemperatureWatcher::MainPage::valueChangedSliderLowerTemp1(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	if (sliderLowerTemp1->Value >= sliderUpperTemp1->Value) {
		sliderLowerTemp1->Value = sliderUpperTemp1->Value;
	}

	sendDataToServer("slider2:" + sliderValToString((int)sliderLowerTemp1->Value));
	textBlockLowerTemp1->Text = _TEXTLOWERTEMP1 + (int)sliderLowerTemp1->Value;
}

void IotTemperatureWatcher::MainPage::valueChangedSliderUpperTemp2(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	sendDataToServer("slider3:" + sliderValToString((int)sliderUpperTemp2->Value));
	textBlockUpperTemp2->Text = _TEXTUPPERTEMP2 + (int)sliderUpperTemp2->Value;
}

void IotTemperatureWatcher::MainPage::valueChangedSliderLowerTemp2(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	if (sliderLowerTemp2->Value >= sliderUpperTemp2->Value) {
		sliderLowerTemp2->Value = sliderUpperTemp2->Value;
	}

	sendDataToServer("slider4:" + sliderValToString((int)sliderLowerTemp2->Value));
	textBlockLowerTemp2->Text = _TEXTLOWERTEMP2 + (int)sliderLowerTemp2->Value;
}

std::string sliderValToString(int val) {
	std::wstring stringW(val.ToString()->Begin());
	std::string sliderVal(stringW.begin(), stringW.end());
	return sliderVal;
}

void IotTemperatureWatcher::MainPage::openedPopupTest(Platform::Object^ sender, Platform::Object^ e)
{
	popupTest->Visibility = Windows::UI::Xaml::Visibility::Visible;
}

void IotTemperatureWatcher::MainPage::closedPopupTest(Platform::Object^ sender, Platform::Object^ e)
{
	popupTest->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
}

void IotTemperatureWatcher::MainPage::clickedButtonPopupOk(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	popupTest->IsOpen = false;
}

void IotTemperatureWatcher::MainPage::checkedCheckBoxAdvancedCustomMode(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	sendDataToServer(_CUSTOMMODEOFF);

	sliderUpperTemp1->IsEnabled = true;
	sliderLowerTemp1->IsEnabled = true;
	sliderUpperTemp2->IsEnabled = true;
	sliderLowerTemp2->IsEnabled = true;

	textBlockUpperTemp1->Opacity = 1;
	textBlockLowerTemp1->Opacity = 1;
	textBlockUpperTemp2->Opacity = 1;
	textBlockLowerTemp2->Opacity = 1;

	checkBoxAdvancedCustomMode->IsChecked = true;
}

void IotTemperatureWatcher::MainPage::uncheckedCheckBoxAdvancedCustomMode(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	sendDataToServer(_CUSTOMMODEON);

	sliderUpperTemp1->IsEnabled = false;
	sliderLowerTemp1->IsEnabled = false;
	sliderUpperTemp2->IsEnabled = false;
	sliderLowerTemp2->IsEnabled = false;

	textBlockUpperTemp1->Opacity = 0.4;
	textBlockLowerTemp1->Opacity = 0.4;
	textBlockUpperTemp2->Opacity = 0.4;
	textBlockLowerTemp2->Opacity = 0.4;

	checkBoxAdvancedCustomMode->IsChecked = false;
}