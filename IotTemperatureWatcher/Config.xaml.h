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
		int ticksMultiplier = 10;


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
		std::string _GETTEMP3 = "getTemp3";
		std::string _GETTEMP4 = "getTemp4";
		std::string _GETTEMP5 = "getTemp5";
		std::string _GETTEMP6 = "getTemp6";
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
		Platform::String^ _TEXTPOPUPERRORMESSAGESENSOR3NOTCONNECTED = "SENSOR 3 NOT CONNECTED!!";
		Platform::String^ _TEXTPOPUPERRORMESSAGESENSOR4NOTCONNECTED = "SENSOR 4 NOT CONNECTED!!";
		Platform::String^ _TEXTPOPUPERRORMESSAGESENSOR5NOTCONNECTED = "SENSOR 5 NOT CONNECTED!!";
		Platform::String^ _TEXTPOPUPERRORMESSAGESENSOR6NOTCONNECTED = "SENSOR 6 NOT CONNECTED!!";


		Platform::String^ _TEXTAPPBARTOGGLEBUTTONENABLEFANCONFIG = "Enable Fan Configuration";
		Platform::String^ _TEXTAPPBARTOGGLEBUTTONENABLESENSORCONFIG = "Enable Sensor Configuration";
		Platform::String^ _TEXTAPPBARTOGGLEBUTTONENABLETHRESHOLDCONFIG = "Enable Threshold Configuration";
		Platform::String^ _TEXTAPPBARTOGGLEBUTTONDISABLEFANCONFIG = "Disable Fan Configuration";
		Platform::String^ _TEXTAPPBARTOGGLEBUTTONDISABLESENSORCONFIG = "Disable Sensor Configuration";
		Platform::String^ _TEXTAPPBARTOGGLEBUTTONDISABLETHRESHOLDCONFIG = "Disable Threshold Configuration";

		Platform::String^ _TEMPSENSORSTL = "Temperature Sensor Top Left (STL): ";
		Platform::String^ _TEMPSENSORSCL = "Temperature Sensor Center Left (SCL): ";
		Platform::String^ _TEMPSENSORSBL = "Temperature Sensor Bottom Left (SBL): ";
		Platform::String^ _TEMPSENSORSTR = "Temperature Sensor Top Right (STR): ";
		Platform::String^ _TEMPSENSORSCR = "Temperature Sensor Center Right (SCR): ";
		Platform::String^ _TEMPSENSORSBR = "Temperature Sensor Bottom Right (SBR): ";

		Platform::String^ _DATACENTER = "Datacenter: ";
		Platform::String^ _ROOM = " | Room: ";
		Platform::String^ _RACK = " | Rack: ";
		Platform::String^ _CURRTHRESHSENSTOP = "Current Thresholds Sensor Top:\n";
		Platform::String^ _CURRTHRESHSENSCEN = "Current Thresholds Sensor Center:\n";
		Platform::String^ _CURRTHRESHSENSBOT = "Current Thresholds Sensor Bottom:\n";
		//Platform::String^ _ = "";

		//Platform::String^ _TEXTPOPUPERRORMESSAGE = "";

		int _ETHERNETPORT = 50001;

		void updateAll();
		void sendDataToServer(std::string str);
		void sendDataToServerFromInt(int i);
		float getDataFromServer(std::string str);
		std::string getDataFromServerToString(std::string str);
		void openedPopupError(Platform::Object^ sender, Platform::Object^ e);
		void closedPopupError(Platform::Object^ sender, Platform::Object^ e);
		void openedPopupConfig(Platform::Object^ sender, Platform::Object^ e);
		void closedPopupConfig(Platform::Object^ sender, Platform::Object^ e);
		void clickedButtonPopupErrorOk(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void clickedButtonPopupConfigSave(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

		
		void checkedCheckBoxAdvancedCustomMode(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void uncheckedCheckBoxAdvancedCustomMode(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void clickAppBarButtonSave(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void checkedAppBarToggleButtonFanConfig(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void uncheckedAppBarToggleButtonFanConfig(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void checkedAppBarToggleButtonSensorConfig(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void uncheckedAppBarToggleButtonSensorConfig(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void checkedAppBarToggleButtonThresholdConfig(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void uncheckedAppBarToggleButtonThresholdConfig(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void clickButtonChangeThresholds(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void clickButtonAddFan(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void clickButtonRemoveFan(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void clickButtonAddSensor(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void clickButtonRemoveSensor(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
};
}
