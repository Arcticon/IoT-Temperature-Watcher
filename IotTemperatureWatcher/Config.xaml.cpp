#pragma comment(lib,"Ws2_32.lib")

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

char sendbuffer[256];
char recvbuffer[256];
float floatBuffer = 0;
long rc = 0;

float temp1, temp2, temp3, temp4, temp5, temp6 = 0;

static int thresholdSensorTopHigh = 40;
static int thresholdSensorTopLow = 30;
static int thresholdSensorCenterHigh = 40;
static int thresholdSensorCenterLow = 30;
static int thresholdSensorBottomHigh = 40;
static int thresholdSensorBottomLow = 30;
int datacenter, room, rack = 0;


Platform::String^ errorString = "";

std::string sliderValToString(int val);

Config::Config()
{
	InitializeComponent();
	StoryboardText1->Begin();
	StoryboardText2->Begin();

	getThresholds();

	updateAll();

	TimeSpan period;
	period.Duration = ticksMultiplier * ticks; // 10,000,000 ticks per second -

	TimeSpan period2;
	period2.Duration = ticksMultiplier2 * ticks;

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

	/*ThreadPoolTimer ^ PeriodicTimer2 = ThreadPoolTimer::CreatePeriodicTimer(
		ref new TimerElapsedHandler([this](ThreadPoolTimer^ source)
	{
		Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::High,
			ref new Windows::UI::Core::DispatchedHandler([this]()
		{
			// 
			// UI components can be accessed within this scope.
			// 

			IotTemperatureWatcher::Config::cleanErrors();

		}));
	}), period);*/

	/*datacenter = getDataFromServer("getdc");
	room = getDataFromServer("getrm");
	rack = getDataFromServer("getrk");*/

	textBoxInfo->Text = _DATACENTER + datacenter + _ROOM + room + _RACK + rack;

}

Platform::String ^ IotTemperatureWatcher::Config::getStatus(float temp, int choice)
{
	if (choice == 1) {
		if (temp <= thresholdSensorTopLow) {
			return "optimal";
		}
		else if (temp > thresholdSensorTopLow && temp <= thresholdSensorTopHigh) {
			return "warning";
		}
		else if (temp > thresholdSensorTopHigh) {
			return "critical";
		}
	} else if (choice == 2) {
		if (temp <= thresholdSensorCenterLow) {
			return "optimal";
		}
		else if (temp > thresholdSensorCenterLow && temp <= thresholdSensorCenterHigh) {
			return "warning";
		}
		else if (temp > thresholdSensorCenterHigh) {
			return "critical";
		}
	} else if (choice == 3) {
		if (temp <= thresholdSensorBottomLow) {
			return "optimal";
		}
		else if (temp > thresholdSensorBottomLow && temp <= thresholdSensorBottomHigh) {
			return "warning";
		}
		else if (temp > thresholdSensorBottomHigh) {
			return "critical";
		}
	} else {
		if (temp <= thresholdSensorCenterLow) {
			return "optimal";
		}
		else if (temp > thresholdSensorCenterLow && temp <= thresholdSensorCenterHigh) {
			return "warning";
		}
		else if (temp > thresholdSensorCenterHigh) {
			return "critical";
		}
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
		if (!errorString->IsEmpty()) {
			errorString += "\n";
		}
		errorString += _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESOCKETFAILED + " --> check connection/arduino";
		textBoxErrors->Text = errorString;
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
		if (!errorString->IsEmpty()) {
			errorString += "\n";
		}
		errorString += _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGECONNECTFAILED + " --> check IP/PORT";
		textBoxErrors->Text = errorString;
		return;
	}


	memset(&sendbuffer, 0, sizeof sendbuffer);
	memset(&recvbuffer, 0, sizeof recvbuffer);

	const char *data = str.c_str();

	rc = send(sConnect, data, strlen(data), 0);
	if (rc == SOCKET_ERROR) {

		if (!errorString->IsEmpty()) {
			errorString += "\n";
		}
		errorString += _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENDFAILED;
		textBoxErrors->Text = errorString;
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
		if (!errorString->IsEmpty()) {
			errorString += "\n";
		}
		errorString += _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESOCKETFAILED + " --> check connection/arduino";
		textBoxErrors->Text = errorString;
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
		if (!errorString->IsEmpty()) {
			errorString += "\n";
		}
		errorString += _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGECONNECTFAILED + " --> check IP/PORT";
		textBoxErrors->Text = errorString;
		return;
	}


	memset(&sendbuffer, 0, sizeof sendbuffer);
	memset(&recvbuffer, 0, sizeof recvbuffer);

	//const char *data = f;

	rc = send(sConnect, (const char *)&i, sizeof(i), 0);
	if (rc == SOCKET_ERROR) {
		if (!errorString->IsEmpty()) {
			errorString += "\n";
		}
		errorString += _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENDFAILED;
		textBoxErrors->Text = errorString;
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
		if (!errorString->IsEmpty()) {
			errorString += "\n";
		}
		errorString += _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESOCKETFAILED + " --> check connection/arduino";
		textBoxErrors->Text = errorString;
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
		if (!errorString->IsEmpty()) {
			errorString += "\n";
		}
		errorString += _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGECONNECTFAILED + " --> check IP/PORT";
		textBoxErrors->Text = errorString;
		return 0;
	}


	memset(&sendbuffer, 0, sizeof sendbuffer);
	memset(&recvbuffer, 0, sizeof recvbuffer);

	const char *data = str.c_str();

	rc = send(sConnect, data, strlen(data), 0);
	if (rc == SOCKET_ERROR) {
		if (!errorString->IsEmpty()) {
			errorString += "\n";
		}
		errorString += _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENDFAILED;
		textBoxErrors->Text = errorString;
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
		if (!!errorString->IsEmpty()) {
			errorString += "\n";
		}
		errorString += _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESOCKETFAILED + " --> check connection/arduino";
		textBoxErrors->Text = errorString;
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
		if (!!errorString->IsEmpty()) {
			errorString += "\n";
		}
		errorString += _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGECONNECTFAILED + " --> check IP/PORT";
		textBoxErrors->Text = errorString;
		return "";
	}


	memset(&sendbuffer, 0, sizeof sendbuffer);
	memset(&recvbuffer, 0, sizeof recvbuffer);

	const char *data = str.c_str();

	rc = send(sConnect, data, strlen(data), 0);
	if (rc == SOCKET_ERROR) {
		if (!errorString->IsEmpty()) {
			errorString += "\n";
		}
		errorString += _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENDFAILED;
		textBoxErrors->Text = errorString;
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

void IotTemperatureWatcher::Config::openedPopupConfig(Platform::Object^ sender, Platform::Object^ e) {

	sliderThresholdSensorTopLower->Value = thresholdSensorTopLow;
	sliderThresholdSensorTopUpper->Value = thresholdSensorTopHigh;
	sliderThresholdSensorCenterLower->Value = thresholdSensorCenterLow;
	sliderThresholdSensorCenterUpper->Value = thresholdSensorCenterHigh;
	sliderThresholdSensorBottomLower->Value = thresholdSensorBottomLow;
	sliderThresholdSensorBottomUpper->Value = thresholdSensorBottomHigh;

	popupConfig->Visibility = Windows::UI::Xaml::Visibility::Visible;
}

void IotTemperatureWatcher::Config::closedPopupConfig(Platform::Object^ sender, Platform::Object^ e) {
	popupConfig->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
}

void IotTemperatureWatcher::Config::clickedButtonPopupConfigSave(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
	sendDataToServer("settstl:" + sliderValToString((int)sliderThresholdSensorTopLower->Value));
	Sleep(10);
	sendDataToServer("settsth:" + sliderValToString((int)sliderThresholdSensorTopUpper->Value));
	Sleep(10);
	sendDataToServer("settscl:" + sliderValToString((int)sliderThresholdSensorCenterLower->Value));
	Sleep(10);
	sendDataToServer("settsch:" + sliderValToString((int)sliderThresholdSensorCenterUpper->Value));
	Sleep(10);
	sendDataToServer("settsbl:" + sliderValToString((int)sliderThresholdSensorBottomLower->Value));
	Sleep(10);
	sendDataToServer("settsbh:" + sliderValToString((int)sliderThresholdSensorBottomUpper->Value));
	Sleep(10);
	sendDataToServer("slider1:" + sliderValToString((int)sliderUpperTemp1F1->Value));
	Sleep(10);
	sendDataToServer("slider2:" + sliderValToString((int)sliderUpperTemp1F2->Value));
	Sleep(10);
	sendDataToServer("slider3:" + sliderValToString((int)sliderLowerTemp1->Value));
	Sleep(10);
	sendDataToServer("slider4:" + sliderValToString((int)sliderUpperTemp2F1->Value));
	Sleep(10);
	sendDataToServer("slider5:" + sliderValToString((int)sliderUpperTemp2F2->Value));
	Sleep(10);
	sendDataToServer("slider6:" + sliderValToString((int)sliderLowerTemp2->Value));



	thresholdSensorTopHigh = (int)sliderThresholdSensorTopUpper->Value;
	thresholdSensorTopLow = (int)sliderThresholdSensorTopLower->Value; 
	thresholdSensorCenterHigh = (int)sliderThresholdSensorCenterUpper->Value;
	thresholdSensorCenterLow = (int)sliderThresholdSensorCenterLower->Value;
	thresholdSensorBottomHigh = (int)sliderThresholdSensorBottomUpper->Value;
	thresholdSensorBottomLow = (int)sliderThresholdSensorBottomLower->Value;

	updateAll();

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
		if (!errorString->IsEmpty()) {
			errorString += "\n";
		}
		errorString += _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENSOR1NOTCONNECTED;
		textBoxErrors->Text = errorString;
		return;
	}

	temp2 = getDataFromServer(_GETTEMP2);

	if (temp2 < 2) {
		if (!errorString->IsEmpty()) {
			errorString += "\n";
		}
		errorString += _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENSOR2NOTCONNECTED;
		textBoxErrors->Text = errorString;
		return;
	}

	temp3 = getDataFromServer(_GETTEMP3);

	if (temp3 < 2) {
		if (!errorString->IsEmpty()) {
			errorString += "\n";
		}
		errorString += _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENSOR3NOTCONNECTED;
		textBoxErrors->Text = errorString;
		return;
	}

	temp4 = getDataFromServer(_GETTEMP4);

	if (temp4 < 2) {
		if (!errorString->IsEmpty()) {
			errorString += "\n";
		}
		errorString += _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENSOR4NOTCONNECTED;
		textBoxErrors->Text = errorString;
		return;
	}

	temp5 = getDataFromServer(_GETTEMP5);

	if (temp5 < 2) {
		if (!errorString->IsEmpty()) {
			errorString += "\n";
		}
		errorString += _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENSOR5NOTCONNECTED;
		textBoxErrors->Text = errorString;
		return;
	}

	temp6 = getDataFromServer(_GETTEMP6);

	if (temp6 < 2) {
		if (!errorString->IsEmpty()) {
			errorString += "\n";
		}
		errorString += _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENSOR6NOTCONNECTED;
		textBoxErrors->Text = errorString;
		return;
	}

	textBoxSTL->Text = _TEMPSENSORSTL + temp1 + "°C - Status: " + getStatus(temp1, 1);
	textBoxSCL->Text = _TEMPSENSORSCL + temp2 + "°C - Status: " + getStatus(temp2, 2);
	textBoxSBL->Text = _TEMPSENSORSBL + temp3 + "°C - Status: " + getStatus(temp3, 3);
	textBoxSTR->Text = _TEMPSENSORSTR + temp4 + "°C - Status: " + getStatus(temp4, 1);
	textBoxSCR->Text = _TEMPSENSORSCR + temp5 + "°C - Status: " + getStatus(temp5, 2);
	textBoxSBR->Text = _TEMPSENSORSBR + temp6 + "°C - Status: " + getStatus(temp6, 3);

	/*textBlockTemperature1->Text = tmpFloat1 + "°C";
	textBlockTemperature2->Text = tmpFloat2 + "°C";*/

	std::string tmpStringV1 = getDataFromServerToString(_GETFAN1);
	std::string tmpStringV2 = getDataFromServerToString(_GETFAN2);

	if (tmpStringV1.compare(_F1ON) == 0) {
		textBoxUpperFan1->Text = _TEXTFAN1ON->ToString();
	}
	else {
		textBoxUpperFan1->Text = _TEXTFAN1OFF->ToString();
	}
	if (tmpStringV2.compare(_F2ON) == 0) {
		textBoxUpperFan2->Text = _TEXTFAN2ON->ToString();
	}
	else {
		textBoxUpperFan2->Text = _TEXTFAN2OFF->ToString();
	}

	textBoxThresholdUpperSensor->Text = _CURRTHRESHSENSTOP + _CURRTHRESHLOWER + thresholdSensorTopLow + _CURRTHRESHUPPER + thresholdSensorTopHigh;
	textBoxThresholdCenterSensor->Text = _CURRTHRESHSENSCEN + _CURRTHRESHLOWER + thresholdSensorCenterLow + _CURRTHRESHUPPER + thresholdSensorCenterHigh;
	textBoxThresholdLowerSensor->Text = _CURRTHRESHSENSBOT + _CURRTHRESHLOWER + thresholdSensorBottomLow + _CURRTHRESHUPPER + thresholdSensorBottomHigh;
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

std::string IotTemperatureWatcher::Config::sliderValToString(int val) {
	std::wstring stringW(val.ToString()->Begin());
	std::string sliderVal(stringW.begin(), stringW.end());
	return sliderVal;
}

void IotTemperatureWatcher::Config::cleanErrors() {
	errorString = "";
	textBoxErrors->Text = errorString;
}

void IotTemperatureWatcher::Config::getThresholds() {
	thresholdSensorTopHigh = getDataFromServer("gettsth");
	thresholdSensorTopLow = getDataFromServer("gettstl");
	thresholdSensorCenterHigh = getDataFromServer("gettsch");
	thresholdSensorCenterLow = getDataFromServer("gettscl");
	thresholdSensorBottomHigh = getDataFromServer("gettsbh");
	thresholdSensorBottomLow = getDataFromServer("gettsbl");
}