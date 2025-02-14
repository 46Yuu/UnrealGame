#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TextPopUp.generated.h"

class UTextRenderComponent;
class UNiagaraSystem;
class USoundCue;


UCLASS()
class UNREALFIRSTGAME_API ATextPopUp : public AActor
{
	GENERATED_BODY()
	
public:	
	ATextPopUp();
	virtual void Tick(float DeltaTime) override;

	void SetPopUpText(FText Text);
	void SetPopUpColor(FColor Color);
	

protected:
	virtual void BeginPlay() override;

private:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* TextRenderComponent;

	UPROPERTY(EditAnywhere, Category = "Pop Up Properties")
	float DelayUntilDisappear;

	FTimerHandle DisappearTimerHandle;

	UPROPERTY(EditAnywhere, Category = "VFX Properties")
	float DelayExplosion;

	UPROPERTY(EditAnywhere, Category = "VFX Properties")
	UNiagaraSystem* ExplosionVFXComp;
	
	FTimerHandle ExplosionTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Pop Up Properties")
	float DistanceUpward;
	
	UPROPERTY(EditAnywhere, Category = "Pop Up Properties")
	float InterpSpeed = 1.f;

	FVector EndLocation;
	FVector InitialLocation;

	void StartDestroyPopUp();
	void DestroyPopUp();
	
	UPROPERTY(EditAnywhere, Category = "SFX")
	TArray<USoundCue*> ExplosionSFX;

	void PlayExplosionSFX();
};
