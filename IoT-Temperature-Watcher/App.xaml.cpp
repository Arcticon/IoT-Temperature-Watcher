//
// App.xaml.cpp
// Implementierung der App-Klasse
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace IoT_Temperature_Watcher;

using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

/// <summary>
/// Initialisiert das Singletonanwendungsobjekt.  Dies ist die erste Zeile von erstelltem Code
/// und daher das logische Äquivalent von main() bzw. WinMain().
/// </summary>
App::App()
{
	InitializeComponent();
	Suspending += ref new SuspendingEventHandler(this, &App::OnSuspending);
}

/// <summary>
/// Wird aufgerufen, wenn die Anwendung durch den Endbenutzer normal gestartet wird.	Weitere Einstiegspunkte
/// werden z. B. verwendet, wenn die Anwendung gestartet wird, um eine bestimmte Datei zu öffnen.
/// </summary>
/// <param name="e">Details über Startanforderung und -prozess.</param>
void App::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e)
{

#if _DEBUG
		// Während des Debuggens Profilerstellungsinformationen zur Grafikleistung anzeigen.
		if (IsDebuggerPresent())
		{
			// Zähler für die aktuelle Bildrate anzeigen
			 DebugSettings->EnableFrameRateCounter = true;
		}
#endif

	auto rootFrame = dynamic_cast<Frame^>(Window::Current->Content);

	// App-Initialisierung nicht wiederholen, wenn das Fenster bereits Inhalte enthält.
	// Nur sicherstellen, dass das Fenster aktiv ist.
	if (rootFrame == nullptr)
	{
		// Frame erstellen, der als Navigationskontext fungiert, und ihn mit
		// einem SuspensionManager-Schlüssel verknüpfen
		rootFrame = ref new Frame();

		rootFrame->NavigationFailed += ref new Windows::UI::Xaml::Navigation::NavigationFailedEventHandler(this, &App::OnNavigationFailed);

		if (e->PreviousExecutionState == ApplicationExecutionState::Terminated)
		{
			// TODO: Den gespeicherten Sitzungszustand nur bei Bedarf wiederherstellen. Dabei die
			// abschließenden Schritte zum Start planen, nachdem die Wiederherstellung abgeschlossen ist

		}

		if (rootFrame->Content == nullptr)
		{
			// Wenn der Navigationsstapel nicht wiederhergestellt wird, zur ersten Seite navigieren
			// und die neue Seite konfigurieren, indem die erforderlichen Informationen als Navigationsparameter
			// übergeben werden
			rootFrame->Navigate(TypeName(MainPage::typeid), e->Arguments);
		}
		// Den Frame im aktuellen Fenster platzieren
		Window::Current->Content = rootFrame;
		// Sicherstellen, dass das aktuelle Fenster aktiv ist
		Window::Current->Activate();
	}
	else
	{
		if (rootFrame->Content == nullptr)
		{
			// Wenn der Navigationsstapel nicht wiederhergestellt wird, zur ersten Seite navigieren
			// und die neue Seite konfigurieren, indem die erforderlichen Informationen als Navigationsparameter
			// übergeben werden
			rootFrame->Navigate(TypeName(MainPage::typeid), e->Arguments);
		}
		// Sicherstellen, dass das aktuelle Fenster aktiv ist
		Window::Current->Activate();
	}
}

/// <summary>
/// Wird aufgerufen, wenn die Ausführung der Anwendung angehalten wird.	Der Anwendungszustand wird gespeichert,
/// ohne zu wissen, ob die Anwendung beendet oder fortgesetzt wird und die Speicherinhalte dabei
/// unbeschädigt bleiben.
/// </summary>
/// <param name="sender">Die Quelle der Anhalteanforderung.</param>
/// <param name="e">Details zur Anhalteanforderung.</param>
void App::OnSuspending(Object^ sender, SuspendingEventArgs^ e)
{
	(void) sender;	// Nicht verwendeter Parameter
	(void) e;	// Nicht verwendeter Parameter

	//TODO: Anwendungszustand speichern und alle Hintergrundaktivitäten beenden
}

/// <summary>
/// Wird aufgerufen, wenn die Navigation auf eine bestimmte Seite fehlschlägt
/// </summary>
/// <param name="sender">Der Rahmen, bei dem die Navigation fehlgeschlagen ist</param>
/// <param name="e">Details über den Navigationsfehler</param>
void App::OnNavigationFailed(Platform::Object ^sender, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs ^e)
{
	throw ref new FailureException("Failed to load Page " + e->SourcePageType.Name);
}