#include "Actors/Hole.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"

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

	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text Renderer"));
	TextRenderComponent->SetupAttachment(SphereComp);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	BoxComp->SetupAttachment(RootComponent);
}
	

void AHole::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(MoveTimerHandle, this, &AHole::MoveTowardsNextPoint, MoveDelay, false);
	CreateRandomCheckpoints();
	if(CheckPointLocationList.Num()>0)
	{
		this->SetActorLocation(CheckPointLocationList[0]);
	}
	UpdateTextRenderer();
}

void AHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(IsMoving)
	{
		if(CurrentDistance < TotalDistance)
		{
			FVector Location = GetActorLocation();
			Location += Destination * MoveSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);
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
	if(CheckPointLocationList.Num() > 0)
	{
		StartLocation = GetActorLocation();
		Destination = CheckPointLocationList[CurrentCheckPointIndex] - StartLocation;
		TotalDistance = Destination.Size();
		Destination = Destination.GetSafeNormal();
		CurrentDistance = 0.0f;
		IsMoving = true;
	}
}

void AHole::CreateRandomCheckpoints()
{
	for(int i =0; i< CheckPointsCount;i++)
	{
		FVector CheckpointPos;
		FVector VerticalPos = FMath::Lerp(BottomLeftPoint,TopLeftPoint,FMath::FRandRange(0.f,1.f));
		FVector HorizontalPos = FMath::Lerp(BottomLeftPoint,BottomRightPoint,FMath::FRandRange(0.f,1.f));
		CheckpointPos = VerticalPos;
		CheckpointPos.Y = HorizontalPos.Y;
		CheckPointLocationList.Add(CheckpointPos);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
			CheckpointPos.ToString());
	}
}

void AHole::UpdateTextRenderer()
{
	FString Multiplier = FString::FromInt(BaseMultiplier);
	TextRenderComponent->SetText(FText::FromString("x"+Multiplier));
}
