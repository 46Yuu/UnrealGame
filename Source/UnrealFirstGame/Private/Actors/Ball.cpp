#include "Actors/Ball.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"

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
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
			TEXT("HELLO"));
		if(ExplosionVFX)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(
				ExplosionVFX,
				OtherComp,
				NAME_None,
				OtherActor->GetActorLocation(),
				OtherActor->GetActorRotation(),
				EAttachLocation::Type::KeepWorldPosition,
				true);
		}
		GetWorldTimerManager().SetTimer(DelayBeforeReset, this, &ABall::ResetPosition, DelayReset, false);
		return;
	}
	if(OtherActor->ActorHasTag("HOLE") && Comp->GetCollisionProfileName()!= "Ball_Hole")
	{
		Comp->SetCollisionProfileName("Ball_Hole",false);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
			Comp->GetCollisionProfileName().ToString());
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


