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

bool isFan1Enabled = true, isFan2Enabled = true;
bool isSensor1Enabled = true, isSensor2Enabled = true, isSensor3Enabled = true, isSensor4Enabled = true, isSensor5Enabled = true, isSensor6Enabled = true;

int state = 0;

static int thresholdSensorTopHigh = 40;
static int thresholdSensorTopLow = 30;
static int thresholdSensorCenterHigh = 40;
static int thresholdSensorCenterLow = 30;
static int thresholdSensorBottomHigh = 40;
static int thresholdSensorBottomLow = 30;
static int upperTemp1F1 = 34;
static int upperTemp1F2 = 38;
static int lowerTemp1 = 30;
static int upperTemp2F1 = 32;
static int upperTemp2F2 = 34;
static int lowerTemp2 = 27;

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
	if (state == 1) {
		//Visible

		comboBoxFan->Visibility = Windows::UI::Xaml::Visibility::Visible;
		if (isFan1Enabled) {
			comboBoxItemFan1->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		}
		else {
			comboBoxItemFan1->Visibility = Windows::UI::Xaml::Visibility::Visible;
		}
		if (isFan2Enabled) {
			comboBoxItemFan2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		}
		else {
			comboBoxItemFan2->Visibility = Windows::UI::Xaml::Visibility::Visible;
		}

		//Invisible

		comboBoxSensor->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockThresholdSensorTopLower->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockThresholdSensorTopUpper->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockThresholdSensorCenterLower->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockThresholdSensorCenterUpper->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockThresholdSensorBottomLower->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockThresholdSensorBottomUpper->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

		sliderThresholdSensorTopLower->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderThresholdSensorTopUpper->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderThresholdSensorCenterLower->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderThresholdSensorCenterUpper->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderThresholdSensorBottomLower->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderThresholdSensorBottomUpper->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

		textBlockUpper1Fan1->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockUpper1Fan2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockLower1->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

		textBlockUpper2Fan1->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockUpper2Fan2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockLower2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

		sliderUpperTemp1F1->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderUpperTemp1F2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderLowerTemp1->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderUpperTemp2F1->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderUpperTemp2F2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderLowerTemp2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	}
	else if (state == 2) {
		//Visible

		comboBoxFan->Visibility = Windows::UI::Xaml::Visibility::Visible;
		if (isFan1Enabled) {
			comboBoxItemFan1->Visibility = Windows::UI::Xaml::Visibility::Visible;
		}
		else {
			comboBoxItemFan1->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		}
		if (isFan2Enabled) {
			comboBoxItemFan2->Visibility = Windows::UI::Xaml::Visibility::Visible;
		}
		else {
			comboBoxItemFan2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		}

		//Invisible

		comboBoxSensor->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockThresholdSensorTopLower->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockThresholdSensorTopUpper->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockThresholdSensorCenterLower->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockThresholdSensorCenterUpper->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockThresholdSensorBottomLower->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockThresholdSensorBottomUpper->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

		sliderThresholdSensorTopLower->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderThresholdSensorTopUpper->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderThresholdSensorCenterLower->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderThresholdSensorCenterUpper->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderThresholdSensorBottomLower->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderThresholdSensorBottomUpper->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

		textBlockUpper1Fan1->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockUpper1Fan2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockLower1->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

		textBlockUpper2Fan1->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockUpper2Fan2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockLower2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

		sliderUpperTemp1F1->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderUpperTemp1F2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderLowerTemp1->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderUpperTemp2F1->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderUpperTemp2F2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderLowerTemp2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	}
	else if (state == 3) {
		//Visible

		comboBoxSensor->Visibility = Windows::UI::Xaml::Visibility::Visible;
		if (isSensor1Enabled) {
			comboBoxItemSensor1->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		}
		else {
			comboBoxItemSensor1->Visibility = Windows::UI::Xaml::Visibility::Visible;
		}
		if (isSensor2Enabled) {
			comboBoxItemSensor2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		}
		else {
			comboBoxItemSensor2->Visibility = Windows::UI::Xaml::Visibility::Visible;
		}
		if (isSensor3Enabled) {
			comboBoxItemSensor3->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		}
		else {
			comboBoxItemSensor3->Visibility = Windows::UI::Xaml::Visibility::Visible;
		}
		if (isSensor4Enabled) {
			comboBoxItemSensor4->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		}
		else {
			comboBoxItemSensor4->Visibility = Windows::UI::Xaml::Visibility::Visible;
		}
		if (isSensor5Enabled) {
			comboBoxItemSensor5->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		}
		else {
			comboBoxItemSensor5->Visibility = Windows::UI::Xaml::Visibility::Visible;
		}
		if (isSensor6Enabled) {
			comboBoxItemSensor6->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		}
		else {
			comboBoxItemSensor6->Visibility = Windows::UI::Xaml::Visibility::Visible;
		}

		//Invisible

		comboBoxFan->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockThresholdSensorTopLower->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockThresholdSensorTopUpper->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockThresholdSensorCenterLower->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockThresholdSensorCenterUpper->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockThresholdSensorBottomLower->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockThresholdSensorBottomUpper->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

		sliderThresholdSensorTopLower->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderThresholdSensorTopUpper->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderThresholdSensorCenterLower->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderThresholdSensorCenterUpper->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderThresholdSensorBottomLower->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderThresholdSensorBottomUpper->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

		textBlockUpper1Fan1->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockUpper1Fan2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockLower1->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

		textBlockUpper2Fan1->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockUpper2Fan2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockLower2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

		sliderUpperTemp1F1->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderUpperTemp1F2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderLowerTemp1->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderUpperTemp2F1->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderUpperTemp2F2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderLowerTemp2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	}
	else if (state == 4) {
		//Visible

		comboBoxSensor->Visibility = Windows::UI::Xaml::Visibility::Visible;
		if (isSensor1Enabled) {
			comboBoxItemSensor1->Visibility = Windows::UI::Xaml::Visibility::Visible;
		}
		else {
			comboBoxItemSensor1->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		}
		if (isSensor2Enabled) {
			comboBoxItemSensor2->Visibility = Windows::UI::Xaml::Visibility::Visible;
		}
		else {
			comboBoxItemSensor2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		}
		if (isSensor3Enabled) {
			comboBoxItemSensor3->Visibility = Windows::UI::Xaml::Visibility::Visible;
		}
		else {
			comboBoxItemSensor3->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		}
		if (isSensor4Enabled) {
			comboBoxItemSensor4->Visibility = Windows::UI::Xaml::Visibility::Visible;
		}
		else {
			comboBoxItemSensor4->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		}
		if (isSensor5Enabled) {
			comboBoxItemSensor5->Visibility = Windows::UI::Xaml::Visibility::Visible;
		}
		else {
			comboBoxItemSensor5->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		}
		if (isSensor6Enabled) {
			comboBoxItemSensor6->Visibility = Windows::UI::Xaml::Visibility::Visible;
		}
		else {
			comboBoxItemSensor6->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		}

		//Invisible

		comboBoxFan->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockThresholdSensorTopLower->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockThresholdSensorTopUpper->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockThresholdSensorCenterLower->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockThresholdSensorCenterUpper->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockThresholdSensorBottomLower->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockThresholdSensorBottomUpper->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

		sliderThresholdSensorTopLower->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderThresholdSensorTopUpper->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderThresholdSensorCenterLower->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderThresholdSensorCenterUpper->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderThresholdSensorBottomLower->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderThresholdSensorBottomUpper->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

		textBlockUpper1Fan1->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockUpper1Fan2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockLower1->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

		textBlockUpper2Fan1->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockUpper2Fan2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		textBlockLower2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

		sliderUpperTemp1F1->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderUpperTemp1F2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderLowerTemp1->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderUpperTemp2F1->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderUpperTemp2F2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		sliderLowerTemp2->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	}
	else if (state == 5) {
		//Visible


		textBlockThresholdSensorTopLower->Visibility = Windows::UI::Xaml::Visibility::Visible;
		textBlockThresholdSensorTopUpper->Visibility = Windows::UI::Xaml::Visibility::Visible;
		textBlockThresholdSensorCenterLower->Visibility = Windows::UI::Xaml::Visibility::Visible;
		textBlockThresholdSensorCenterUpper->Visibility = Windows::UI::Xaml::Visibility::Visible;
		textBlockThresholdSensorBottomLower->Visibility = Windows::UI::Xaml::Visibility::Visible;
		textBlockThresholdSensorBottomUpper->Visibility = Windows::UI::Xaml::Visibility::Visible;

		sliderThresholdSensorTopLower->Visibility = Windows::UI::Xaml::Visibility::Visible;
		sliderThresholdSensorTopUpper->Visibility = Windows::UI::Xaml::Visibility::Visible;
		sliderThresholdSensorCenterLower->Visibility = Windows::UI::Xaml::Visibility::Visible;
		sliderThresholdSensorCenterUpper->Visibility = Windows::UI::Xaml::Visibility::Visible;
		sliderThresholdSensorBottomLower->Visibility = Windows::UI::Xaml::Visibility::Visible;
		sliderThresholdSensorBottomUpper->Visibility = Windows::UI::Xaml::Visibility::Visible;

		textBlockUpper1Fan1->Visibility = Windows::UI::Xaml::Visibility::Visible;
		textBlockUpper1Fan2->Visibility = Windows::UI::Xaml::Visibility::Visible;
		textBlockLower1->Visibility = Windows::UI::Xaml::Visibility::Visible;

		textBlockUpper2Fan1->Visibility = Windows::UI::Xaml::Visibility::Visible;
		textBlockUpper2Fan2->Visibility = Windows::UI::Xaml::Visibility::Visible;
		textBlockLower2->Visibility = Windows::UI::Xaml::Visibility::Visible;

		sliderUpperTemp1F1->Visibility = Windows::UI::Xaml::Visibility::Visible;
		sliderUpperTemp1F2->Visibility = Windows::UI::Xaml::Visibility::Visible;
		sliderLowerTemp1->Visibility = Windows::UI::Xaml::Visibility::Visible;
		sliderUpperTemp2F1->Visibility = Windows::UI::Xaml::Visibility::Visible;
		sliderUpperTemp2F2->Visibility = Windows::UI::Xaml::Visibility::Visible;
		sliderLowerTemp2->Visibility = Windows::UI::Xaml::Visibility::Visible;

		//Invisible

		comboBoxFan->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		comboBoxSensor->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

		//

		sliderThresholdSensorTopLower->Value = thresholdSensorTopLow;
		sliderThresholdSensorTopUpper->Value = thresholdSensorTopHigh;
		sliderThresholdSensorCenterLower->Value = thresholdSensorCenterLow;
		sliderThresholdSensorCenterUpper->Value = thresholdSensorCenterHigh;
		sliderThresholdSensorBottomLower->Value = thresholdSensorBottomLow;
		sliderThresholdSensorBottomUpper->Value = thresholdSensorBottomHigh;

		sliderUpperTemp1F1->Value = upperTemp1F1;
		sliderUpperTemp1F2->Value = upperTemp1F2;
		sliderLowerTemp1->Value = lowerTemp1;
		sliderUpperTemp2F1->Value = upperTemp2F1;
		sliderUpperTemp2F2->Value = upperTemp2F2;
		sliderLowerTemp2->Value = lowerTemp2;
	}
	
	popupConfigError->Text = "" + state;
	popupConfig->Visibility = Windows::UI::Xaml::Visibility::Visible;
}

void IotTemperatureWatcher::Config::closedPopupConfig(Platform::Object^ sender, Platform::Object^ e) {
	popupConfig->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	state = 0;
}

void IotTemperatureWatcher::Config::clickedButtonPopupConfigSave(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
	if (state == 1) {
		if (comboBoxFan->SelectedItem == comboBoxItemFan1) {
			sendDataToServer("addF1");
			isFan1Enabled = true;
			Sleep(10);
		}
		else if (comboBoxFan->SelectedItem == comboBoxItemFan2) {
			sendDataToServer("addF2");
			isFan2Enabled = true;
			Sleep(10);
		}
	}else if(state == 2){
		if (comboBoxFan->SelectedItem == comboBoxItemFan1) {
			sendDataToServer("remF1");
			isFan1Enabled = false;
			Sleep(10);
		}
		else if (comboBoxFan->SelectedItem == comboBoxItemFan2) {
			sendDataToServer("remF2");
			isFan1Enabled = false;
			Sleep(10);
		}
	}else if (state == 3) {
		if (comboBoxSensor->SelectedItem == comboBoxItemSensor1) {
			sendDataToServer("addS1");
			isSensor1Enabled = true;
			Sleep(10);
		}
		else if (comboBoxSensor->SelectedItem == comboBoxItemSensor2) {
			sendDataToServer("addS2");
			isSensor2Enabled = true;
			Sleep(10);
		}
		else if (comboBoxSensor->SelectedItem == comboBoxItemSensor3) {
			sendDataToServer("addS3");
			isSensor3Enabled = true;
			Sleep(10);
		}
		else if (comboBoxSensor->SelectedItem == comboBoxItemSensor4) {
			sendDataToServer("addS4");
			isSensor4Enabled = true;
			Sleep(10);
		}
		else if (comboBoxSensor->SelectedItem == comboBoxItemSensor5) {
			sendDataToServer("addS5");
			isSensor5Enabled = true;
			Sleep(10);
		}
		else if (comboBoxSensor->SelectedItem == comboBoxItemSensor6) {
			sendDataToServer("addS6");
			isSensor6Enabled = true;
			Sleep(10);
		}
	}else if (state == 4) {
		if (comboBoxSensor->SelectedItem == comboBoxItemSensor1) {
			sendDataToServer("remS1");
			isSensor1Enabled = false;
			Sleep(10);
			textBoxErrors->Text = "remS1";
		}
		else if (comboBoxSensor->SelectedItem == comboBoxItemSensor2) {
			sendDataToServer("remS2");
			isSensor2Enabled = false;
			Sleep(10);
		}
		else if (comboBoxSensor->SelectedItem == comboBoxItemSensor3) {
			sendDataToServer("remS3");
			isSensor3Enabled = false;
			Sleep(10);
		}
		else if (comboBoxSensor->SelectedItem == comboBoxItemSensor4) {
			sendDataToServer("remS4");
			isSensor4Enabled = false;
			Sleep(10);
		}
		else if (comboBoxSensor->SelectedItem == comboBoxItemSensor5) {
			sendDataToServer("remS5");
			isSensor5Enabled = false;
			Sleep(10);
		}
		else if (comboBoxSensor->SelectedItem == comboBoxItemSensor6) {
			sendDataToServer("remS6");
			isSensor6Enabled = false;
			Sleep(10);
		}
	}else if (state == 5) {
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

		upperTemp1F1 = (int)sliderUpperTemp1F1->Value;
		upperTemp1F2 = (int)sliderUpperTemp1F2->Value;
		lowerTemp1 = (int)sliderLowerTemp1->Value;
		upperTemp2F1 = (int)sliderUpperTemp2F1->Value;
		upperTemp2F2 = (int)sliderUpperTemp2F2->Value;
		lowerTemp2 = (int)sliderLowerTemp2->Value;
	}
	

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

	if (temp1 < 0) {
		if (!errorString->IsEmpty()) {
			errorString += "\n";
		}
		errorString += _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENSOR1NOTCONNECTED;
		textBoxErrors->Text = errorString;
		return;
	}

	temp2 = getDataFromServer(_GETTEMP2);

	if (temp2 < 0) {
		if (!errorString->IsEmpty()) {
			errorString += "\n";
		}
		errorString += _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENSOR2NOTCONNECTED;
		textBoxErrors->Text = errorString;
		return;
	}

	temp3 = getDataFromServer(_GETTEMP3);

	if (temp3 < 0) {
		if (!errorString->IsEmpty()) {
			errorString += "\n";
		}
		errorString += _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENSOR3NOTCONNECTED;
		textBoxErrors->Text = errorString;
		return;
	}

	temp4 = getDataFromServer(_GETTEMP4);

	if (temp4 < 0) {
		if (!errorString->IsEmpty()) {
			errorString += "\n";
		}
		errorString += _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENSOR4NOTCONNECTED;
		textBoxErrors->Text = errorString;
		return;
	}

	temp5 = getDataFromServer(_GETTEMP5);

	if (temp5 < 0) {
		if (!errorString->IsEmpty()) {
			errorString += "\n";
		}
		errorString += _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENSOR5NOTCONNECTED;
		textBoxErrors->Text = errorString;
		return;
	}

	temp6 = getDataFromServer(_GETTEMP6);

	if (temp6 < 0) {
		if (!errorString->IsEmpty()) {
			errorString += "\n";
		}
		errorString += _TEXTPOPUPERRORMESSAGE + _TEXTPOPUPERRORMESSAGESENSOR6NOTCONNECTED;
		textBoxErrors->Text = errorString;
		return;
	}

	if (temp1 == -1) {
		textBoxSTL->Text = _TEMPSENSORSTL + "NOT CONNECTED";
	}
	else {
		textBoxSTL->Text = _TEMPSENSORSTL + temp1 + "°C - Status: " + getStatus(temp1, 1);
	}
	if (temp2 == -1) {
		textBoxSCL->Text = _TEMPSENSORSCL + "NOT CONNECTED";
	}
	else {
		textBoxSCL->Text = _TEMPSENSORSCL + temp2 + "°C - Status: " + getStatus(temp2, 2);
	}
	if (temp3 == -1) {
		textBoxSBL->Text = _TEMPSENSORSBL + "NOT CONNECTED";
	}
	else {
		textBoxSBL->Text = _TEMPSENSORSBL + temp3 + "°C - Status: " + getStatus(temp3, 3);
	}
	if (temp4 == -1) {
		textBoxSTR->Text = _TEMPSENSORSTR + "NOT CONNECTED";
	}
	else {
		textBoxSTR->Text = _TEMPSENSORSTR + temp4 + "°C - Status: " + getStatus(temp4, 1);
	}
	if (temp5 == -1) {
		textBoxSCR->Text = _TEMPSENSORSCR + "NOT CONNECTED";
	}
	else {
		textBoxSCR->Text = _TEMPSENSORSCR + temp5 + "°C - Status: " + getStatus(temp5, 2);
	}
	if (temp6 == -1) {
		textBoxSBR->Text = _TEMPSENSORSBR + "NOT CONNECTED";
	}
	else {
		textBoxSBR->Text = _TEMPSENSORSBR + temp6 + "°C - Status: " + getStatus(temp6, 3);
	}	

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
		state = 1;

		popupConfig->IsOpen = true;
	}
}

void IotTemperatureWatcher::Config::clickButtonRemoveFan(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (!popupConfig->IsOpen) {
		state = 2;

		popupConfig->IsOpen = true;
	}
}

void IotTemperatureWatcher::Config::clickButtonAddSensor(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (!popupConfig->IsOpen) {
		state = 3;

		popupConfig->IsOpen = true;
	}
}

void IotTemperatureWatcher::Config::clickButtonRemoveSensor(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (!popupConfig->IsOpen) {
		state = 4;

		popupConfig->IsOpen = true;
	}
}

void IotTemperatureWatcher::Config::clickButtonChangeThresholds(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (!popupConfig->IsOpen) {
		state = 5;

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