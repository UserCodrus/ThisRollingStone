#pragma once

#include "BaseHUD.h"

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

// The that displays in-game
UCLASS()
class ROLLINGSTONE_API AGameHUD : public ABaseHUD
{
	GENERATED_BODY()

public:
	AGameHUD();

	/// AHUD ///

	virtual void BeginPlay() override;

	/// ABaseHUD ///

	virtual void SetOpacity(float Opacity) override;

	/// Public Interface ///

	// Show the end of level widget
	void ShowFinishWidget();
	// Show the level failed widget
	void ShowFailedWidget();
	// Show or hide the pause widget
	void TogglePauseWidget(bool Show);

	/// Internal Functions ///

protected:
	// Create a new widget if one does not exist
	void SafeCreateWidget(UClass* WidgetClass, UUserWidget*& Widget, bool Visible = false);

	/// Properties ///

	// If set to false, the main hud display will be hidden
	UPROPERTY(EditDefaultsOnly)
		bool DisplayHUD;

	// The main game HUD
	UPROPERTY(Category = "Widgets", EditDefaultsOnly)
		TSubclassOf<class UUserWidget> MainWidgetClass;
	UPROPERTY(VisibleAnywhere)
		UUserWidget* MainWidget;
	// The start of level countdown
	UPROPERTY(Category = "Widgets", EditDefaultsOnly)
		TSubclassOf<class UUserWidget> StartWidgetClass;
	UPROPERTY(VisibleAnywhere)
		UUserWidget* StartWidget;
	// The end of level splash screen
	UPROPERTY(Category = "Widgets", EditDefaultsOnly)
		TSubclassOf<class UUserWidget> FinishWidgetClass;
	// The level failed splash screen
	UPROPERTY(Category = "Widgets", EditDefaultsOnly)
		TSubclassOf<class UUserWidget> FailedWidgetClass;
	UPROPERTY(VisibleAnywhere)
		UUserWidget* FinishWidget;
	// The pause screen
	UPROPERTY(Category = "Widgets", EditDefaultsOnly)
		TSubclassOf<class UUserWidget> PauseWidgetClass;
	UPROPERTY(VisibleAnywhere)
		UUserWidget* PauseWidget;
};
