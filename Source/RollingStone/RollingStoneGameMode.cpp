// Copyright Epic Games, Inc. All Rights Reserved.

#include "RollingStoneGameMode.h"
#include "Stone.h"
#include "BaseController.h"

ARollingStoneGameMode::ARollingStoneGameMode()
{
	// Set default classes
	DefaultPawnClass = AStone::StaticClass();
	PlayerControllerClass = ABaseController::StaticClass();

	// Set default properties
	LevelDuration = 3600.0f;
	LevelStarted = false;
	TimePenalty = 0.0f;
	TotalTime = -1.0f;
}

/// AGameModeBase ///

void ARollingStoneGameMode::StartPlay()
{
	Super::StartPlay();

}

/// Public Functions ///

float ARollingStoneGameMode::GetCurrentTime()
{
	if (GetWorldTimerManager().IsTimerActive(GameTimer))
	{
		return GetWorldTimerManager().GetTimerElapsed(GameTimer) + TimePenalty;
	}
	else
	{
		return TotalTime;
	}
}

void ARollingStoneGameMode::AddTimePenalty(float PenaltyAmount)
{
	TimePenalty += PenaltyAmount;
}

void ARollingStoneGameMode::StartLevel()
{
	if (!LevelStarted)
	{
		LevelStarted = true;

		// Start the game timer
		GetWorldTimerManager().SetTimer(GameTimer, LevelDuration, false);
	}
}

void ARollingStoneGameMode::StopLevel()
{
	// Stop the game timer and record the player's time
	if (GetWorldTimerManager().IsTimerActive(GameTimer))
	{
		TotalTime = GetWorldTimerManager().GetTimerElapsed(GameTimer) + TimePenalty;
		GetWorldTimerManager().ClearTimer(GameTimer);
	}
}

bool ARollingStoneGameMode::IsLevelStarted()
{
	return LevelStarted;
}