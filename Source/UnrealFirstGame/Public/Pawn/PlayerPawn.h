#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

class ABall;
class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class UNiagaraSystem;

UCLASS()
class UNREALFIRSTGAME_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	APlayerPawn();
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
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
	
	double PressedTimer;
	
	bool IsPressing;
	
	UPROPERTY(EditAnywhere, Category = "Ball Properties")
	float MaxShootPower;
	
	UPROPERTY(EditAnywhere, Category = "Ball Properties")
	double DelayToMaxShoot;

	UPROPERTY(EditAnywhere, Category = "AimLineVFX")
	UNiagaraSystem* AimLineVFX;

	void CreateLine();
	
	FTimerHandle DelayBeforeNewPoints;
	
	UPROPERTY(EditAnywhere, Category = "AimLineVFX")
	float DelayNewPoints = 1.f;

	float CooldownNewPoints;

	FHitResult HitResult;
	
	void Fire();
};
