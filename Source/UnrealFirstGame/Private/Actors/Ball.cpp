#include "Actors/Ball.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"

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
		}
		GetWorldTimerManager().SetTimer(DelayBeforeReset, this, &ABall::DestroyBall, DelayReset, false);
		return;
	}
	if(OtherActor->ActorHasTag("HOLE") && Comp->GetCollisionProfileName()!= "Ball_Hole")
	{
		Comp->SetCollisionProfileName("Ball_Hole",false);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
			Comp->GetCollisionProfileName().ToString());
	}
	if(OtherComp->ComponentHasTag("DESTROYER"))
	{
		DestroyBallEffect();
	}

}

void ABall::OnEndOverlap(class UPrimitiveComponent* Comp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if(OtherActor->ActorHasTag("HOLE") && Comp->GetCollisionProfileName()!= "PhysicsActor")
	{
		Comp->SetCollisionProfileName("PhysicsActor");
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
			Comp->GetCollisionProfileName().ToString());
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
	GetWorldTimerManager().SetTimer(DelayBeforeReset, this, &ABall::DestroyBall, DelayReset, false);
}

void ABall::HideBall()
{
	SphereComp->SetVisibility(false);
	BaseMesh->SetVisibility(false);
	SphereComp->SetSimulatePhysics(false);
	GetWorldTimerManager().SetTimer(DelayHideTrail, this, &ABall::HideTrail, DelayHide, false);
}

void ABall::HideTrail()
{
	TrailVFXComp->SetVisibility(false);
}


