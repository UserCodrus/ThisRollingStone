#pragma once

#include "BaseHUD.h"

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainMenuHUD.generated.h"

class UMenuWidget;

// The hud for the main menu
UCLASS()
class ROLLINGSTONE_API AMainMenuHUD : public ABaseHUD
{
	GENERATED_BODY()

public:
	AMainMenuHUD();
	
	/// AHUD ///

	virtual void BeginPlay() override;

	/// Object Interface ///

	virtual void SetOpacity(float Opacity) override;

	/// Object Interface ///

protected:
	// Create a new widget
	void CreateMenuWidget(UClass* WidgetClass, UMenuWidget*& Widget, bool Visible = false);

	/// Properties ///

	// The menu widget
	UPROPERTY(Category = "Widgets", EditDefaultsOnly)
		TSubclassOf<class UMenuWidget> MainMenuClass;
	UMenuWidget* MainMenuWidget;
};
