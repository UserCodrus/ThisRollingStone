#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Powerup.generated.h"

class USphereComponent;
class AStone;

// A powerup that the player can collect
UCLASS()
class ROLLINGSTONE_API APowerup : public AActor
{
	GENERATED_BODY()
	
public:	
	APowerup();

	/// Delegates ///

	UFUNCTION()
		void BeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION(BlueprintImplementableEvent)
		void CollectPowerup(AStone* Player);

	/// Components ///

protected:
	// The powerup's mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* Mesh;
	// The powerup's collision
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USphereComponent* CollisionSphere;
};
