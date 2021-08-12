#include "LevelData.h"

void ULevelData::FindMapData(FName MapName, const FWorldData*& World, const FMapData*& Level) const
{
	for (int32 i = 0; i < Worlds.Num(); ++i)
	{
		for (int32 j = 0; j < Worlds[i].Levels.Num(); ++j)
		{
			// Check for matching maps
			if (Worlds[i].Levels[j].MapName == MapName)
			{
				World = &Worlds[i];
				Level = &Worlds[i].Levels[j];
				return;
			}
		}
	}

	// No level matched
	World = nullptr;
	Level = nullptr;
}

void ULevelData::FindNext(FName MapName, const FWorldData*& World, const FMapData*& Level) const
{
	bool found = false;
	for (int32 i = 0; i < Worlds.Num(); ++i)
	{
		for (int32 j = 0; j < Worlds[i].Levels.Num(); ++j)
		{
			// If the last level matched the given name, return the current level
			if (found)
			{
				World = &Worlds[i];
				Level = &Worlds[i].Levels[j];
				return;
			}

			// Check for a matching level
			if (Worlds[i].Levels[j].MapName == MapName)
			{
				found = true;
			}
		}
	}

	// No level matched the current level or we are on the last level
	World = nullptr;
	Level = nullptr;
}