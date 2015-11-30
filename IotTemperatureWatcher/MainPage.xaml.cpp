#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib, "user32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Winsock2.h>
#include <Windows.h>
#include <thread>

#include <time.h>

#include "pch.h"
#include "MainPage.xaml.h"
#include "MainView.xaml.h"
#include "MainPage.g.h"
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
float tmpFloat1 = 0;
float tmpFloat2 = 0;
int slp = 75; //75

void sendDataToServer(std::string str);
std::string sliderValToString(int val);

MainPage::MainPage()
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

			IotTemperatureWatcher::MainPage::updateAll();

		}));
	}), period);
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

		sendDataToServer(_CUSTOMMODEOFF);

		textBlockUpperTemp1->Opacity = 0.4;
		textBlockUpperTemp1V2->Opacity = 0.4;
		textBlockLowerTemp1->Opacity = 0.4;
		textBlockUpperTemp2->Opacity = 0.4;
		textBlockUpperTemp2V1->Opacity = 0.4;
		textBlockLowerTemp2->Opacity = 0.4;

		sliderUpperTemp1->IsEnabled = false;
		sliderLowerTemp1->IsEnabled = false;
		sliderUpperTemp1V2->IsEnabled = false;
		sliderUpperTemp2->IsEnabled = false;
		sliderUpperTemp2V1->IsEnabled = false;
		sliderLowerTemp2->IsEnabled = false;
		checkBoxAdvancedCustomMode->IsEnabled = false;
		checkBoxAdvancedCustomMode->IsChecked = false;
		toggleSwitchVentilator1->IsEnabled = false;
		toggleSwitchVentilator2->IsEnabled = false;

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

	textBlockTemperature1->Text = tmpFloat1 + "°C";
	textBlockTemperature2->Text = tmpFloat2 + "°C";
	
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

	Color c1;
	Color c2;
	if (tmpFloat1 <= 20) {
		c1.A = 255;
		c1.G = 255;
		c1.R = 0;
		c1.B = 41;
	}
	else if (tmpFloat1 > 20 && tmpFloat1 <= 40) {
		c1.A = 255;
		c1.G = 255;
		c1.R = (tmpFloat1 - 20) * 12.75;
		c1.B = 41;
		//tbv1->Text = "Rot: " + c1.R + " | Grün: " + c1.G;
	}
	else if (tmpFloat1 > 40 && tmpFloat1 <= 80) {
		c1.A = 255;
		c1.G = 255 - (tmpFloat1 - 40) * 6.25;
		c1.R = 255;
		c1.B = 41;
	}
	else if (tmpFloat1 > 40) {
		c1.A = 255;
		c1.R = 255 - (tmpFloat1 - 80) * 12.75;
		c1.G = 41 - (tmpFloat1 - 80) * 2;
		c1.B = 41 - (tmpFloat1 - 80) * 2;
	}
	if (tmpFloat2 <= 20) {
		c2.A = 255;
		c2.G = 255;
		c2.R = 0;
		c2.B = 41;
	}
	else if (tmpFloat2 > 20 && tmpFloat2 <= 40) {
		c2.A = 255;
		c2.G = 255;
		c2.R = (tmpFloat2 - 20) * 12.75;
		c2.B = 41;
	}
	else if (tmpFloat2 > 40 && tmpFloat2 <= 80) {
		c2.A = 255;
		c2.G = 255 - (tmpFloat2 - 40) * 6.25;
		c2.R = 255;
		c2.B = 41;
	}
	else if (tmpFloat2 > 80) {
		c2.A = 255;
		c2.G = 255 - (tmpFloat2 - 80) * 12.75;
		c2.R = 41 - (tmpFloat2 - 80) * 2;
		c2.B = 41 - (tmpFloat2 - 80) * 2;
	}

	auto textForeground1 = ref new SolidColorBrush();
	auto textForeground2 = ref new SolidColorBrush();
	textForeground1->Color = c1;
	textForeground2->Color = c2;
	progressBar1->Foreground = textForeground1;
	progressBar2->Foreground = textForeground2;

}

void IotTemperatureWatcher::MainPage::valueChangedSliderUpperTemp1(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	sendDataToServer("slider1:" + sliderValToString((int)sliderUpperTemp1->Value));
	textBlockUpperTemp1->Text = _TEXTUPPERTEMP1 + (int)sliderUpperTemp1->Value;
	Sleep(slp);
}

void IotTemperatureWatcher::MainPage::valueChangedSliderUpperTemp1V2(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e) {
	sendDataToServer("slider5:" + sliderValToString((int)sliderUpperTemp1V2->Value));
	textBlockUpperTemp1V2->Text = _TEXTUPPERTEMP1V2 + (int)sliderUpperTemp1V2->Value;
	Sleep(slp);
}

void IotTemperatureWatcher::MainPage::valueChangedSliderLowerTemp1(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
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

void IotTemperatureWatcher::MainPage::valueChangedSliderUpperTemp2(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	sendDataToServer("slider3:" + sliderValToString((int)sliderUpperTemp2->Value));
	textBlockUpperTemp2->Text = _TEXTUPPERTEMP2 + (int)sliderUpperTemp2->Value;
	Sleep(slp);
}

void IotTemperatureWatcher::MainPage::valueChangedSliderUpperTemp2V1(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e) {
	sendDataToServer("slider6:" + sliderValToString((int)sliderUpperTemp2V1->Value));
	textBlockUpperTemp2V1->Text = _TEXTUPPERTEMP2V1 + (int)sliderUpperTemp2V1->Value;
	Sleep(slp);
}

void IotTemperatureWatcher::MainPage::valueChangedSliderLowerTemp2(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
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

void IotTemperatureWatcher::MainPage::uncheckedCheckBoxAdvancedCustomMode(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
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

void IotTemperatureWatcher::MainPage::changeXaml(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//this->FrameMainPage->Navigate(IotTemperatureWatcher::MainView::);
}
