#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatZone.generated.h"

class UBoxComponent;
class AStone;

// A collsion trigger that makes the player float
UCLASS()
class ROLLINGSTONE_API AFloatZone : public AActor
{
	GENERATED_BODY()
	
public:
	AFloatZone();

	/// Delegates ///

	UFUNCTION()
		void BeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
		void EndOverlap(AActor* OverlappedActor, AActor* OtherActor);

	/// Components ///

protected:
	// The collision box for the trigger
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBoxComponent* CollisionBox;
	// The trigger graphic
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UDecalComponent* Decal;
};
