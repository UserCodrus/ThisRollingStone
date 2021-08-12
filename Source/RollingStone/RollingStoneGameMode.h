// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RollingStoneGameMode.generated.h"

UCLASS(minimalapi)
class ARollingStoneGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARollingStoneGameMode();

	/// AGameModeBase ///

	virtual void StartPlay() override;

	/// Public Functions ///

	// Get the time that has passed since the level started
	UFUNCTION(BlueprintCallable)
		float GetCurrentTime();
	// Add a time penalty to the level timer
	UFUNCTION(BlueprintCallable)
		void AddTimePenalty(float PenaltyAmount);
	// Start the level timer
	UFUNCTION(BlueprintCallable)
		void StartLevel();
	// Stop the level timer
	UFUNCTION(BlueprintCallable)
		void StopLevel();

	// Determine if the level has started or not
	UFUNCTION(BlueprintPure)
		bool IsLevelStarted();

	/// Properties ///

protected:
	// Set to true when the level has started
	UPROPERTY(EditAnywhere)
		bool LevelStarted;
	// The maximum time in seconds the player can be in a level before the timer stops counting
	UPROPERTY(EditDefaultsOnly)
		float LevelDuration;
	// The total accumulated time penalty
	UPROPERTY(EditAnywhere)
		float TimePenalty;
	// The final time for the level
	UPROPERTY(EditAnywhere)
		float TotalTime;

	// The timer for the level
	FTimerHandle GameTimer;
};



