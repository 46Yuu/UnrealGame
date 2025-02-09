#include "Actors/Hole.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

AHole::AHole()
{
	PrimaryActorTick.bCanEverTick = true;

	this->Tags.Add("HOLE");
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	RootComponent = SphereComp;
	
	HoleCollisionMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HoleCollision Mesh"));
	HoleCollisionMeshComp->SetupAttachment(SphereComp);
	HoleCollisionMeshComp->SetCollisionProfileName("HoleMesh");
	HoleCollisionMeshComp->SetVisibility(false);
	HoleCollisionMeshComp->SetHiddenInGame(true);

	CylinderComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cylinder Collider"));
	CylinderComp->SetupAttachment(SphereComp);
	CylinderComp->SetCollisionProfileName("NoCollision");

	HoleMask = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hole Mask"));
	HoleMask->SetupAttachment(SphereComp);
	HoleMask->SetCollisionProfileName("NoCollision");
	HoleMask->CastShadow = false;
	HoleMask->SetRenderInMainPass(false);
	HoleMask->SetRenderInDepthPass(false);
	HoleMask->SetRenderCustomDepth(true);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	BoxComp->SetupAttachment(RootComponent);
}

void AHole::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(MoveTimerHandle, this, &AHole::MoveTowardsNextPoint, MoveDelay, false);
}

void AHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(IsMoving)
	{
		if(CurrentDistance < TotalDistance)
		{
			FVector Location = GetActorLocation();
			Location += Destination * MoveSpeed * DeltaTime;
			SetActorLocation(Location);
			CurrentDistance = (Location - StartLocation).Size();
		}
		else
		{
			CurrentCheckPointIndex++;
			if(CurrentCheckPointIndex >= CheckPointLocationList.Num())
			{
				CurrentCheckPointIndex = 0;
			}
			IsMoving = false;
			GetWorldTimerManager().SetTimer(MoveTimerHandle, this, &AHole::MoveTowardsNextPoint, MoveDelay, false);
		}
	}
}

void AHole::MoveTowardsNextPoint()
{
	StartLocation = GetActorLocation();
	Destination = CheckPointLocationList[CurrentCheckPointIndex] - StartLocation;
	TotalDistance = Destination.Size();
	Destination = Destination.GetSafeNormal();
	CurrentDistance = 0.0f;
	IsMoving = true;
}

