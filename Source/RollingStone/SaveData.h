#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveData.generated.h"

// Data for a completed level
USTRUCT(BlueprintType)
struct FLevelState
{
	GENERATED_BODY()

	// The name of the level
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName LevelName;
	// The player's best time
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float RecordTime = 0.0f;
};

// Save game data
UCLASS()
class ROLLINGSTONE_API USaveData : public USaveGame
{
	GENERATED_BODY()

public:
	// Get the best time for a given level
	float GetRecord(FName MapName);
	// Store a record in the level data table
	bool StoreRecord(FName MapName, float RecordTime);
	
protected:
	// Data for each level the player has completed
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FLevelState> LevelData;
};
