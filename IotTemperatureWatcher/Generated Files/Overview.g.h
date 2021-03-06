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
            namespace Media {
                namespace Animation {
                    ref class Storyboard;
                }
            }
        }
    }
}
namespace Windows {
    namespace UI {
        namespace Xaml {
            namespace Controls {
                ref class Frame;
                ref class Grid;
                ref class Image;
                ref class TextBox;
                ref class Button;
                ref class TextBlock;
                ref class PasswordBox;
            }
        }
    }
}

namespace IotTemperatureWatcher
{
    [::Windows::Foundation::Metadata::WebHostHidden]
    partial ref class Overview : public ::Windows::UI::Xaml::Controls::Page, 
        public ::Windows::UI::Xaml::Markup::IComponentConnector,
        public ::Windows::UI::Xaml::Markup::IComponentConnector2
    {
    public:
        void InitializeComponent();
        virtual void Connect(int connectionId, ::Platform::Object^ target);
        virtual ::Windows::UI::Xaml::Markup::IComponentConnector^ GetBindingConnector(int connectionId, ::Platform::Object^ target);
    
    private:
        bool _contentLoaded;
    
        private: ::Windows::UI::Xaml::Media::Animation::Storyboard^ StoryboardButton1;
        private: ::Windows::UI::Xaml::Media::Animation::Storyboard^ StoryboardFlyout1;
        private: ::Windows::UI::Xaml::Media::Animation::Storyboard^ StoryboardText1;
        private: ::Windows::UI::Xaml::Media::Animation::Storyboard^ StoryboardText2;
        private: ::Windows::UI::Xaml::Controls::Frame^ FrameOverview;
        private: ::Windows::UI::Xaml::Controls::Grid^ GridOverview;
        private: ::Windows::UI::Xaml::Controls::Image^ serverRack1;
        private: ::Windows::UI::Xaml::Controls::TextBox^ textBoxRackInfo1;
        private: ::Windows::UI::Xaml::Controls::TextBox^ textBoxRackErrors1;
        private: ::Windows::UI::Xaml::Controls::Button^ buttonRackConfig1;
        private: ::Windows::UI::Xaml::Controls::Image^ serverRack2;
        private: ::Windows::UI::Xaml::Controls::TextBox^ textBoxRackInfo2;
        private: ::Windows::UI::Xaml::Controls::TextBox^ textBoxRackErrors2;
        private: ::Windows::UI::Xaml::Controls::Button^ buttonRackConfig2;
        private: ::Windows::UI::Xaml::Controls::Image^ serverRack3;
        private: ::Windows::UI::Xaml::Controls::TextBox^ textBoxRackInfo3;
        private: ::Windows::UI::Xaml::Controls::TextBox^ textBoxRackErrors3;
        private: ::Windows::UI::Xaml::Controls::Button^ buttonRackConfig3;
        private: ::Windows::UI::Xaml::Controls::Image^ serverRack4;
        private: ::Windows::UI::Xaml::Controls::TextBox^ textBoxRackInfo4;
        private: ::Windows::UI::Xaml::Controls::TextBox^ textBoxRackErrors4;
        private: ::Windows::UI::Xaml::Controls::Button^ buttonRackConfig4;
        private: ::Windows::UI::Xaml::Controls::Image^ serverRack5;
        private: ::Windows::UI::Xaml::Controls::TextBox^ textBoxRackInfo5;
        private: ::Windows::UI::Xaml::Controls::TextBox^ textBoxRackErrors5;
        private: ::Windows::UI::Xaml::Controls::Button^ buttonRackConfig5;
        private: ::Windows::UI::Xaml::Controls::Image^ serverRack6;
        private: ::Windows::UI::Xaml::Controls::TextBox^ textBoxRackInfo6;
        private: ::Windows::UI::Xaml::Controls::TextBox^ textBoxRackErrors6;
        private: ::Windows::UI::Xaml::Controls::Button^ buttonRackConfig6;
        private: ::Windows::UI::Xaml::Controls::Button^ button;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ textBlock1;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ textBlock2;
        private: ::Windows::UI::Xaml::Controls::TextBox^ userName;
        private: ::Windows::UI::Xaml::Controls::PasswordBox^ passWord;
    };
}

