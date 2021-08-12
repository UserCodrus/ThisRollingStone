#include "MainMenuHUD.h"
#include "MenuWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

AMainMenuHUD::AMainMenuHUD()
{
	// Set property defaults
	FadeDuration = 1.0f;
}

/// AHUD ///

void AMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	// Create game widgets
	CreateMenuWidget(MainMenuClass, MainMenuWidget, true);
}

/// Object Interface ///

void AMainMenuHUD::SetOpacity(float Opacity)
{
	MainMenuWidget->SetRenderOpacity(Opacity);
}

/// Object Interface ///


void AMainMenuHUD::CreateMenuWidget(UClass* WidgetClass, UMenuWidget*& Widget, bool Visible)
{
	if (WidgetClass != nullptr && Widget == nullptr)
	{
		// Create the widget
		Widget = CreateWidget<UMenuWidget>(GetOwningPlayerController(), WidgetClass);
		if (Widget != nullptr)
		{
			// Set this hud as the parent
			Widget->SetOwner(this);

			// Attach the widget and hide it
			Widget->AddToViewport();
			if (!Visible)
			{
				Widget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}