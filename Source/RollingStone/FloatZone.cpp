#include "FloatZone.h"
#include "Stone.h"

#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"

AFloatZone::AFloatZone()
{
	PrimaryActorTick.bCanEverTick = false;

	// Assign delegates
	OnActorBeginOverlap.AddDynamic(this, &AFloatZone::BeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AFloatZone::EndOverlap);

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

void AFloatZone::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	// Toggle floating state
	AStone* stone = Cast<AStone>(OtherActor);
	if (stone != nullptr)
	{
		stone->StartFloat();
	}
}

void AFloatZone::EndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	// Toggle floating state
	AStone* stone = Cast<AStone>(OtherActor);
	if (stone != nullptr)
	{
		stone->StopFloat();
	}
}
