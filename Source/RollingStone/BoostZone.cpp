#include "BoostZone.h"
#include "Stone.h"

#include "Components/BoxComponent.h"

ABoostZone::ABoostZone()
{
	PrimaryActorTick.bCanEverTick = false;

	// Assign delegates
	OnActorBeginOverlap.AddDynamic(this, &ABoostZone::BeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ABoostZone::EndOverlap);

	// Create components
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent->Mobility = EComponentMobility::Static;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetBoxExtent(FVector(50.0f));
	CollisionBox->SetCollisionProfileName("OverlapAll");
	CollisionBox->Mobility = EComponentMobility::Static;
	CollisionBox->SetupAttachment(RootComponent);

	// Set property defaults
	Direction = FVector(1.0f, 0.0f, 0.0f);
}

void ABoostZone::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	// Toggle boosting state
	AStone* stone = Cast<AStone>(OtherActor);
	if (stone != nullptr)
	{
		stone->ApplyBoost(GetActorRotation().RotateVector(Direction));
	}
}

void ABoostZone::EndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	// Toggle boosting state
	AStone* stone = Cast<AStone>(OtherActor);
	if (stone != nullptr)
	{
		stone->RemoveBoost(GetActorRotation().RotateVector(Direction));
	}
}
