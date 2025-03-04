#include "Actors/Ball.h"

#include <string>

#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Actors/Hole.h"
#include "Actors/TextPopUp.h"
#include "GameMode/GameModeBallGame.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ABall::ABall()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	RootComponent = SphereComp;
	
	SphereComp->OnComponentBeginOverlap.AddDynamic(this,&ABall::OnBeginOverlap);
	SphereComp->OnComponentEndOverlap.AddDynamic(this,&ABall::OnEndOverlap);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base mesh"));
	BaseMesh->SetupAttachment(SphereComp);

	SphereComp->SetSimulatePhysics(true);
	SphereComp->SetCollisionProfileName("PhysicsActor");

	TrailVFXComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Trail VFX"));
	TrailVFXComp->SetupAttachment(SphereComp);

	BaseMesh->SetCollisionProfileName("NoCollision");
}

void ABall::BeginPlay()
{
	Super::BeginPlay();
	BaseLocation = GetActorLocation();
}

/*void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}*/

void ABall::OnBeginOverlap(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherComp->ComponentHasTag("HOLE_BOTTOM"))
	{
		BallInHole(OtherActor,OtherComp);
		return;
	}
	if(OtherActor->ActorHasTag("HOLE") && Comp->GetCollisionProfileName()!= "Ball_Hole")
	{
		Comp->SetCollisionProfileName("Ball_Hole",false);
	}
	if(OtherComp->ComponentHasTag("DESTROYER"))
	{
		DestroyBallEffect();
		ResetCombo();
	}

}

void ABall::OnEndOverlap(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if(OtherActor->ActorHasTag("HOLE") && Comp->GetCollisionProfileName()!= "PhysicsActor")
	{
		Comp->SetCollisionProfileName("PhysicsActor");
	}
}

void ABall::ResetPosition()
{
	SphereComp->SetSimulatePhysics(false);
	SphereComp->SetPhysicsLinearVelocity(FVector(0, 0, 0));
	SphereComp->SetPhysicsAngularVelocityInDegrees(FVector(0, 0, 0));
	SphereComp->SetWorldLocation(BaseLocation);
	SphereComp->SetCollisionProfileName("PhysicsActor");
	SphereComp->SetSimulatePhysics(true);
}

void ABall::DestroyBall()
{
	this->Destroy();
}

void ABall::DestroyBallEffect()
{
	HideBall();
	if(DestroyBallExplosionVFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			DestroyBallExplosionVFX,
			this->GetActorLocation(),
			this->GetActorRotation());
	}
	PlayExplosionSFX();
	GetWorldTimerManager().SetTimer(DelayBeforeReset, this, &ABall::DestroyBall, DelayReset, false);
}

void ABall::HideBall()
{
	SphereComp->SetVisibility(false);
	BaseMesh->SetVisibility(false);
	SphereComp->SetSimulatePhysics(false);
	SphereComp->SetGenerateOverlapEvents(false);
	GetWorldTimerManager().SetTimer(DelayHideTrail, this, &ABall::HideTrail, DelayHide, false);
}

void ABall::HideTrail()
{
	TrailVFXComp->SetVisibility(false);
}

void ABall::BallInHole(AActor* OtherActor, UPrimitiveComponent* OtherComp)
{
	if(ExplosionVFX)
	{
		HideBall();
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			ExplosionVFX,
			OtherComp,
			NAME_None,
			OtherActor->GetActorLocation(),
			OtherActor->GetActorRotation(),
			EAttachLocation::Type::KeepWorldPosition,
			true);
		AGameModeBallGame* BallGameMode = Cast<AGameModeBallGame>(UGameplayStatics::GetGameMode(GetWorld()));
		if(BallGameMode->IsPlaying)
		{
			PlayCoinSFX();
			IncrementScore(OtherActor);
			IncrementHoleMultiplier(OtherActor);
		}
	}
	GetWorldTimerManager().SetTimer(DelayBeforeReset, this, &ABall::DestroyBall, DelayReset, false);
}

void ABall::IncrementScore(AActor* OtherActor)
{
	IncrementCombo();
	AHole* HoleActor = Cast<AHole>(OtherActor);
	AGameModeBallGame* BallGameMode = Cast<AGameModeBallGame>(UGameplayStatics::GetGameMode(GetWorld()));
	
	int ScoreToAdd = BasePoint * HoleActor->CurrentMultiplier * BallGameMode->CurrentCombo;
	BallGameMode->CurrentScore = BallGameMode->CurrentScore + ScoreToAdd;
	
	FString FloatScore = FString::SanitizeFloat(BallGameMode->CurrentScore);
	FString PopUpString = "+"+FString::FromInt(ScoreToAdd);
	SpawnPopUp(FText::FromString(PopUpString));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
	*FloatScore);

}

void ABall::IncrementCombo()
{
	AGameModeBallGame* BallGameMode = Cast<AGameModeBallGame>(UGameplayStatics::GetGameMode(GetWorld()));
	BallGameMode->CurrentCombo++;
	FString FloatCombo = FString::SanitizeFloat(BallGameMode->CurrentCombo);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue,
	*FloatCombo);
}

void ABall::IncrementHoleMultiplier(AActor* OtherActor)
{
	AHole* HoleActor = Cast<AHole>(OtherActor);
	HoleActor->CurrentMultiplier++;
	FString Multiplier = FString::SanitizeFloat(HoleActor->CurrentMultiplier);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue,
	*Multiplier);
	HoleActor->UpdateTextRenderer();
	HoleActor->StartIsReseting();
}

void ABall::ResetCombo()
{
	AGameModeBallGame* BallGameMode = Cast<AGameModeBallGame>(UGameplayStatics::GetGameMode(GetWorld()));
	BallGameMode->CurrentCombo = 0;
	FString FloatCombo = FString::SanitizeFloat(BallGameMode->CurrentCombo);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue,
	*FloatCombo);
}

void ABall::SpawnPopUp(FText Score)
{
	FVector Location = SphereComp->GetComponentLocation();
	Location.Z += 5.f;
	FRotator Rotation = FRotator(0,0,0);
	FTransform Transform(Rotation, Location,FVector(1,1,1));
	ATextPopUp* ActorTextPopUp = GetWorld()->SpawnActor<ATextPopUp>(PopUpClass,Transform);
	ActorTextPopUp->SetPopUpText(Score);
}

void ABall::PlayExplosionSFX()
{
	AGameModeBallGame* BallGameMode = Cast<AGameModeBallGame>(UGameplayStatics::GetGameMode(GetWorld()));
	if(BallGameMode->IsPlaying)
	{
		int index = FMath::RandRange(0,ExplosionSFXList.Num()-1);
		if (ExplosionSFXList[index])
		{
			UGameplayStatics::PlaySound2D(
				this,
				ExplosionSFXList[index]);
		}
	}
	
}

void ABall::PlayCoinSFX()
{
	int index = FMath::RandRange(0,CoinSFXList.Num()-1);
	if (CoinSFXList[index])
	{
		UGameplayStatics::PlaySound2D(
			this,
			CoinSFXList[index]);
	}
}


