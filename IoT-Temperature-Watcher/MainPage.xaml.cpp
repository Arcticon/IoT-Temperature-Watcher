//
// MainPage.xaml.cpp
// Implementierung der MainPage-Klasse
//

#pragma comment(lib,"Ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include <Winsock2.h>
#include <Windows.h>
#include <thread>

#include "pch.h"
#include "MainPage.xaml.h"

using namespace IoT_Temperature_Watcher;

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

// Die Vorlage "Leere Seite" ist unter http://go.microsoft.com/fwlink/?LinkId=402352clcid=0x409 dokumentiert.

char sendbuffer[256];
char recvbuffer[256];

void sendDataToServer(std::string str);

MainPage::MainPage()
{
	InitializeComponent();
}

void sendDataToServer(std::string str);

MainPage::MainPage()
{
	InitializeComponent();
}


void IoT_Temperature_Watcher::MainPage::toggledToggleSwitchCustomMode(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	ToggleSwitch^ ts = (ToggleSwitch^)sender;
	ToggleSwitch^ tsv1 = toggleSwitchVentilator1;
	ToggleSwitch^ tsv2 = toggleSwitchVentilator2;
	if (ts->IsOn) {
		tsv1->IsEnabled = true;
		tsv2->IsEnabled = true;
	}
	else {
		tsv1->IsEnabled = false;
		tsv2->IsEnabled = false;
	}
}


void IoT_Temperature_Watcher::MainPage::toggledToggleSwitchVentilator1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	ToggleSwitch^ ts = (ToggleSwitch^)sender;
	TextBlock^ tbv1 = textBlockVentilator1;
	if (ts->IsOn) {
		tbv1->Text = "Ventilator 1 ON";
		sendDataToServer("v1on");
	}
	else {
		tbv1->Text = "Ventilator 1 OFF";
		sendDataToServer("v1off");
	}
}


void IoT_Temperature_Watcher::MainPage::toggledToggleSwitchVentilator2(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	ToggleSwitch^ ts = (ToggleSwitch^)sender;
	TextBlock^ tbv2 = textBlockVentilator2;
	if (ts->IsOn) {
		tbv2->Text = "Ventilator 2 ON";
		sendDataToServer("v2on");
	}
	else {
		tbv2->Text = "Ventilator 2 OFF";
		sendDataToServer("v2off");
	}
}

void sendDataToServer(std::string str) {
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
	conpar.sin_addr.s_addr = inet_addr("10.0.0.17");
	conpar.sin_family = AF_INET;
	//conpar.sin_port = htons(1337);
	conpar.sin_port = htons(50001);
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