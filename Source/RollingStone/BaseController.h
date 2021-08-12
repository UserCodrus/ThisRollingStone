#pragma once

#include "LevelData.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaseController.generated.h"

// The base player controller
UCLASS(BlueprintType)
class ROLLINGSTONE_API ABaseController : public APlayerController
{
	GENERATED_BODY()

public:
	ABaseController();

	/// ABaseHUD ///

	virtual void BeginPlay() override;

	/// Object Interface ///

	// Complete the current level
	UFUNCTION(BlueprintCallable)
		void LevelComplete(bool Failed);
	// Fade out and load a level
	UFUNCTION(BlueprintCallable)
		void LevelTransition(FName LevelName = NAME_None, bool FadeMusic = false);
	// Find and load the next level in the level list
	UFUNCTION(BlueprintCallable)
		void NextLevel();

	// Pause or unpause the game
	UFUNCTION(BlueprintCallable)
		void TogglePause();

	// Add a time penalty to the level time
	void AddTimePenalty(float PenaltyAmount);

	// Enable the mouse and focus it on a given widget
	void MouseInputMode();
	// Disable the mouse and lock it to the viewport
	void GameInputMode();
	// Enable game and mouse input
	void DualInputMode();

	/// Internal Functions ///

protected:
	// Trigger a level transition
	void ChangeLevel();
	// Enable the controls on a stone pawn
	void EnableControls();
	
	/// Properties ///

	// If set to true, the game will capture input when the level starts
	UPROPERTY(EditDefaultsOnly)
		bool CaptureInput;

	// The amount of time it takes to fade out when opening a level
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float FadeOutDuration;
	// The amount of time it takes to fade in when entering a level
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float FadeInDuration;
	// The amount of time after starting before the player can control their pawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float ControlDelay;

	// Set to true when a level transition has started
	UPROPERTY(VisibleAnywhere)
		bool InTransition;
	// The level the UI will transition to after fading out
	UPROPERTY(EditDefaultsOnly)
		FName PendingMap;

	// The data for the current world
	UPROPERTY(VisibleAnywhere)
		FWorldData CurrentWorld;
	// The data for the current game map
	UPROPERTY(VisibleAnywhere)
		FMapData CurrentMap;

	// The timer that handles fades
	FTimerHandle FadeTimer;
	// The timer that handles control delay
	FTimerHandle ControlTimer;
};
