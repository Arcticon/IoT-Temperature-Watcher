//
// MainPage.xaml.h
// Deklaration der MainPage-Klasse
//

#pragma once

#include "MainPage.g.h"

namespace IoT_Temperature_Watcher
{
	/// <summary>
	/// Eine leere Seite, die eigenständig verwendet werden kann oder auf die innerhalb eines Rahmens navigiert werden kann.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();

	private:
		void toggledToggleSwitchCustomMode(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void toggledToggleSwitchVentilator1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void toggledToggleSwitchVentilator2(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void clickedButtonStartServer(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
