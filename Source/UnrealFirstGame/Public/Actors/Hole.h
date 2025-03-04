#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hole.generated.h"
class UNiagaraComponent;
class UTextRenderComponent;
class USphereComponent;
class UBoxComponent;
class UStaticMeshComponent;
class AGameModeBallGame;

UCLASS()
class UNREALFIRSTGAME_API AHole : public AActor
{
	GENERATED_BODY()
	
public:	
	AHole();
	virtual void Tick(float DeltaTime) override;

	int CurrentMultiplier;

	void UpdateTextRenderer();
	void StartIsReseting();

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Hole Properties")
	float MoveDelay = 1.f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Hole Properties")
	FTimerHandle MoveTimerHandle;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Ball Scoring")
	float DelayResetMultiplier = 5.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Ball Scoring")
	FTimerHandle DelayResetTimerHandle;
	
protected:
	virtual void BeginPlay() override;

private:	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* HoleCollisionMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* CylinderComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* HoleMask;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* TextRenderComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* TextRenderCooldownComponent;
	

	UPROPERTY(EditAnywhere, Category = "Hole Properties")
	float MoveSpeed = 2.f;

	UPROPERTY(EditAnywhere, Category = "Checkpoints")
	FVector BottomLeftPoint;
	
	UPROPERTY(EditAnywhere, Category = "Checkpoints")
	FVector TopLeftPoint;
	
	UPROPERTY(EditAnywhere, Category = "Checkpoints")
	FVector BottomRightPoint;

	UPROPERTY(EditAnywhere, Category = "Checkpoints")
	int CheckPointsCount; 
	
	UPROPERTY(EditAnywhere, Category = "Checkpoints")
	TArray<FVector> CheckPointLocationList;

	void CreateRandomCheckpoints();
	
	int CurrentCheckPointIndex = 0;
	
	UPROPERTY(EditAnywhere, Category = "Checkpoints")
	bool IsCheckpointRandom = false;

	FVector CreateNextRandomCheckpoint();
	FVector Destination;
	FVector StartLocation;
	float TotalDistance;
	float CurrentDistance;
	bool IsMoving = false;
	
	void MoveTowardsNextPoint();
	
	UPROPERTY(EditAnywhere, Category = "Colors Combo")
	TArray<FColor> ColorMultipliers;
	UPROPERTY(EditAnywhere, Category = "Colors Combo")
	TArray<UNiagaraComponent*> HoleNiagaraList;

	UPROPERTY(EditAnywhere, Category = "Ball Scoring")
	int BaseMultiplier = 1;
	

	
	float CurrentResetMultiplierTimer = 0.f;
	bool IsResetting = false;

	void UpdateTextCooldownRender();
	
	void CountDownResetCooldown();
	void ResetMultiplier();

	AGameModeBallGame* GameModeBallGame;
};
