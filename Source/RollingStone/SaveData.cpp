#include "SaveData.h"

float USaveData::GetRecord(FName MapName)
{
	// Search the level data for a matching record
	for (int32 i = 0; i < LevelData.Num(); ++i)
	{
		if (LevelData[i].LevelName == MapName)
		{
			return LevelData[i].RecordTime;
		}
	}

	return 0.0f;
}

bool USaveData::StoreRecord(FName MapName, float RecordTime)
{
	// Search level data for a matching record
	for (int32 i = 0; i < LevelData.Num(); ++i)
	{
		if (LevelData[i].LevelName == MapName)
		{
			// Record the new record if it's better than the last record
			if (LevelData[i].RecordTime > RecordTime)
			{
				LevelData[i].RecordTime = RecordTime;
				return true;
			}
			return false;
		}
	}

	// Add a new record
	FLevelState new_level;
	new_level.LevelName = MapName;
	new_level.RecordTime = RecordTime;
	LevelData.Add(new_level);

	return true;
}