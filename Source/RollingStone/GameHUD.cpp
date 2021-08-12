#include "GameHUD.h"
#include "BaseController.h"

#include "Blueprint/UserWidget.h"

AGameHUD::AGameHUD()
{
	// Set property defaults
	DisplayHUD = true;
}

/// AHUD ///

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();

	// Create the HUD display
	SafeCreateWidget(MainWidgetClass, MainWidget, DisplayHUD);
	SafeCreateWidget(StartWidgetClass, StartWidget, DisplayHUD);
	SafeCreateWidget(PauseWidgetClass, PauseWidget);
}

/// ABaseHUD ///

void AGameHUD::SetOpacity(float Opacity)
{
	MainWidget->SetRenderOpacity(Opacity);

	if (FinishWidget != nullptr)
	{
		FinishWidget->SetRenderOpacity(Opacity);
	}
	if (PauseWidget != nullptr)
	{
		PauseWidget->SetRenderOpacity(Opacity);
	}
	if (StartWidget != nullptr)
	{
		StartWidget->SetRenderOpacity(Opacity);
	}
}

/// Public Interface ///

void AGameHUD::ShowFinishWidget()
{
	// Hide the hud
	MainWidget->SetVisibility(ESlateVisibility::Hidden);

	// Create a finish widget
	SafeCreateWidget(FinishWidgetClass, FinishWidget, true);
}

void AGameHUD::ShowFailedWidget()
{
	// Hide the hud
	MainWidget->SetVisibility(ESlateVisibility::Hidden);

	// Create a finish widget
	SafeCreateWidget(FailedWidgetClass, FinishWidget, true);
}

void AGameHUD::TogglePauseWidget(bool Show)
{
	if (PauseWidget != nullptr)
	{
		if (Show)
		{
			PauseWidget->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			PauseWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

/// Internal Functions ///

void AGameHUD::SafeCreateWidget(UClass* WidgetClass, UUserWidget*& Widget, bool Visible)
{
	if (WidgetClass != nullptr && Widget == nullptr)
	{
		// Create the widget
		Widget = CreateWidget<UUserWidget>(GetOwningPlayerController(), WidgetClass);
		if (Widget != nullptr)
		{
			// Attach the widget and hide it
			Widget->AddToViewport();
			if (!Visible)
			{
				Widget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}