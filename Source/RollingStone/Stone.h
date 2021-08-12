#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Stone.generated.h"

class USpringArmComponent;
class UCameraComponent;

// The player pawn
UCLASS()
class ROLLINGSTONE_API AStone : public APawn
{
	GENERATED_BODY()

public:
	AStone();

	/// APawn ///

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	/// Input Functions ///

	void MoveRight(float Value);
	void MoveForward(float Value);
	void CameraHorizontal(float Value);
	void CameraVertical(float Value);
	void Jump();
	void Reset();
	void Pause();

	/// Public Interface ///

public:
	// Called when the stone enters a moss actor
	void EnterMoss();
	// Called when the stone exits a moss actor
	void ExitMoss();

	// Called when the stone enters the goal zone
	void EnterGoal();
	// Called when the stone exits the goal zone
	void ExitGoal();

	// Called after the level countdown ends to enable controls
	void EnableControls();
	// Called on level end to remove control from the player pawn
	void DisableControls();

	// Notify the player that the level has been ended
	void NotifyLevelComplete(bool Failed);

	/// Internal Functions ///

protected:
	// Called when the level is completed or failed
	void FinishLevel(bool Failed);
	// Called when the player's moss immunity ends
	void EndImmunity();

	/// Blueprint Interface ///

public:
	// Allows the player to do a single jump
	UFUNCTION(BlueprintCallable)
		void AddJump();
	// Enter the float state
	UFUNCTION(BlueprintCallable)
		void StartFloat();
	// Exit the float state
	UFUNCTION(BlueprintCallable)
		void StopFloat();
	// Apply a boost to the player
	UFUNCTION(BlueprintCallable)
		void ApplyBoost(FVector Direction);
	// Remove a boost from the player
	UFUNCTION(BlueprintCallable)
		void RemoveBoost(FVector Direction);

	/// Properties ///

protected:
	// Set to true when controls are usable
	UPROPERTY(EditAnywhere, Category = "Status")
		bool ControlEnabled;
	// Set to true when camera movement is enabled
	UPROPERTY(EditAnywhere, Category = "Status")
		bool CameraEnabled;
	// Set to true when the level is complete
	UPROPERTY(EditAnywhere, Category = "Status")
		bool FinishedLevel;
	// Set to true if the player won the level
	UPROPERTY(EditAnywhere, Category = "Status")
		bool PassedLevel;

	// Set to true when the player can jump
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
		bool CanJump;
	// Set to true when the player can boost
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
		bool CanBoost;
	// Set to true when inside the goal zone
	UPROPERTY(EditAnywhere, Category = "Status")
		bool InsideGoal;

	// The number of moss layers the stone has passed through
	UPROPERTY(EditAnywhere, Category = "Status")
		int32 MossCounter;
	// Set to false after touching moss to provide iframes
	UPROPERTY(EditAnywhere, Category = "Status")
		bool CanGatherMoss;

	// The float counter used to track when the player is inside float zones
	UPROPERTY(EditAnywhere, Category = "Status")
		int32 FloatCounter;
	// The direction the player is currently boosting
	UPROPERTY(EditAnywhere, Category = "Status")
		FVector BoostDirection;

	// The amount of time lost when rolling through moss
	UPROPERTY(EditAnywhere, Category = "Attributes")
		float MossPenalty;
	// The amount of time the player will be immune to moss after touching it
	UPROPERTY(EditAnywhere, Category = "Attributes")
		float MossImmunity;
	// If set to true, the player can jump every time they touch the ground
	UPROPERTY(EditAnywhere, Category = "Attributes")
		bool AllowJumping;

	// The force of the player's jump
	UPROPERTY(EditAnywhere, Category = "Attributes")
		float JumpForce;
	// The force of the player's dash
	UPROPERTY(EditAnywhere, Category = "Attributes")
		float DashForce;
	// The player's movement speed
	UPROPERTY(EditAnywhere, Category = "Attributes")
		float MovementForce;
	// The velocity applied when floating
	UPROPERTY(EditAnywhere, Category = "Attributes")
		float FloatVelocity;
	// The velocity applied when boosting
	UPROPERTY(EditAnywhere, Category = "Attributes")
		float BoostVelocity;

	// If set to true, the stone will die when it goes under water
	UPROPERTY(EditAnywhere, Category = "Attributes")
		bool WaterKills;

	// The camera's horizontal turn rate
	UPROPERTY(EditAnywhere, Category = "Attributes")
		float CameraHorizontalSpeed;
	// The camera's vertical turn rate
	UPROPERTY(EditAnywhere, Category = "Attributes")
		float CameraVerticalSpeed;

	// The water plane actor
	UPROPERTY(EditAnywhere, Category = "Water")
		AActor* WaterPlane;
	// The maximum distance from the water the player can be before failing
	UPROPERTY(EditAnywhere, Category = "Water")
		float WaterOffset;
	// The offset of the camera compared to the water level
	UPROPERTY(EditAnywhere, Category = "Water")
		float CameraOffset;

	/// Components ///

	// The object's mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* Mesh;
	// The camera arm
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USpringArmComponent* SpringArm;
	// The game camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UCameraComponent* Camera;

	// The timer used to measure moss immunity
	FTimerHandle ImmunityTimer;
};
