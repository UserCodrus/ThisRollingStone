#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

class AMainMenuHUD;

// A main menu widget
UCLASS()
class ROLLINGSTONE_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
	/// Object Interface ///

public:
	// Set the parent of the widget
	void SetOwner(AMainMenuHUD* NewParent);

	/// Properties ///

protected:
	// The HUD that controls this widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AMainMenuHUD* Owner;
};
