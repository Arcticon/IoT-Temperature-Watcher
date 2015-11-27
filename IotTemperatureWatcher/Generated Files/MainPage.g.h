﻿#pragma once
//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------


namespace Windows {
    namespace UI {
        namespace Xaml {
            namespace Controls {
                ref class CheckBox;
                ref class TextBlock;
                ref class ToggleSwitch;
                ref class Slider;
                ref class ProgressBar;
                ref class Button;
            }
        }
    }
}
namespace Windows {
    namespace UI {
        namespace Xaml {
            namespace Controls {
                namespace Primitives {
                    ref class Popup;
                }
            }
        }
    }
}

namespace IotTemperatureWatcher
{
    [::Windows::Foundation::Metadata::WebHostHidden]
    partial ref class MainPage : public ::Windows::UI::Xaml::Controls::Page, 
        public ::Windows::UI::Xaml::Markup::IComponentConnector,
        public ::Windows::UI::Xaml::Markup::IComponentConnector2
    {
    public:
        void InitializeComponent();
        virtual void Connect(int connectionId, ::Platform::Object^ target);
        virtual ::Windows::UI::Xaml::Markup::IComponentConnector^ GetBindingConnector(int connectionId, ::Platform::Object^ target);
    
    private:
        bool _contentLoaded;
    
        private: ::Windows::UI::Xaml::Controls::CheckBox^ checkBoxAdvancedCustomMode;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ textBlockVentilator1;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ textBlockVentilator2;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ textBlockTemperature1;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ textBlockTemperature2;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ textBlockUpperTemp1;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ textBlockUpperTemp1V2;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ textBlockLowerTemp1;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ textBlockUpperTemp2;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ textBlockUpperTemp2V1;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ textBlockLowerTemp2;
        private: ::Windows::UI::Xaml::Controls::ToggleSwitch^ toggleSwitchCustomMode;
        private: ::Windows::UI::Xaml::Controls::ToggleSwitch^ toggleSwitchVentilator1;
        private: ::Windows::UI::Xaml::Controls::ToggleSwitch^ toggleSwitchVentilator2;
        private: ::Windows::UI::Xaml::Controls::Slider^ sliderUpperTemp1;
        private: ::Windows::UI::Xaml::Controls::Slider^ sliderUpperTemp1V2;
        private: ::Windows::UI::Xaml::Controls::Slider^ sliderLowerTemp1;
        private: ::Windows::UI::Xaml::Controls::Slider^ sliderUpperTemp2;
        private: ::Windows::UI::Xaml::Controls::Slider^ sliderUpperTemp2V1;
        private: ::Windows::UI::Xaml::Controls::Slider^ sliderLowerTemp2;
        private: ::Windows::UI::Xaml::Controls::ProgressBar^ progressBar2;
        private: ::Windows::UI::Xaml::Controls::ProgressBar^ progressBar1;
        private: ::Windows::UI::Xaml::Controls::Primitives::Popup^ popupTest;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ textBlockPopupErrorMessage;
        private: ::Windows::UI::Xaml::Controls::Button^ buttonPopupOk;
    };
}

