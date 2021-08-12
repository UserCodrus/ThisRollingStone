#include "Moss.h"
#include "Stone.h"

#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"

AMoss::AMoss()
{
	PrimaryActorTick.bCanEverTick = false;

	// Assign delegates
	OnActorBeginOverlap.AddDynamic(this, &AMoss::BeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AMoss::EndOverlap);

	// Create components
	CollisionBox = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionBox->SetSphereRadius(50.0f);
	CollisionBox->SetCollisionProfileName("OverlapAll");
	CollisionBox->Mobility = EComponentMobility::Static;
	RootComponent = CollisionBox;

	Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	Decal->DecalSize = FVector(50.0f, 50.0f, 50.0f);
	Decal->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	Decal->SetupAttachment(CollisionBox);
}

/// AActor ///

void AMoss::BeginPlay()
{
	Super::BeginPlay();
	
}

/// Delegates ///

void AMoss::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	// Increment the moss counter on stones that enter the moss
	AStone* stone = Cast<AStone>(OtherActor);
	if (stone != nullptr)
	{
		stone->EnterMoss();
	}
}

void AMoss::EndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	// Decrement the moss counter on stones that enter the moss
	AStone* stone = Cast<AStone>(OtherActor);
	if (stone != nullptr)
	{
		stone->ExitMoss();
	}
}