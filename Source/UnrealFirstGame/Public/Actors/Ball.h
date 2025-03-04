#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

class ATextPopUp;
class UNiagaraComponent;
class USphereComponent;
class UStaticMeshComponent;
class UNiagaraSystem;
class USoundCue;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* TrailVFXComp;
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
	
	void ResetPosition();

	FVector BaseLocation;

	UPROPERTY(EditAnywhere, Category = "InHole")
	UNiagaraSystem* ExplosionVFX;

	UPROPERTY(EditAnywhere, Category = "SFX")
	TArray<USoundCue*> ExplosionSFXList;

	UPROPERTY(EditAnywhere, Category = "SFX")
	TArray<USoundCue*> CoinSFXList;

	void PlayExplosionSFX();
	void PlayCoinSFX();
	
	UPROPERTY(EditAnywhere, Category = "Pop Up")
	TSubclassOf<ATextPopUp> PopUpClass;

	void SpawnPopUp(FText Score);

	void BallInHole(AActor* OtherActor, UPrimitiveComponent* OtherComp);

	void IncrementScore(AActor* OtherActor);

	void IncrementCombo();

	void IncrementHoleMultiplier(AActor* OtherActor);

	void ResetCombo();

	UPROPERTY(EditAnywhere, Category = "Ball Scoring")
	float BasePoint = 1;

	FTimerHandle DelayBeforeReset;

	UPROPERTY(EditAnywhere, Category = "DestroyBall")
	float DelayReset = 2.f;

	void DestroyBall();

	void DestroyBallEffect();

	UPROPERTY(EditAnywhere, Category = "DestroyBall")
	UNiagaraSystem* DestroyBallExplosionVFX;

	void HideBall();

	void HideTrail();

	FTimerHandle DelayHideTrail;

	UPROPERTY(EditAnywhere, Category = "Trail")
	float DelayHide = 1.f;

	
};
