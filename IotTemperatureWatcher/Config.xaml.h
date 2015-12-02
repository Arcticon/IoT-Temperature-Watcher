//
// Config.xaml.h
// Deklaration der Config-Klasse
//

#pragma once

#include "Config.g.h"

namespace IotTemperatureWatcher
{
	/// <summary>
	/// Eine leere Seite, die eigenständig verwendet werden kann oder auf die innerhalb eines Rahmens navigiert werden kann.
	/// </summary>
	public ref class Config sealed
	{
	public:
		Config();

	private:

		int ticks = 10000000;
		int ticksMultiplier = 1;

		std::string _CUSTOMMODEOFF = "customModeOff";
		std::string _CUSTOMMODEON = "customModeOn";
		std::string _FANCONFIGOFF = "fanConfigOff";
		std::string _FANCONFIGON = "fanConfigOn";
		std::string _SENSORCONFIGOFF = "sensorConfigOff";
		std::string _SENSORCONFIGON = "sensorConfigOn";
		std::string _THRESHOLDCONFIGOFF = "thresholdConfigOff";
		std::string _THRESHOLDCONFIGON = "thresholdConfigOn";
		std::string _SETVENT1ON = "setVent1On";
		std::string _SETVENT1OFF = "setVent1Off";
		std::string _SETVENT2ON = "setVent2On";
		std::string _SETVENT2OFF = "setVent2Off";
		std::string _ARDUINOIP = "10.11.90.231";
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
		Platform::String^ _TEXTUPPERTEMP1 = "Upper Temperature1: ";
		Platform::String^ _TEXTUPPERTEMP1V2 = "Upper Temperature1 Vent2: ";
		Platform::String^ _TEXTLOWERTEMP1 = "Lower Temperature1: ";
		Platform::String^ _TEXTUPPERTEMP2 = "Upper Temperature2: ";
		Platform::String^ _TEXTUPPERTEMP2V1 = "Upper Temperature2 Vent1: ";
		Platform::String^ _TEXTLOWERTEMP2 = "Lower Temperature2: ";
		Platform::String^ _TEXTPOPUPERRORMESSAGE = "Error Message: ";
		Platform::String^ _TEXTPOPUPERRORMESSAGESOCKETFAILED = "SOCKET FAILED!!";
		Platform::String^ _TEXTPOPUPERRORMESSAGECONNECTFAILED = "CONNECT FAILED!!";
		Platform::String^ _TEXTPOPUPERRORMESSAGESENDFAILED = "SEND FAILED!!";
		Platform::String^ _TEXTPOPUPERRORMESSAGESENSOR1NOTCONNECTED = "SENSOR 1 NOT CONNECTED!!";
		Platform::String^ _TEXTPOPUPERRORMESSAGESENSOR2NOTCONNECTED = "SENSOR 2 NOT CONNECTED!!";


		Platform::String^ _TEXTAPPBARTOGGLEBUTTONENABLEFANCONFIG = "Enable Fan Configuration";
		Platform::String^ _TEXTAPPBARTOGGLEBUTTONENABLESENSORCONFIG = "Enable Sensor Configuration";
		Platform::String^ _TEXTAPPBARTOGGLEBUTTONENABLETHRESHOLDCONFIG = "Enable Threshold Configuration";
		Platform::String^ _TEXTAPPBARTOGGLEBUTTONDISABLEFANCONFIG = "Disable Fan Configuration";
		Platform::String^ _TEXTAPPBARTOGGLEBUTTONDISABLESENSORCONFIG = "Disable Sensor Configuration";
		Platform::String^ _TEXTAPPBARTOGGLEBUTTONDISABLETHRESHOLDCONFIG = "Disable Threshold Configuration";

		//Platform::String^ _TEXTPOPUPERRORMESSAGE = "";

		int _ETHERNETPORT = 50001;

		void toggledToggleSwitchCustomMode(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void toggledToggleSwitchVentilator1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void toggledToggleSwitchVentilator2(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void updateAll();
		void sendDataToServer(std::string str);
		void sendDataToServerFromInt(int i);
		float getDataFromServer(std::string str);
		std::string getDataFromServerToString(std::string str);
		void valueChangedSliderUpperTemp1(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
		void valueChangedSliderUpperTemp1V2(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
		void valueChangedSliderLowerTemp1(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
		void valueChangedSliderUpperTemp2(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
		void valueChangedSliderUpperTemp2V1(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
		void valueChangedSliderLowerTemp2(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
		void openedPopupTest(Platform::Object^ sender, Platform::Object^ e);
		void closedPopupTest(Platform::Object^ sender, Platform::Object^ e);
		void clickedButtonPopupOk(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void checkedCheckBoxAdvancedCustomMode(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void uncheckedCheckBoxAdvancedCustomMode(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

		void clickAppBarButtonSave(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

		void checkedAppBarToggleButtonFanConfig(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void uncheckedAppBarToggleButtonFanConfig(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void checkedAppBarToggleButtonSensorConfig(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void uncheckedAppBarToggleButtonSensorConfig(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void checkedAppBarToggleButtonThresholdConfig(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void uncheckedAppBarToggleButtonThresholdConfig(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
