#include "BaseController.h"
#include "GameHUD.h"
#include "Stone.h"
#include "RollingStoneGameMode.h"
#include "GameData.h"

#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"

#include "Engine/Engine.h"

ABaseController::ABaseController()
{
	// Set property defaults
	CaptureInput = true;
	FadeOutDuration = 3.0f;
	FadeInDuration = 3.0f;
	ControlDelay = 5.0f;
	InTransition = false;
}

/// ABaseHUD ///

void ABaseController::BeginPlay()
{
	Super::BeginPlay();

	// Lock mouse controls to the game if the mouse isn't visible
	if (CaptureInput)
	{
		GameInputMode();
	}
	else
	{
		MouseInputMode();
	}

	bool music_fade = false;
	UGameData* game_data = GetWorld()->GetGameInstance<UGameData>();
	if (game_data != nullptr)
	{
		// Create or load save data when the game starts
		game_data->Initialize();

		// Create an audio manager for background music
		game_data->CreateAudioComponent();

		// Find the current map's data
		const FWorldData* current_world;
		const FMapData* current_level;
		game_data->GetLevels()->FindMapData(FName(*GetWorld()->GetName()), current_world, current_level);

		if (current_world != nullptr)
		{
			CurrentWorld = *current_world;
		}
		if (current_level != nullptr)
		{
			CurrentMap = *current_level;
		}

		// Get the music transition
		music_fade = game_data->FadeMusic;
		game_data->FadeMusic = false;
	}

	// Fade in the camera
	if (FadeInDuration > 0.0f)
	{
		// Start the camera fade in, only fade in music if it isn't starting when controls are enabled
		PlayerCameraManager->StartCameraFade(1.0f, 0.0f, FadeInDuration, FLinearColor::Black, music_fade, false);

		// Start the UI fade in
		ABaseHUD* hud = Cast<ABaseHUD>(GetHUD());
		if (hud != nullptr)
		{
			hud->StartFade(FadeInDuration, false);
		}
	}

	// Delay activation of player controls
	if (ControlDelay > 0.0f)
	{
		GetWorldTimerManager().SetTimer(ControlTimer, this, &ABaseController::EnableControls, ControlDelay, false);
	}
	else
	{
		EnableControls();
	}
}

/// Object Interface ///

void ABaseController::LevelComplete(bool Failed)
{
	// Don't finish during level transitions
	if (InTransition)
		return;

	// Disable player controls and notify the player if the level was finished
	AStone* stone = Cast<AStone>(GetPawn());
	if (stone != nullptr)
	{
		stone->DisableControls();
		stone->NotifyLevelComplete(Failed);
	}

	// Stop the level timer
	float level_time = 0.0f;
	ARollingStoneGameMode* mode = GetWorld()->GetAuthGameMode<ARollingStoneGameMode>();
	if (mode != nullptr)
	{
		mode->StopLevel();
		level_time = mode->GetCurrentTime();
	}

	if (Failed)
	{
		// Show the level failed screen
		AGameHUD* hud = Cast<AGameHUD>(GetHUD());
		if (hud != nullptr)
		{
			hud->ShowFailedWidget();
		}
	}
	else
	{
		// Show the end level screen
		AGameHUD* hud = Cast<AGameHUD>(GetHUD());
		if (hud != nullptr)
		{
			hud->ShowFinishWidget();
		}

		// Save the current level time
		UGameData* game_data = GetWorld()->GetGameInstance<UGameData>();
		if (game_data != nullptr)
		{
			game_data->SaveCurrentRecord(level_time);
		}
	}

	// Change to mouse input
	DualInputMode();
}

void ABaseController::LevelTransition(FName LevelName, bool FadeMusic)
{
	// Don't overlap transitions
	if (InTransition)
		return;

	// Unpause the game if needed
	AGameHUD* game_hud = Cast<AGameHUD>(GetHUD());
	if (game_hud != nullptr)
	{
		if (IsPaused())
		{
			SetPause(false);
			game_hud->TogglePauseWidget(false);
		}
	}

	// Restart the level if no level name is provided
	if (LevelName.IsNone())
	{
		LevelName = FName(*GetWorld()->GetName());
	}

	if (!GetWorldTimerManager().IsTimerActive(FadeTimer))
	{
		// Store the map name
		PendingMap = LevelName;

		if (FadeOutDuration > 0.0f)
		{
			// Start the camera fade out
			PlayerCameraManager->StartCameraFade(0.0f, 1.0f, FadeOutDuration, FLinearColor::Black, FadeMusic, true);

			// Store the fade in for the start of the next level
			UGameData* game_data = GetWorld()->GetGameInstance<UGameData>();
			if (game_data != nullptr)
			{
				game_data->FadeMusic = FadeMusic;
			}

			// Start the level transition timer
			GetWorldTimerManager().SetTimer(FadeTimer, this, &ABaseController::ChangeLevel, FadeOutDuration, false);

			// Start the UI fade out
			ABaseHUD* base_hud = Cast<ABaseHUD>(GetHUD());
			if (base_hud != nullptr)
			{
				base_hud->StartFade(FadeOutDuration, true);
			}

			InTransition = true;
		}
		else
		{
			ChangeLevel();
		}
	}
}

void ABaseController::NextLevel()
{
	// Default to going back to the main menu
	FName next_map("MainMenu");
	bool change_music = true;

	UGameData* game_data = GetWorld()->GetGameInstance<UGameData>();
	if (game_data != nullptr)
	{
		// Find the next map
		const FWorldData* next_world;
		const FMapData* next_level;
		game_data->GetLevels()->FindNext(FName(*GetWorld()->GetName()), next_world, next_level);
		if (next_world != nullptr && next_level != nullptr)
		{
			// Set the map data
			change_music = (game_data->GetMusic() != next_world->BackgroundMusic);
			next_map = next_level->MapName;
		}
	}

	// Start the level transition
	LevelTransition(next_map, change_music);
}

void ABaseController::TogglePause()
{
	// Don't pause during transitions
	if (InTransition)
		return;

	AGameHUD* hud = Cast<AGameHUD>(GetHUD());
	if (hud != nullptr)
	{
		// Toggle pause state, widgets, and input modes
		if (IsPaused())
		{
			SetPause(false);
			hud->TogglePauseWidget(false);
			GameInputMode();
		}
		else
		{
			SetPause(true);
			hud->TogglePauseWidget(true);
			DualInputMode();
		}
	}
}

void ABaseController::AddTimePenalty(float PenaltyAmount)
{
	ARollingStoneGameMode* mode = GetWorld()->GetAuthGameMode<ARollingStoneGameMode>();
	if (mode != nullptr)
	{
		mode->AddTimePenalty(PenaltyAmount);
	}
}

void ABaseController::MouseInputMode()
{
	// Show the mouse
	bShowMouseCursor = true;

	// Lock input to the UI
	SetInputMode(FInputModeUIOnly());
}

void ABaseController::GameInputMode()
{
	// Hide the mouse
	bShowMouseCursor = false;

	// Lock input to the game
	SetInputMode(FInputModeGameOnly());
}

void ABaseController::DualInputMode()
{
	// Show the mouse
	bShowMouseCursor = true;

	// Allow game and UI input
	SetInputMode(FInputModeGameAndUI());
}

/// Internal Functions ///

void ABaseController::ChangeLevel()
{
	// Stop music
	UGameData* game_data = GetWorld()->GetGameInstance<UGameData>();
	if (game_data != nullptr)
	{
		if (game_data->FadeMusic)
		{
			game_data->StopMusic();
		}
	}

	// Load the next level
	UGameplayStatics::OpenLevel(GetWorld(), PendingMap);
}

void ABaseController::EnableControls()
{
	// Enable player controls
	AStone* stone = Cast<AStone>(GetPawn());
	if (stone != nullptr)
	{
		stone->EnableControls();
	}

	// Start the level timer
	ARollingStoneGameMode* mode = GetWorld()->GetAuthGameMode<ARollingStoneGameMode>();
	if (mode != nullptr)
	{
		mode->StartLevel();
	}

	// Start playing background music
	UGameData* game_data = GetWorld()->GetGameInstance<UGameData>();
	if (game_data != nullptr)
	{
		game_data->PlayMusic(CurrentWorld.BackgroundMusic);
	}
}