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

		
		void clickButtonBack(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
