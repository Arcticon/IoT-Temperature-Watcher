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

		int ticks = 10000000;
		int ticksMultiplier = 10;
		

		void updateAll();
		void clickButtonRackConfig1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
