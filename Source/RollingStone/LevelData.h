#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelData.generated.h"

// Data for a single game level
USTRUCT(BlueprintType)
struct ROLLINGSTONE_API FMapData
{
	GENERATED_BODY()

	// The name of the map
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName MapName;

	// The display name for the level
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText DisplayName;
	// The thumbnail for the level
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UTexture2D* Thumbnail = nullptr;
};

// Data for a collection of stages
USTRUCT(BlueprintType)
struct ROLLINGSTONE_API FWorldData
{
	GENERATED_BODY()

	// The display name for the world
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText DisplayName;
	// The music that will play in the world
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		USoundBase* BackgroundMusic = nullptr;

	// The levels contained in the world
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FMapData> Levels;
};

// A data asset that stores level data
UCLASS(BlueprintType)
class ROLLINGSTONE_API ULevelData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// Find the map data with the given name
	void FindMapData(FName MapName, const FWorldData*& World, const FMapData*& Level) const;
	// Find the next map following the given map
	void FindNext(FName MapName, const FWorldData*& World, const FMapData*& Level) const;

protected:
	// The data for game maps
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FWorldData> Worlds;
};
