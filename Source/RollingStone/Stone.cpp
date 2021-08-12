#include "Stone.h"
#include "BaseController.h"
#include "WaterPlane.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/CollisionProfile.h"
#include "Kismet/GameplayStatics.h"

AStone::AStone()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create components
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetAngularDamping(0.1f);
	Mesh->SetLinearDamping(0.1f);
	Mesh->BodyInstance.MassScale = 3.5f;
	Mesh->BodyInstance.MaxAngularVelocity = 800.0f;
	Mesh->SetNotifyRigidBodyCollision(true);
	RootComponent = Mesh;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 1200.f;
	SpringArm->SetUsingAbsoluteLocation(true);
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = false;
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// Set defaults
	ControlEnabled = false;
	CameraEnabled = true;
	FinishedLevel = false;
	PassedLevel = false;
	CanJump = false;
	InsideGoal = false;
	MossCounter = 0;
	CanGatherMoss = true;
	FloatCounter = 0;
	BoostDirection = FVector::ZeroVector;

	CameraOffset = 0.0f;
	MossPenalty = 0.25f;
	MossImmunity = 0.25f;
	AllowJumping = false;
	JumpForce = 200000.0f;
	DashForce = 200000.0;
	MovementForce = 200000.0;
	FloatVelocity = 2000.0f;
	BoostVelocity = 2000.0f;
	CameraHorizontalSpeed = 45.0f;
	CameraVerticalSpeed = 45.0f;
	WaterOffset = 0.0f;
	WaterKills = true;
}

/// APawn ///

void AStone::Tick(float DeltaTime)
{
	// Apply force in the direction of movement
	if (Mesh->IsSimulatingPhysics())
	{
		Mesh->AddImpulse(ConsumeMovementInputVector().GetClampedToMaxSize(1.0f) * MovementForce * DeltaTime);

		// Apply floating force
		if (FloatCounter > 0)
		{
			Mesh->AddForce(FVector(0.0f, 0.0f, 1.0f) * FloatVelocity, NAME_None, true);
		}

		// Apply boosts
		if (!BoostDirection.IsNearlyZero())
		{
			Mesh->AddForce(BoostDirection.GetSafeNormal() * BoostVelocity, NAME_None, true);
		}
	}

	// Check to see if the player is under the water plane
	if (WaterPlane != nullptr)
	{
		if (GetActorLocation().Z < WaterPlane->GetActorLocation().Z + WaterOffset && WaterKills)
		{
			// Hide the player and disable physics
			Mesh->SetSimulatePhysics(false);
			SetActorHiddenInGame(true);

			// End the level
			FinishLevel(true);
		}

		// Reposition the spring arm
		FVector location = GetActorLocation();
		if (location.Z <= WaterPlane->GetActorLocation().Z + CameraOffset)
		{
			location.Z = WaterPlane->GetActorLocation().Z + CameraOffset;
		}
		SpringArm->SetWorldLocation(location);
	}
}

void AStone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Add input bindings
	PlayerInputComponent->BindAxis("MoveRight", this, &AStone::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &AStone::MoveForward);
	PlayerInputComponent->BindAxis("CameraHorizontal", this, &AStone::CameraHorizontal);
	PlayerInputComponent->BindAxis("CameraVertical", this, &AStone::CameraVertical);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AStone::Jump);
	PlayerInputComponent->BindAction("Reset", IE_Pressed, this, &AStone::Reset).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AStone::Pause).bExecuteWhenPaused = true;
}

void AStone::BeginPlay()
{
	Super::BeginPlay();

	// Get a reference to the water plane in the level
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaterPlane::StaticClass(), actors);
	if (actors.Num() > 0)
	{
		WaterPlane = actors[0];
	}
}

void AStone::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Reset the jump flag
	if (AllowJumping)
	{
		CanJump = true;
	}

	// Gather some moss
	if (MossCounter > 0 && CanGatherMoss)
	{
		// Add a time penalty
		ABaseController* controller = Cast<ABaseController>(GetController());
		if (controller != nullptr)
		{
			controller->AddTimePenalty(MossPenalty);
		}

		// Make the player immune to moss for a while
		CanGatherMoss = false;
		GetWorldTimerManager().SetTimer(ImmunityTimer, this, &AStone::EndImmunity, MossImmunity, false);
	}

	// Finish the level
	if (InsideGoal)
	{
		FinishLevel(false);
	}
}

/// Input Functions ///

void AStone::MoveRight(float Value)
{
	if (!ControlEnabled)
		return;

	// Get the right vector
	FVector direction = FRotationMatrix(FRotator(0, Controller->GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::Y);
	AddMovementInput(direction, Value);
}

void AStone::MoveForward(float Value)
{
	if (!ControlEnabled)
		return;

	// Get the forward vector
	FVector direction = FRotationMatrix(FRotator(0, Controller->GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::X);
	AddMovementInput(direction, Value);
}

void AStone::CameraHorizontal(float Value)
{
	if (!CameraEnabled)
		return;

	AddControllerYawInput(Value * CameraHorizontalSpeed * GetWorld()->GetDeltaSeconds());
}

void AStone::CameraVertical(float Value)
{
	if (!CameraEnabled)
		return;

	AddControllerPitchInput(Value * CameraVerticalSpeed * GetWorld()->GetDeltaSeconds());
}

void AStone::Jump()
{
	if (FinishedLevel && PassedLevel)
	{
		ABaseController* controller = Cast<ABaseController>(GetController());
		if (controller != nullptr)
		{
			controller->NextLevel();
		}
		return;
	}
	else if (!ControlEnabled)
	{
		return;
	}

	if (CanJump)
	{
		Mesh->AddImpulse(FVector(0.0f, 0.0f, JumpForce));
		CanJump = false;
	}
}

void AStone::Reset()
{
	if (!ControlEnabled && !FinishedLevel)
		return;

	ABaseController* controller = Cast<ABaseController>(GetController());
	if (controller != nullptr)
	{
		controller->LevelTransition();
	}
}

void AStone::Pause()
{
	if (!ControlEnabled)
		return;

	ABaseController* controller = Cast<ABaseController>(GetController());
	if (controller != nullptr)
	{
		controller->TogglePause();
	}
}

/// Public Interface ///

void AStone::EnterMoss()
{
	++MossCounter;
}

void AStone::ExitMoss()
{
	--MossCounter;
}

void AStone::EnterGoal()
{
	InsideGoal = true;
}

void AStone::ExitGoal()
{
	InsideGoal = false;
}

void AStone::EnableControls()
{
	ControlEnabled = true;
	CameraEnabled = true;
}

void AStone::DisableControls()
{
	ControlEnabled = false;
	CameraEnabled = false;
}

void AStone::NotifyLevelComplete(bool Failed)
{
	FinishedLevel = true;
	PassedLevel = !Failed;
}

/// Internal Functions ///

void AStone::FinishLevel(bool Failed)
{
	InsideGoal = false;

	// Trigger the level end sequence
	if (ControlEnabled)
	{
		ABaseController* controller = Cast<ABaseController>(GetController());
		if (controller != nullptr)
		{
			controller->LevelComplete(Failed);
		}
	}
}

void AStone::EndImmunity()
{
	CanGatherMoss = true;
}

/// Blueprint Interface ///

void AStone::AddJump()
{
	CanJump = true;
}

void AStone::StartFloat()
{
	FloatCounter++;
}

void AStone::StopFloat()
{
	FloatCounter--;
}

void AStone::ApplyBoost(FVector Direction)
{
	BoostDirection += Direction;
}

void AStone::RemoveBoost(FVector Direction)
{
	BoostDirection -= Direction;
}