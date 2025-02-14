#include "Actors/TextPopUp.h"
#include "Sound/SoundCue.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"

ATextPopUp::ATextPopUp()
{
	PrimaryActorTick.bCanEverTick = true;
	
	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text Renderer"));
	TextRenderComponent->SetupAttachment(RootComponent);
	
}

void ATextPopUp::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(DisappearTimerHandle, this, &ATextPopUp::StartDestroyPopUp, DelayUntilDisappear, false);
	InitialLocation = this->GetActorLocation();
	EndLocation = InitialLocation;
	EndLocation.Z += DistanceUpward;
}

void ATextPopUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorLocation(FMath::VInterpTo(GetActorLocation(),EndLocation,
	UGameplayStatics::GetWorldDeltaSeconds(this),InterpSpeed));
}

void ATextPopUp::SetPopUpText(FText Text)
{
	TextRenderComponent->SetText(Text);
}

void ATextPopUp::SetPopUpColor(FColor Color)
{
	TextRenderComponent->SetTextRenderColor(Color);
}

void ATextPopUp::StartDestroyPopUp()
{
	TextRenderComponent->SetVisibility(false);
	if(ExplosionVFXComp)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			ExplosionVFXComp,
			this->GetActorLocation(),
			this->GetActorRotation());
	}
	PlayExplosionSFX();
	GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &ATextPopUp::DestroyPopUp, DelayExplosion, false);
}


void ATextPopUp::DestroyPopUp()
{
	this->Destroy();
}

void ATextPopUp::PlayExplosionSFX()
{
	int index = FMath::RandRange(0,ExplosionSFX.Num()-1);
	if (ExplosionSFX[index])
	{
		UGameplayStatics::PlaySound2D(
			this,
			ExplosionSFX[index]);
	}
}
