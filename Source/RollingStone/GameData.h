#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameData.generated.h"

class UAudioComponent;
class ULevelData;
class USaveData;

// Data for music and level scores
UCLASS(Blueprintable)
class ROLLINGSTONE_API UGameData : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UGameData();

	/// Public Interface ///

	// Attach an audio component to the game instance
	void CreateAudioComponent();
	// Start playing a music track or play the default track if one isn't specified
	void PlayMusic(USoundBase* Music = nullptr);
	// Stop playing the current music track
	void StopMusic();
	// Get the current music playing
	USoundBase* GetMusic();

	// Initialize save data
	void Initialize();
	// Save a level record
	void SaveRecord(FName MapName, float Time);
	// Save a record for the current level
	void SaveCurrentRecord(float Time);
	// Retrieve a level record
	UFUNCTION(BlueprintPure)
		float GetRecord(FName MapName);
	// Retrieve the current level's record
	UFUNCTION(BlueprintPure)
		float GetCurrentRecord();

	/// Public Properties ///

	// Set to true when music needs to fade in during a level transition
	UPROPERTY(EditAnywhere)
		bool FadeMusic;

	/// Acccessors ///

	FORCEINLINE UAudioComponent* GetMusicPlayer() { return MusicPlayer; }
	FORCEINLINE ULevelData* GetLevels() { return LevelData; }

	/// Properties ///

protected:
	// The audio component that handles background music
	UPROPERTY()
		UAudioComponent* MusicPlayer;

	// The list of levels the player can access
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		ULevelData* LevelData;

	// The name of the save game
	UPROPERTY(Category = "Save Data", EditDefaultsOnly)
		FString SaveName;
	// The slot to save to
	UPROPERTY(Category = "Save Data", EditInstanceOnly)
		int32 SaveSlot;
	// The current save data
	UPROPERTY(Category = "Save Data", EditInstanceOnly)
		USaveData* SaveData;

	// The default music to play in levels when no music is provided
	UPROPERTY(Category = "Music", EditAnywhere)
		USoundBase* DefaultMusic;
	// If set to false, music will be disabled
	UPROPERTY(Category = "Music", EditAnywhere)
		bool AllowMusic;
};
