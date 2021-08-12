#include "Powerup.h"
#include "Stone.h"

#include "Components/SphereComponent.h"

APowerup::APowerup()
{
	PrimaryActorTick.bCanEverTick = false;

	OnActorBeginOverlap.AddDynamic(this, &APowerup::BeginOverlap);

	// Create components
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PowerupCollision"));
	CollisionSphere->SetSphereRadius(50.0f);
	CollisionSphere->SetCollisionProfileName("OverlapAll");
	RootComponent = CollisionSphere;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PowerupMesh"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetupAttachment(RootComponent);
}

/// Delegates ///

void APowerup::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	// Collect the powerup then destroy it
	AStone* stone = Cast<AStone>(OtherActor);
	if (stone != nullptr)
	{
		CollectPowerup(stone);
		Destroy();
	}
}