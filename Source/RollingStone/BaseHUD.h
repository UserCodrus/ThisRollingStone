#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseHUD.generated.h"

// Basic HUD functions for all game HUDs
UCLASS()
class ROLLINGSTONE_API ABaseHUD : public AHUD
{
	GENERATED_BODY()

public:
	ABaseHUD();

	/// AHUD ///

	virtual void Tick(float DeltaTime) override;
	
	/// Object Interface ///

	// Start UI fade
	void StartFade(float Duration, bool FadeOut);
	// Set the opacity of the UI
	virtual void SetOpacity(float Opacity) {};

	/// Properties ///

protected:
	// Set to true when fading in and false when fading out
	bool FadeIn;
	// The duration of the current fade
	float FadeDuration;

	// The timer that handles fade in and fade out
	FTimerHandle FadeTimer;
};
