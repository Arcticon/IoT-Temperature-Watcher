//
// MainPage.xaml.h
// Deklaration der MainPage-Klasse
//

#pragma once

#include "MainPage.g.h"

namespace IotTemperatureWatcher
{
	/// <summary>
	/// Eine leere Seite, die eigenständig verwendet werden kann oder auf die innerhalb eines Rahmens navigiert werden kann.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();

	private:

		std::string _CUSTOMMODEOFF = "customModeOff";
		std::string _CUSTOMMODEON = "customModeOn";
		std::string _SETVENT1ON = "setVent1On";
		std::string _SETVENT1OFF = "setVent1Off";
		std::string _SETVENT2ON = "setVent2On";
		std::string _SETVENT2OFF = "setVent2Off";
		std::string _ARDUINOIP = "10.0.0.17";
		std::string _V1ON = "v1on";
		std::string _V1OFF = "v1off";
		std::string _V2ON = "v2on";
		std::string _V2OFF = "v2off";
		std::string _GETTEMP1 = "getTemp1";
		std::string _GETTEMP2 = "getTemp2";
		std::string _GETVENT1 = "getVent1";
		std::string _GETVENT2 = "getVent2";


		Platform::String^ _TEXTVENT1ON = "Ventilator 1: ON";
		Platform::String^ _TEXTVENT1OFF = "Ventilator 1: OFF";
		Platform::String^ _TEXTVENT2ON = "Ventilator 2: ON";
		Platform::String^ _TEXTVENT2OFF = "Ventilator 2: OFF";

		int _ETHERNETPORT = 50001;

		void toggledToggleSwitchCustomMode(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void toggledToggleSwitchVentilator1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void toggledToggleSwitchVentilator2(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void updateAll();
		void sendDataToServer(std::string str);
		float getDataFromServer(std::string str);
		std::string getDataFromServerToString(std::string str);
	};
}
