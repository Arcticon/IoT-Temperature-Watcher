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
		static Platform::String^ getStatus(float temp);

	private:

		int ticks = 10000000;
		int ticksMultiplier = 10;
		int ticksMultiplier2 = 300;


		std::string _CUSTOMMODEOFF = "customModeOff";
		std::string _CUSTOMMODEON = "customModeOn";
		std::string _FANCONFIGOFF = "fanConfigOff";
		std::string _FANCONFIGON = "fanConfigOn";
		std::string _SENSORCONFIGOFF = "sensorConfigOff";
		std::string _SENSORCONFIGON = "sensorConfigOn";
		std::string _THRESHOLDCONFIGOFF = "thresholdConfigOff";
		std::string _THRESHOLDCONFIGON = "thresholdConfigOn";
		std::string _SETFAN1ON = "setFan1On";
		std::string _SETFAN1OFF = "setFan1Off";
		std::string _SETFAN2ON = "setFan2On";
		std::string _SETFAN2OFF = "setFan2Off";
		std::string _ARDUINOIP = "10.11.90.231";
		std::string _F1ON = "f1on";
		std::string _F1OFF = "f1off";
		std::string _F2ON = "f2on";
		std::string _F2OFF = "f2off";
		std::string _GETTEMP1 = "getTemp1";
		std::string _GETTEMP2 = "getTemp2";
		std::string _GETTEMP3 = "getTemp3";
		std::string _GETTEMP4 = "getTemp4";
		std::string _GETTEMP5 = "getTemp5";
		std::string _GETTEMP6 = "getTemp6";
		std::string _GETFAN1 = "getFan1";
		std::string _GETFAN2 = "getFan2";


		Platform::String^ _TEXTFAN1ON = "Fan 1 Top - running";
		Platform::String^ _TEXTFAN1OFF = "Fan 1 Top - not running";
		Platform::String^ _TEXTFAN2ON = "Fan 2 Top - running";
		Platform::String^ _TEXTFAN2OFF = "Fan 2 Top - not running";
		Platform::String^ _TEXTUPPERTEMP1 = "Upper Temperature1: ";
		Platform::String^ _TEXTUPPERTEMP1V2 = "Upper Temperature1 Fan2: ";
		Platform::String^ _TEXTLOWERTEMP1 = "Lower Temperature1: ";
		Platform::String^ _TEXTUPPERTEMP2 = "Upper Temperature2: ";
		Platform::String^ _TEXTUPPERTEMP2V1 = "Upper Temperature2 Fan1: ";
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

		Platform::String^ _TEMPSENSORSTL = "STL: ";
		Platform::String^ _TEMPSENSORSCL = "SCL: ";
		Platform::String^ _TEMPSENSORSBL = "SBL: ";
		Platform::String^ _TEMPSENSORSTR = "STR: ";
		Platform::String^ _TEMPSENSORSCR = "SCR: ";
		Platform::String^ _TEMPSENSORSBR = "SBR: ";

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
		void cleanErrors();
		void sendDataToServer(std::string str);
		void sendDataToServerFromInt(int i);
		float getDataFromServer(std::string str);
		std::string getDataFromServerToString(std::string str);
		void openedPopupConfig(Platform::Object^ sender, Platform::Object^ e);
		void closedPopupConfig(Platform::Object^ sender, Platform::Object^ e);
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

		std::string sliderValToString(int val);
};
}
