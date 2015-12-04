#pragma once

#include "Overview.g.h"

namespace IotTemperatureWatcher
{
	/// <summary>
	/// Eine leere Seite, die eigenständig verwendet werden kann oder auf die innerhalb eines Rahmens navigiert werden kann.
	/// </summary>
	public ref class Overview sealed
	{
	public:
		Overview();

	private:

		std::string _ARDUINOIP = "10.11.90.231";

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

		int _ETHERNETPORT = 50001;

		int ticks2 = 10000000;
		int ticksMultiplier2 = 10;
		

		void updateAll();
		void clickButtonRackConfig1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		std::string getDataFromServerToString(std::string str);
		float getDataFromServer(std::string str);
	};
}
