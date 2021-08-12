#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Goal.generated.h"

class UBoxComponent;

// The goal the player is aiming for
UCLASS()
class ROLLINGSTONE_API AGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	AGoal();

	/// Delegates ///

	UFUNCTION()
		void BeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
		void EndOverlap(AActor* OverlappedActor, AActor* OtherActor);

	/// Components ///

protected:
	// The collision box for the goal
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBoxComponent* CollisionBox;
	// The goal graphic
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UDecalComponent* Decal;
};
