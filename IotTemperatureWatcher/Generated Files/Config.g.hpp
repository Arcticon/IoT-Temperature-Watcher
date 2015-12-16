﻿//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------
#include "pch.h"

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BINDING_DEBUG_OUTPUT
extern "C" __declspec(dllimport) int __stdcall IsDebuggerPresent();
#endif

#include "Config.xaml.h"

void ::IotTemperatureWatcher::Config::InitializeComponent()
{
    if (_contentLoaded)
    {
        return;
    }
    _contentLoaded = true;
    ::Windows::Foundation::Uri^ resourceLocator = ref new ::Windows::Foundation::Uri(L"ms-appx:///Config.xaml");
    ::Windows::UI::Xaml::Application::LoadComponent(this, resourceLocator, ::Windows::UI::Xaml::Controls::Primitives::ComponentResourceLocation::Application);
}

void ::IotTemperatureWatcher::Config::Connect(int __connectionId, ::Platform::Object^ __target)
{
    switch (__connectionId)
    {
        case 1:
            {
                this->RackAnimationStoryboard = safe_cast<::Windows::UI::Xaml::Media::Animation::Storyboard^>(__target);
            }
            break;
        case 2:
            {
                this->StoryboardText1 = safe_cast<::Windows::UI::Xaml::Media::Animation::Storyboard^>(__target);
            }
            break;
        case 3:
            {
                this->StoryboardText2 = safe_cast<::Windows::UI::Xaml::Media::Animation::Storyboard^>(__target);
            }
            break;
        case 4:
            {
                this->FrameConfig = safe_cast<::Windows::UI::Xaml::Controls::Frame^>(__target);
            }
            break;
        case 5:
            {
                this->GridConfig = safe_cast<::Windows::UI::Xaml::Controls::Grid^>(__target);
            }
            break;
        case 6:
            {
                this->serverRackOpened = safe_cast<::Windows::UI::Xaml::Controls::Image^>(__target);
            }
            break;
        case 7:
            {
                this->textBoxUpperFan1 = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            }
            break;
        case 8:
            {
                this->textBoxUpperFan2 = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            }
            break;
        case 9:
            {
                this->textBoxSTL = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            }
            break;
        case 10:
            {
                this->textBoxSTR = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            }
            break;
        case 11:
            {
                this->textBoxSCL = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            }
            break;
        case 12:
            {
                this->textBoxSCR = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            }
            break;
        case 13:
            {
                this->textBoxSBL = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            }
            break;
        case 14:
            {
                this->textBoxSBR = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            }
            break;
        case 15:
            {
                this->textBoxErrors = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            }
            break;
        case 16:
            {
                this->textBoxInfo = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            }
            break;
        case 17:
            {
                this->textBoxThresholdUpperSensor = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            }
            break;
        case 18:
            {
                this->textBoxThresholdCenterSensor = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            }
            break;
        case 19:
            {
                this->textBoxThresholdLowerSensor = safe_cast<::Windows::UI::Xaml::Controls::TextBox^>(__target);
            }
            break;
        case 20:
            {
                this->buttonFanConfigPanel = safe_cast<::Windows::UI::Xaml::Controls::StackPanel^>(__target);
            }
            break;
        case 21:
            {
                this->buttonAddFan = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::Button^>(this->buttonAddFan))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::IotTemperatureWatcher::Config::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&Config::clickButtonAddFan);
            }
            break;
        case 22:
            {
                this->buttonRemoveFan = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::Button^>(this->buttonRemoveFan))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::IotTemperatureWatcher::Config::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&Config::clickButtonRemoveFan);
            }
            break;
        case 23:
            {
                this->buttonSenConfigPanel1 = safe_cast<::Windows::UI::Xaml::Controls::StackPanel^>(__target);
            }
            break;
        case 24:
            {
                this->buttonAddSensor = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::Button^>(this->buttonAddSensor))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::IotTemperatureWatcher::Config::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&Config::clickButtonAddSensor);
            }
            break;
        case 25:
            {
                this->buttonRemoveSensor = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::Button^>(this->buttonRemoveSensor))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::IotTemperatureWatcher::Config::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&Config::clickButtonRemoveSensor);
            }
            break;
        case 26:
            {
                this->buttonSenConfigPanel2 = safe_cast<::Windows::UI::Xaml::Controls::StackPanel^>(__target);
            }
            break;
        case 27:
            {
                this->buttonChangeThresholds = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::Button^>(this->buttonChangeThresholds))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::IotTemperatureWatcher::Config::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&Config::clickButtonChangeThresholds);
            }
            break;
        case 28:
            {
                this->popupConfig = safe_cast<::Windows::UI::Xaml::Controls::Primitives::Popup^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::Primitives::Popup^>(this->popupConfig))->Opened += ref new ::Windows::Foundation::EventHandler<::Platform::Object^>(this, (void (::IotTemperatureWatcher::Config::*)
                    (::Platform::Object^, ::Platform::Object^))&Config::openedPopupConfig);
                (safe_cast<::Windows::UI::Xaml::Controls::Primitives::Popup^>(this->popupConfig))->Closed += ref new ::Windows::Foundation::EventHandler<::Platform::Object^>(this, (void (::IotTemperatureWatcher::Config::*)
                    (::Platform::Object^, ::Platform::Object^))&Config::closedPopupConfig);
            }
            break;
        case 29:
            {
                this->button = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
            }
            break;
        case 30:
            {
                this->textBlock1 = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
            }
            break;
        case 31:
            {
                this->textBlock2 = safe_cast<::Windows::UI::Xaml::Controls::TextBlock^>(__target);
            }
            break;
        case 32:
            {
                this->sliderThresholdLowerText = safe_cast<::Windows::UI::Xaml::Controls::Slider^>(__target);
            }
            break;
        case 33:
            {
                this->sliderThresholdUpperText = safe_cast<::Windows::UI::Xaml::Controls::Slider^>(__target);
            }
            break;
        case 34:
            {
                this->buttonPopupConfigSave = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::Button^>(this->buttonPopupConfigSave))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::IotTemperatureWatcher::Config::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&Config::clickedButtonPopupConfigSave);
            }
            break;
        case 35:
            {
                this->appBarToggleButtonFanConfig = safe_cast<::Windows::UI::Xaml::Controls::AppBarToggleButton^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::AppBarToggleButton^>(this->appBarToggleButtonFanConfig))->Checked += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::IotTemperatureWatcher::Config::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&Config::checkedAppBarToggleButtonFanConfig);
                (safe_cast<::Windows::UI::Xaml::Controls::AppBarToggleButton^>(this->appBarToggleButtonFanConfig))->Unchecked += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::IotTemperatureWatcher::Config::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&Config::uncheckedAppBarToggleButtonFanConfig);
            }
            break;
        case 36:
            {
                this->appBarToggleButtonSensorConfig = safe_cast<::Windows::UI::Xaml::Controls::AppBarToggleButton^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::AppBarToggleButton^>(this->appBarToggleButtonSensorConfig))->Checked += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::IotTemperatureWatcher::Config::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&Config::checkedAppBarToggleButtonSensorConfig);
                (safe_cast<::Windows::UI::Xaml::Controls::AppBarToggleButton^>(this->appBarToggleButtonSensorConfig))->Unchecked += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::IotTemperatureWatcher::Config::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&Config::uncheckedAppBarToggleButtonSensorConfig);
            }
            break;
        case 37:
            {
                this->appBarToggleButtonThresholdConfig = safe_cast<::Windows::UI::Xaml::Controls::AppBarToggleButton^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::AppBarToggleButton^>(this->appBarToggleButtonThresholdConfig))->Checked += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::IotTemperatureWatcher::Config::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&Config::checkedAppBarToggleButtonThresholdConfig);
                (safe_cast<::Windows::UI::Xaml::Controls::AppBarToggleButton^>(this->appBarToggleButtonThresholdConfig))->Unchecked += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::IotTemperatureWatcher::Config::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&Config::uncheckedAppBarToggleButtonThresholdConfig);
            }
            break;
        case 38:
            {
                ::Windows::UI::Xaml::Controls::AppBarButton^ element38 = safe_cast<::Windows::UI::Xaml::Controls::AppBarButton^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::AppBarButton^>(element38))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::IotTemperatureWatcher::Config::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&Config::clickAppBarButtonSave);
            }
            break;
    }
    _contentLoaded = true;
}

::Windows::UI::Xaml::Markup::IComponentConnector^ ::IotTemperatureWatcher::Config::GetBindingConnector(int __connectionId, ::Platform::Object^ __target)
{
    __connectionId;         // unreferenced
    __target;               // unreferenced
    return nullptr;
}


