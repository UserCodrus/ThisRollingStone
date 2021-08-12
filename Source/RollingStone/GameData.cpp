#include "GameData.h"
#include "SaveData.h"

#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "AudioDevice.h"

#include "Engine/Engine.h"

UGameData::UGameData()
{
	// Set property defaults
	AllowMusic = true;
	FadeMusic = true;
	SaveName = "TRSSave";
	SaveSlot = 0;
}

/// Public Interface ///

void UGameData::CreateAudioComponent()
{
	if (MusicPlayer == nullptr && AllowMusic)
	{
		// Create the music player and make it persist across level transitions
		MusicPlayer = FAudioDevice::CreateComponent(DefaultMusic, FAudioDevice::FCreateComponentParams(GetWorld()->GetAudioDevice().GetAudioDevice()));
		MusicPlayer->bAllowSpatialization = false;
		MusicPlayer->bIsUISound = true;
		MusicPlayer->bAutoDestroy = false;
		MusicPlayer->bIgnoreForFlushing = true;
	}
}

void UGameData::PlayMusic(USoundBase* Music)
{
	// Set the default music if no music is provided
	if (Music == nullptr)
	{
		if (DefaultMusic != nullptr)
		{
			Music = DefaultMusic;
		}
		else
		{
			return;
		}
	}

	// Restart the music player if the provided track is different from the one being played
	if (MusicPlayer != nullptr)
	{
		if (MusicPlayer->Sound != Music)
		{
			MusicPlayer->Stop();
			MusicPlayer->SetSound(Music);
		}

		if (!MusicPlayer->IsPlaying() && AllowMusic)
		{
			MusicPlayer->Play();
		}
	}
}

void UGameData::StopMusic()
{
	if (MusicPlayer != nullptr)
	{
		MusicPlayer->Stop();
	}
}

USoundBase* UGameData::GetMusic()
{
	if (MusicPlayer != nullptr)
	{
		return MusicPlayer->Sound;
	}

	return nullptr;
}

void UGameData::Initialize()
{
	if (SaveData == nullptr)
	{
		// Try to load save data
		SaveData = Cast<USaveData>(UGameplayStatics::LoadGameFromSlot(SaveName, SaveSlot));

		// Create new save data
		if (SaveData == nullptr)
		{
			SaveData = Cast<USaveData>(UGameplayStatics::CreateSaveGameObject(USaveData::StaticClass()));
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "New save created");
		}
	}
}

void UGameData::SaveRecord(FName MapName, float Time)
{
	if (SaveData->StoreRecord(MapName, Time))
	{
		UGameplayStatics::SaveGameToSlot(SaveData, SaveName, SaveSlot);
	}
}

void UGameData::SaveCurrentRecord(float Time)
{
	if (SaveData->StoreRecord(FName(*GetWorld()->GetName()), Time))
	{
		UGameplayStatics::SaveGameToSlot(SaveData, SaveName, SaveSlot);
	}
}

float UGameData::GetRecord(FName MapName)
{
	return SaveData->GetRecord(MapName);
}

float UGameData::GetCurrentRecord()
{
	return SaveData->GetRecord(FName(*GetWorld()->GetName()));
}