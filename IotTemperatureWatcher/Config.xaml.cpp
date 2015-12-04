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

	sliderUpperTemp1->Value = 34;
	sliderUpperTemp1V2->Value = 38;
	sliderLowerTemp1->Value = 30;
	sliderUpperTemp2->Value = 30;
	sliderUpperTemp2V1->Value = 32;
	sliderLowerTemp2->Value = 27;
	checkBoxAdvancedCustomMode->IsChecked = false;

	sendDataToServer("slider1:" + sliderValToString((int)sliderUpperTemp1->Value));
	sendDataToServer("slider2:" + sliderValToString((int)sliderLowerTemp1->Value));
	sendDataToServer("slider3:" + sliderValToString((int)sliderUpperTemp2->Value));
	sendDataToServer("slider4:" + sliderValToString((int)sliderLowerTemp2->Value));
	sendDataToServer("slider5:" + sliderValToString((int)sliderUpperTemp1V2->Value));
	sendDataToServer("slider6:" + sliderValToString((int)sliderUpperTemp2V1->Value));

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
	}
	else {
		sendDataToServer(_CUSTOMMODEOFF);
	}




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
}

void IotTemperatureWatcher::Config::toggledToggleSwitchCustomMode(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {

}

void IotTemperatureWatcher::Config::toggledToggleSwitchVentilator1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (toggleSwitchVentilator1->IsOn) {

		sendDataToServer(_SETVENT1ON);
	}
	else {

		sendDataToServer(_SETVENT1OFF);
	}
}

void IotTemperatureWatcher::Config::toggledToggleSwitchVentilator2(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (toggleSwitchVentilator2->IsOn) {

		sendDataToServer(_SETVENT2ON);
	}
	else {

		sendDataToServer(_SETVENT2OFF);
	}
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

void IotTemperatureWatcher::Config::valueChangedSliderUpperTemp1(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	sendDataToServer("slider1:" + sliderValToString((int)sliderUpperTemp1->Value));
	textBlockUpperTemp1->Text = _TEXTUPPERTEMP1 + (int)sliderUpperTemp1->Value;
	Sleep(slp);
}

void IotTemperatureWatcher::Config::valueChangedSliderUpperTemp1V2(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e) {
	sendDataToServer("slider5:" + sliderValToString((int)sliderUpperTemp1V2->Value));
	textBlockUpperTemp1V2->Text = _TEXTUPPERTEMP1V2 + (int)sliderUpperTemp1V2->Value;
	Sleep(slp);
}

void IotTemperatureWatcher::Config::valueChangedSliderLowerTemp1(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	if (sliderUpperTemp1->Value > sliderUpperTemp1V2->Value) {
		
		if (sliderLowerTemp1->Value >= sliderUpperTemp1V2->Value) {
			sliderLowerTemp1->Value = sliderUpperTemp1V2->Value;
		}
	}
	else if (sliderUpperTemp1->Value < sliderUpperTemp1V2->Value) {
		if (sliderLowerTemp1->Value >= sliderUpperTemp1->Value) {
			sliderLowerTemp1->Value = sliderUpperTemp1->Value;
		}
	}

	sendDataToServer("slider2:" + sliderValToString((int)sliderLowerTemp1->Value));
	textBlockLowerTemp1->Text = _TEXTLOWERTEMP1 + (int)sliderLowerTemp1->Value;
	Sleep(slp);
}

void IotTemperatureWatcher::Config::valueChangedSliderUpperTemp2(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	sendDataToServer("slider3:" + sliderValToString((int)sliderUpperTemp2->Value));
	textBlockUpperTemp2->Text = _TEXTUPPERTEMP2 + (int)sliderUpperTemp2->Value;
	Sleep(slp);
}

void IotTemperatureWatcher::Config::valueChangedSliderUpperTemp2V1(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e) {
	sendDataToServer("slider6:" + sliderValToString((int)sliderUpperTemp2V1->Value));
	textBlockUpperTemp2V1->Text = _TEXTUPPERTEMP2V1 + (int)sliderUpperTemp2V1->Value;
	Sleep(slp);
}

void IotTemperatureWatcher::Config::valueChangedSliderLowerTemp2(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	if (sliderUpperTemp2->Value > sliderUpperTemp2V1->Value) {
		if (sliderLowerTemp2->Value >= sliderUpperTemp2V1->Value) {
			sliderLowerTemp2->Value = sliderUpperTemp2V1->Value;
		}
	}
	else if (sliderUpperTemp2->Value < sliderUpperTemp2V1->Value) {
		if (sliderLowerTemp2->Value >= sliderUpperTemp2->Value) {
			sliderLowerTemp2->Value = sliderUpperTemp2->Value;
		}
	}

	sendDataToServer("slider4:" + sliderValToString((int)sliderLowerTemp2->Value));
	textBlockLowerTemp2->Text = _TEXTLOWERTEMP2 + (int)sliderLowerTemp2->Value;
	Sleep(slp);
}

std::string sliderValToString(int val) {
	std::wstring stringW(val.ToString()->Begin());
	std::string sliderVal(stringW.begin(), stringW.end());
	return sliderVal;
}

void IotTemperatureWatcher::Config::openedPopupTest(Platform::Object^ sender, Platform::Object^ e)
{
	popupTest->Visibility = Windows::UI::Xaml::Visibility::Visible;
}

void IotTemperatureWatcher::Config::closedPopupTest(Platform::Object^ sender, Platform::Object^ e)
{
	popupTest->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
}

void IotTemperatureWatcher::Config::clickedButtonPopupOk(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	popupTest->IsOpen = false;
}

void IotTemperatureWatcher::Config::checkedCheckBoxAdvancedCustomMode(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	sendDataToServer(_CUSTOMMODEOFF);

	sliderUpperTemp1->IsEnabled = true;
	sliderUpperTemp1V2->IsEnabled = true;
	sliderLowerTemp1->IsEnabled = true;
	sliderUpperTemp2->IsEnabled = true;
	sliderUpperTemp2V1->IsEnabled = true;
	sliderLowerTemp2->IsEnabled = true;

	toggleSwitchVentilator1->IsEnabled = false;
	toggleSwitchVentilator2->IsEnabled = false;

	textBlockUpperTemp1->Opacity = 1;
	textBlockUpperTemp1V2->Opacity = 1;
	textBlockLowerTemp1->Opacity = 1;
	textBlockUpperTemp2->Opacity = 1;
	textBlockUpperTemp2V1->Opacity = 1;
	textBlockLowerTemp2->Opacity = 1;

	checkBoxAdvancedCustomMode->IsChecked = true;
}

void IotTemperatureWatcher::Config::uncheckedCheckBoxAdvancedCustomMode(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
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

	sliderUpperTemp1->IsEnabled = false;
	sliderLowerTemp1->IsEnabled = false;
	sliderUpperTemp1V2->IsEnabled = false;
	sliderUpperTemp2->IsEnabled = false;
	sliderUpperTemp2V1->IsEnabled = false;
	sliderLowerTemp2->IsEnabled = false;

	toggleSwitchVentilator1->IsEnabled = true;
	toggleSwitchVentilator2->IsEnabled = true;

	textBlockUpperTemp1->Opacity = 0.4;
	textBlockUpperTemp1V2->Opacity = 0.4;
	textBlockLowerTemp1->Opacity = 0.4;
	textBlockUpperTemp2->Opacity = 0.4;
	textBlockUpperTemp2V1->Opacity = 0.4;
	textBlockLowerTemp2->Opacity = 0.4;

	checkBoxAdvancedCustomMode->IsChecked = false;
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

	if (!appBarToggleButtonSensorConfig->IsChecked) {
		appBarToggleButtonSensorConfig->IsChecked = false;
	}
	if (appBarToggleButtonThresholdConfig->IsChecked) {
		appBarToggleButtonThresholdConfig->IsChecked = false;
	}

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
}

void IotTemperatureWatcher::Config::uncheckedAppBarToggleButtonFanConfig(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	appBarToggleButtonFanConfig->Label = _TEXTAPPBARTOGGLEBUTTONENABLEFANCONFIG;
	sendDataToServer(_FANCONFIGOFF);
	
	toggleSwitchVentilator1->IsEnabled = false;
	toggleSwitchVentilator2->IsEnabled = false;
}

void IotTemperatureWatcher::Config::checkedAppBarToggleButtonSensorConfig(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	appBarToggleButtonSensorConfig->Label = _TEXTAPPBARTOGGLEBUTTONDISABLESENSORCONFIG;
	sendDataToServer(_SENSORCONFIGON);

	
}

void IotTemperatureWatcher::Config::uncheckedAppBarToggleButtonSensorConfig(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	appBarToggleButtonSensorConfig->Label = _TEXTAPPBARTOGGLEBUTTONENABLESENSORCONFIG;
	sendDataToServer(_SENSORCONFIGOFF);

	
}

void IotTemperatureWatcher::Config::checkedAppBarToggleButtonThresholdConfig(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	appBarToggleButtonThresholdConfig->Label = _TEXTAPPBARTOGGLEBUTTONDISABLETHRESHOLDCONFIG;
	sendDataToServer(_THRESHOLDCONFIGON);

	if (appBarToggleButtonFanConfig->IsChecked) {
		appBarToggleButtonFanConfig->IsChecked = false;
	}
	if (appBarToggleButtonSensorConfig->IsChecked) {
		appBarToggleButtonSensorConfig->IsChecked = false;
	}

	sliderUpperTemp1->IsEnabled = true;
	sliderLowerTemp1->IsEnabled = true;
	sliderUpperTemp2->IsEnabled = true;
	sliderLowerTemp2->IsEnabled = true;

	textBlockUpperTemp1->Opacity = 1;
	textBlockLowerTemp1->Opacity = 1;
	textBlockUpperTemp2->Opacity = 1;
	textBlockLowerTemp2->Opacity = 1;

	sliderUpperTemp1V2->IsEnabled = true;
	sliderUpperTemp2V1->IsEnabled = true;
	textBlockUpperTemp1V2->Opacity = 1;
	textBlockUpperTemp2V1->Opacity = 1;
}

void IotTemperatureWatcher::Config::uncheckedAppBarToggleButtonThresholdConfig(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	appBarToggleButtonThresholdConfig->Label = _TEXTAPPBARTOGGLEBUTTONENABLETHRESHOLDCONFIG;
	sendDataToServer(_THRESHOLDCONFIGOFF);

	sliderUpperTemp1->IsEnabled = false;
	sliderLowerTemp1->IsEnabled = false;
	sliderUpperTemp2->IsEnabled = false;
	sliderLowerTemp2->IsEnabled = false;

	textBlockUpperTemp1->Opacity = 0.4;
	textBlockLowerTemp1->Opacity = 0.4;
	textBlockUpperTemp2->Opacity = 0.4;
	textBlockLowerTemp2->Opacity = 0.4;

	sliderUpperTemp1V2->IsEnabled = false;
	sliderUpperTemp2V1->IsEnabled = false;
	textBlockUpperTemp1V2->Opacity = 0.4;
	textBlockUpperTemp2V1->Opacity = 0.4;
}

void IotTemperatureWatcher::Config::updateAll() {
	temp1 = getDataFromServer(_GETTEMP1);

	if (temp1 < 2) {
		if (!popupTest->IsOpen) {
			popupTest->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENSOR1NOTCONNECTED;
		}
		return;
	}

	temp2 = getDataFromServer(_GETTEMP2);

	if (temp2 < 2) {
		if (!popupTest->IsOpen) {
			popupTest->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENSOR2NOTCONNECTED;
		}
		return;
	}

	temp3 = getDataFromServer(_GETTEMP3);

	if (temp3 < 2) {
		if (!popupTest->IsOpen) {
			popupTest->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENSOR3NOTCONNECTED;
		}
		return;
	}

	temp4 = getDataFromServer(_GETTEMP4);

	if (temp4 < 2) {
		if (!popupTest->IsOpen) {
			popupTest->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENSOR4NOTCONNECTED;
		}
		return;
	}

	temp5 = getDataFromServer(_GETTEMP5);

	if (temp5 < 2) {
		if (!popupTest->IsOpen) {
			popupTest->IsOpen = true;
			textBlockPopupErrorMessage->Text = _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENSOR5NOTCONNECTED;
		}
		return;
	}

	temp6 = getDataFromServer(_GETTEMP6);

	if (temp6 < 2) {
		if (!popupTest->IsOpen) {
			popupTest->IsOpen = true;
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