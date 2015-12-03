#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib, "user32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Winsock2.h>
#include <Windows.h>
#include <thread>

#include <time.h>

#include "pch.h"
#include "Overview.xaml.h"
#include "Config.xaml.h"
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


Overview::Overview()
{
	InitializeComponent();

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

			IotTemperatureWatcher::Overview::updateAll();

		}));
	}), period);
}

void IotTemperatureWatcher::Overview::clickButtonRackConfig1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (this->Frame != nullptr)
	{
		this->Frame->Navigate(Config::typeid);
	}
}

void IotTemperatureWatcher::Overview::updateAll() {
	textBoxRackInfo1->Text = "Datacenter: 01 | Room: 01 | Rack: 01\nTemperature: " + ((globalTemp1 + globalTemp2 + globalTemp3 + globalTemp4 + globalTemp5 + globalTemp6)/6);
}