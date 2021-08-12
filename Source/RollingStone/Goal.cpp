#include "Goal.h"
#include "Stone.h"

#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"

AGoal::AGoal()
{
	PrimaryActorTick.bCanEverTick = false;

	// Assign delegates
	OnActorBeginOverlap.AddDynamic(this, &AGoal::BeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AGoal::EndOverlap);

	// Create components
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent->Mobility = EComponentMobility::Static;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetBoxExtent(FVector(50.0f));
	CollisionBox->SetCollisionProfileName("OverlapAll");
	CollisionBox->Mobility = EComponentMobility::Static;
	CollisionBox->SetupAttachment(RootComponent);

	Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	Decal->DecalSize = FVector(50.0f, 50.0f, 50.0f);
	Decal->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	Decal->SetupAttachment(CollisionBox);
}

/// Delegates ///

void AGoal::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	// Increment the moss counter on stones that enter the moss
	AStone* stone = Cast<AStone>(OtherActor);
	if (stone != nullptr)
	{
		stone->EnterGoal();
	}
}

void AGoal::EndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	// Decrement the moss counter on stones that enter the moss
	AStone* stone = Cast<AStone>(OtherActor);
	if (stone != nullptr)
	{
		stone->ExitGoal();
	}
}