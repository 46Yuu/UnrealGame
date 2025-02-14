#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

class ABall;
class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class UNiagaraSystem;
class USoundCue;
class AGameModeBallGame;
class APlayerController;

UCLASS()
class UNREALFIRSTGAME_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	APlayerPawn();
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shoot Properties")
	float PressedLerped = 0.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Shoot Properties")
	FTimerHandle PressedTimerHandle;
	
	UPROPERTY(EditAnywhere, Category = "Shoot Properties")
	float MaxShootPower;
	
	UPROPERTY(EditAnywhere, Category = "Shoot Properties")
	float DelayToMaxShoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shoot Properties")
	float CurrentPressedShootValue;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* PlayerContext;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* FireAction;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* BallSpawnPoint;

	APlayerController* BallPlayerController;

	UPROPERTY(EditAnywhere, Category = "Ball Properties")
	TSubclassOf<ABall> BallClass;
	
	void RotateShootDirection(const FVector& LookAtTarget);
	
	void StartFire();

	ABall* CurrentBall;
	
	bool IsPressing;
	bool IsIncreasing = true;
	

	UPROPERTY(EditAnywhere, Category = "AimLineVFX")
	UNiagaraSystem* AimLineVFX;

	void CreateLine();
	
	FTimerHandle DelayBeforeNewPoints;
	
	UPROPERTY(EditAnywhere, Category = "AimLineVFX")
	float DelayNewPoints = 1.f;

	float CooldownNewPoints;

	FHitResult HitResult;
	
	void Fire();

	UPROPERTY(EditAnywhere, Category = "SFX")
	USoundCue* ShootSFX;
	UPROPERTY(EditAnywhere, Category = "SFX")
	USoundCue* ShootPressSFX;

	void PlayShootSFX();
	void PlayPressShootSFX();


	void IncrementPressedShootValue();

	void DecrementPressedShootValue();

	AGameModeBallGame* GameModeBallGame;

	APlayerController* PlayerController;
};
