#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Moss.generated.h"

class USphereComponent;
class UDecalComponent;

// A moss hazard
UCLASS()
class ROLLINGSTONE_API AMoss : public AActor
{
	GENERATED_BODY()
	
public:	
	AMoss();

	/// AActor ///

protected:
	virtual void BeginPlay() override;

	/// Delegates ///

public:
	UFUNCTION()
		void BeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
		void EndOverlap(AActor* OverlappedActor, AActor* OtherActor);

	/// Components ///

protected:
	// The collision box for the moss
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USphereComponent* CollisionBox;
	// The moss decal
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UDecalComponent* Decal;
};
