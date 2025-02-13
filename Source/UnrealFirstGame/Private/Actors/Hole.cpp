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
	if(!IsCheckpointRandom)
	{
		CreateRandomCheckpoints();
		if(CheckPointLocationList.Num()>0)
		{
			this->SetActorLocation(CheckPointLocationList[0]);
		}
	}
	CurrentMultiplier = BaseMultiplier;
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
	if(IsResetting)
	{
		CurrentResetMultiplierTimer += UGameplayStatics::GetWorldDeltaSeconds(this);
		if(CurrentResetMultiplierTimer > DelayResetMultiplier)
		{
			IsResetting = false;
			CurrentResetMultiplierTimer = 0;
			CurrentMultiplier = BaseMultiplier;
			UpdateTextRenderer();
		}
	}
}

void AHole::MoveTowardsNextPoint()
{
	StartLocation = GetActorLocation();
	if(!IsCheckpointRandom && CheckPointLocationList.Num() > 0)
	{
		Destination = CheckPointLocationList[CurrentCheckPointIndex] - StartLocation;
	}
	else
	{
		Destination = CreateNextRandomCheckpoint() - StartLocation;
	}
	TotalDistance = Destination.Size();
	Destination = Destination.GetSafeNormal();
	CurrentDistance = 0.0f;
	IsMoving = true;
}

void AHole::CreateRandomCheckpoints()
{
	for(int i =0; i< CheckPointsCount;i++)
	{
		FVector VerticalPos = FMath::Lerp(BottomLeftPoint,TopLeftPoint,FMath::FRandRange(0.f,1.f));
		FVector HorizontalPos = FMath::Lerp(BottomLeftPoint,BottomRightPoint,FMath::FRandRange(0.f,1.f));
		FVector CheckpointPos = VerticalPos;
		CheckpointPos.Y = HorizontalPos.Y;
		CheckPointLocationList.Add(CheckpointPos);
	}
}

FVector AHole::CreateNextRandomCheckpoint()
{
	FVector VerticalPos = FMath::Lerp(BottomLeftPoint,TopLeftPoint,FMath::FRandRange(0.f,1.f));
	FVector HorizontalPos = FMath::Lerp(BottomLeftPoint,BottomRightPoint,FMath::FRandRange(0.f,1.f));
	FVector CheckpointPos = VerticalPos;
	CheckpointPos.Y = HorizontalPos.Y;
	return CheckpointPos;
}

void AHole::UpdateTextRenderer()
{
	FString Multiplier = FString::FromInt(CurrentMultiplier);
	TextRenderComponent->SetText(FText::FromString("x"+Multiplier));
	int Index = FMath::Min(FMath::FloorToInt(CurrentMultiplier / 2.0f), 4); 
	TextRenderComponent->SetTextRenderColor(ColorMultipliers[Index]);
}

void AHole::StartIsReseting()
{
	IsResetting = true;
	CurrentResetMultiplierTimer = 0;
}

