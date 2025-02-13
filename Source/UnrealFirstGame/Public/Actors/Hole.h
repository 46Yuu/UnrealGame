#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hole.generated.h"
class UNiagaraComponent;
class UTextRenderComponent;
class USphereComponent;
class UBoxComponent;
class UStaticMeshComponent;

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

	FTimerHandle MoveTimerHandle;
	
	UPROPERTY(EditAnywhere, Category = "Hole Properties")
	float MoveDelay = 1.f;

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
	
	UPROPERTY(EditAnywhere, Category = "Ball Scoring")
	float DelayResetMultiplier = 5.f;
	
	float CurrentResetMultiplierTimer = 0.f;
	bool IsResetting = false;


};
