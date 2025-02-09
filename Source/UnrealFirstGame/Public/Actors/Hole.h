#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hole.generated.h"
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

	FTimerHandle MoveTimerHandle;
	
	UPROPERTY(EditAnywhere, Category = "Hole Properties")
	float MoveDelay = 1.f;

	UPROPERTY(EditAnywhere, Category = "Hole Properties")
	float MoveSpeed = 2.f;

	UPROPERTY(EditAnywhere, Category = "Checkpoints")
	TArray<FVector> CheckPointLocationList;
	
	int CurrentCheckPointIndex = 0;

	FVector Destination;
	FVector StartLocation;
	float TotalDistance;
	float CurrentDistance;
	bool IsMoving = false;
	
	void MoveTowardsNextPoint();

};
