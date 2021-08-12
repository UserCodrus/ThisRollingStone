#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoostZone.generated.h"

class UBoxComponent;

// A collision trigger that adds momentum to the player
UCLASS()
class ROLLINGSTONE_API ABoostZone : public AActor
{
	GENERATED_BODY()
	
public:	
	ABoostZone();

	/// Delegates ///

	UFUNCTION()
		void BeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
		void EndOverlap(AActor* OverlappedActor, AActor* OtherActor);

	/// Properties ///

protected:
	// The direction of the boost relative to the boost zone
	UPROPERTY(EditAnywhere)
		FVector Direction;

	/// Components ///

	// The collision box for the trigger
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBoxComponent* CollisionBox;
};
