#include "BaseHUD.h"

ABaseHUD::ABaseHUD()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Set defaults
	FadeIn = true;
	FadeDuration = 0.0f;
}

/// AHUD ///

void ABaseHUD::Tick(float DeltaTime)
{
	// Fade UI elements when the fade timer is active
	if (GetWorldTimerManager().IsTimerActive(FadeTimer))
	{
		float opacity;
		if (FadeIn)
		{
			opacity = GetWorldTimerManager().GetTimerElapsed(FadeTimer) / GetWorldTimerManager().GetTimerRate(FadeTimer);
		}
		else
		{
			opacity = GetWorldTimerManager().GetTimerRemaining(FadeTimer) / GetWorldTimerManager().GetTimerRate(FadeTimer);
		}
		SetOpacity(opacity);
	}
}

/// Object Interface ///

void ABaseHUD::StartFade(float Duration, bool FadeOut)
{
	FadeIn = !FadeOut;
	GetWorldTimerManager().SetTimer(FadeTimer, Duration, false);
}