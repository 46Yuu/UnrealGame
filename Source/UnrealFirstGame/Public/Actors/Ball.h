#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UNiagaraSystem;

UCLASS()
class UNREALFIRSTGAME_API ABall : public AActor
{
	GENERATED_BODY()
	
public:	
	ABall();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereComp;
	//virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

private:	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(class UPrimitiveComponent* Comp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	void ResetPosition();

	FVector BaseLocation;

	UPROPERTY(EditAnywhere, Category = "InHole")
	UNiagaraSystem* ExplosionVFX;

	FTimerHandle DelayBeforeReset;

	UPROPERTY(EditAnywhere, Category = "InHole")
	float DelayReset = 2.f;

};
