#pragma comment(lib,"Ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Winsock2.h>
#include <Windows.h>
#include <thread>

#include <time.h>

#include "pch.h"
#include "Config.xaml.h"
#include "Overview.xaml.h"
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

char sendbuffer[256];
char recvbuffer[256];
float floatBuffer = 0;
long rc = 0;

int slp = 75; //75

float temp1, temp2, temp3, temp4, temp5, temp6 = 0;

std::string sliderValToString(int val);

Config::Config()
{
	InitializeComponent();

	updateAll();

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

			IotTemperatureWatcher::Config::updateAll();

		}));
	}), period);

	textBoxInfo->Text = _DATACENTER + _ROOM + _RACK;
	textBoxThresholdUpperSensor->Text = _CURRTHRESHSENSTOP;
	textBoxThresholdCenterSensor->Text = _CURRTHRESHSENSCEN;
	textBoxThresholdLowerSensor->Text = _CURRTHRESHSENSBOT;

}

void IotTemperatureWatcher::Config::sendDataToServer(std::string str) {
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
		if (!popupError->IsOpen) {
			popupError->IsOpen = true;
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
		if (!popupError->IsOpen) {
			popupError->IsOpen = true;
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

		if (!popupError->IsOpen) {
			popupError->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENDFAILED;
		}
		return;
	}

	closesocket(sConnect);

	WSACleanup();
}

void IotTemperatureWatcher::Config::sendDataToServerFromInt(int i) {
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
		if (!popupError->IsOpen) {
			popupError->IsOpen = true;
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
		if (!popupError->IsOpen) {
			popupError->IsOpen = true;
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

		if (!popupError->IsOpen) {
			popupError->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENDFAILED;
		}
		return;
	}

	closesocket(sConnect);

	WSACleanup();
}

float IotTemperatureWatcher::Config::getDataFromServer(std::string str) {
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
		if (!popupError->IsOpen) {
			popupError->IsOpen = true;
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
		if (!popupError->IsOpen) {
			popupError->IsOpen = true;
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

		if (!popupError->IsOpen) {
			popupError->IsOpen = true;
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

std::string IotTemperatureWatcher::Config::getDataFromServerToString(std::string str) {
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
		if (!popupError->IsOpen) {
			popupError->IsOpen = true;
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
		if (!popupError->IsOpen) {
			popupError->IsOpen = true;
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

		if (!popupError->IsOpen) {
			popupError->IsOpen = true;
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

std::string sliderValToString(int val) {
	std::wstring stringW(val.ToString()->Begin());
	std::string sliderVal(stringW.begin(), stringW.end());
	return sliderVal;
}

void IotTemperatureWatcher::Config::openedPopupError(Platform::Object^ sender, Platform::Object^ e)
{
	popupError->Visibility = Windows::UI::Xaml::Visibility::Visible;
}

void IotTemperatureWatcher::Config::closedPopupError(Platform::Object^ sender, Platform::Object^ e)
{
	popupError->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
}

void IotTemperatureWatcher::Config::openedPopupConfig(Platform::Object^ sender, Platform::Object^ e) {
	popupConfig->Visibility = Windows::UI::Xaml::Visibility::Visible;
}

void IotTemperatureWatcher::Config::closedPopupConfig(Platform::Object^ sender, Platform::Object^ e) {
	popupConfig->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
}

void IotTemperatureWatcher::Config::clickedButtonPopupErrorOk(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	popupError->IsOpen = false;
}

void IotTemperatureWatcher::Config::clickedButtonPopupConfigSave(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
	popupConfig->IsOpen = false;
}

void IotTemperatureWatcher::Config::checkedCheckBoxAdvancedCustomMode(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	sendDataToServer(_CUSTOMMODEOFF);
}

void IotTemperatureWatcher::Config::uncheckedCheckBoxAdvancedCustomMode(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	sendDataToServer(_CUSTOMMODEON);
}

void IotTemperatureWatcher::Config::clickAppBarButtonSave(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (this->Frame != nullptr)
	{
		this->Frame->Navigate(Overview::typeid);
	}
}

void IotTemperatureWatcher::Config::checkedAppBarToggleButtonFanConfig(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	appBarToggleButtonFanConfig->Label = _TEXTAPPBARTOGGLEBUTTONDISABLEFANCONFIG;
	sendDataToServer(_FANCONFIGON);
	
	buttonAddFan->IsEnabled = true;
	buttonRemoveFan->IsEnabled = true;
}

void IotTemperatureWatcher::Config::uncheckedAppBarToggleButtonFanConfig(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	appBarToggleButtonFanConfig->Label = _TEXTAPPBARTOGGLEBUTTONENABLEFANCONFIG;
	sendDataToServer(_FANCONFIGOFF);

	buttonAddFan->IsEnabled = false;
	buttonRemoveFan->IsEnabled = false;
}

void IotTemperatureWatcher::Config::checkedAppBarToggleButtonSensorConfig(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	appBarToggleButtonSensorConfig->Label = _TEXTAPPBARTOGGLEBUTTONDISABLESENSORCONFIG;
	sendDataToServer(_SENSORCONFIGON);

	buttonAddSensor->IsEnabled = true;
	buttonRemoveSensor->IsEnabled = true;
}

void IotTemperatureWatcher::Config::uncheckedAppBarToggleButtonSensorConfig(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	appBarToggleButtonSensorConfig->Label = _TEXTAPPBARTOGGLEBUTTONENABLESENSORCONFIG;
	sendDataToServer(_SENSORCONFIGOFF);

	buttonAddSensor->IsEnabled = false;
	buttonRemoveSensor->IsEnabled = false;
}

void IotTemperatureWatcher::Config::checkedAppBarToggleButtonThresholdConfig(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	appBarToggleButtonThresholdConfig->Label = _TEXTAPPBARTOGGLEBUTTONDISABLETHRESHOLDCONFIG;
	sendDataToServer(_THRESHOLDCONFIGON);
	
	buttonChangeThresholds->IsEnabled = true;
}

void IotTemperatureWatcher::Config::uncheckedAppBarToggleButtonThresholdConfig(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	appBarToggleButtonThresholdConfig->Label = _TEXTAPPBARTOGGLEBUTTONENABLETHRESHOLDCONFIG;
	sendDataToServer(_THRESHOLDCONFIGOFF);

	buttonChangeThresholds->IsEnabled = false;
}

void IotTemperatureWatcher::Config::updateAll() {
	temp1 = getDataFromServer(_GETTEMP1);

	if (temp1 < 2) {
		if (!popupError->IsOpen) {
			popupError->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENSOR1NOTCONNECTED;
		}
		return;
	}

	temp2 = getDataFromServer(_GETTEMP2);

	if (temp2 < 2) {
		if (!popupError->IsOpen) {
			popupError->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENSOR2NOTCONNECTED;
		}
		return;
	}

	temp3 = getDataFromServer(_GETTEMP3);

	if (temp3 < 2) {
		if (!popupError->IsOpen) {
			popupError->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENSOR3NOTCONNECTED;
		}
		return;
	}

	temp4 = getDataFromServer(_GETTEMP4);

	if (temp4 < 2) {
		if (!popupError->IsOpen) {
			popupError->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENSOR4NOTCONNECTED;
		}
		return;
	}

	temp5 = getDataFromServer(_GETTEMP5);

	if (temp5 < 2) {
		if (!popupError->IsOpen) {
			popupError->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENSOR5NOTCONNECTED;
		}
		return;
	}

	temp6 = getDataFromServer(_GETTEMP6);

	if (temp6 < 2) {
		if (!popupError->IsOpen) {
			popupError->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENSOR6NOTCONNECTED;
		}
		return;
	}

	textBoxSTL->Text = _TEMPSENSORSTL + temp1 + "°C - Status: ";
	textBoxSCL->Text = _TEMPSENSORSCL + temp2 + "°C - Status: ";
	textBoxSBL->Text = _TEMPSENSORSBL + temp3 + "°C - Status: ";
	textBoxSTR->Text = _TEMPSENSORSTR + temp4 + "°C - Status: ";
	textBoxSCR->Text = _TEMPSENSORSCR + temp5 + "°C - Status: ";
	textBoxSBR->Text = _TEMPSENSORSBR + temp6 + "°C - Status: ";

	/*textBlockTemperature1->Text = tmpFloat1 + "°C";
	textBlockTemperature2->Text = tmpFloat2 + "°C";*/

	std::string tmpStringV1 = getDataFromServerToString(_GETVENT1);
	std::string tmpStringV2 = getDataFromServerToString(_GETVENT2);

	/*if (tmpStringV1.compare(_V1ON) == 0) {
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
	}*/
}

void IotTemperatureWatcher::Config::clickButtonAddFan(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (!popupConfig->IsOpen) {
		popupConfig->IsOpen = true;
	}
}

void IotTemperatureWatcher::Config::clickButtonRemoveFan(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (!popupConfig->IsOpen) {
		popupConfig->IsOpen = true;
	}
}

void IotTemperatureWatcher::Config::clickButtonAddSensor(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (!popupConfig->IsOpen) {
		popupConfig->IsOpen = true;
	}
}

void IotTemperatureWatcher::Config::clickButtonRemoveSensor(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (!popupConfig->IsOpen) {
		popupConfig->IsOpen = true;
	}
}

void IotTemperatureWatcher::Config::clickButtonChangeThresholds(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (!popupConfig->IsOpen) {
		popupConfig->IsOpen = true;
	}
}