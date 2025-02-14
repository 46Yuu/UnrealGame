#include "Actors/Hole.h"

#include "Actors/TextPopUp.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameMode/GameModeBallGame.h"
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

	TextRenderCooldownComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text Cooldown Renderer"));
	TextRenderCooldownComponent->SetupAttachment(SphereComp);
	
}
	

void AHole::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(MoveTimerHandle, this, &AHole::MoveTowardsNextPoint, MoveDelay, false);
	if(!IsCheckpointRandom)
	{
		if(CheckPointsCount>0)
		{
			CreateRandomCheckpoints();
			if(CheckPointLocationList.Num()>0)
			{
				this->SetActorLocation(CheckPointLocationList[0]);
			}
		}
	}
	CurrentMultiplier = BaseMultiplier;
	UpdateTextRenderer();
	TextRenderCooldownComponent->SetVisibility(false);
	GameModeBallGame = Cast<AGameModeBallGame>(UGameplayStatics::GetGameMode(GetWorld()));
}

void AHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsMoving && GameModeBallGame->IsPlaying)
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

void AHole::UpdateTextCooldownRender()
{
	FString Timer = FString::Printf(TEXT("%.0f"), CurrentResetMultiplierTimer);
	TextRenderCooldownComponent->SetText(FText::FromString(Timer));
}

void AHole::StartIsReseting()
{
	IsResetting = true;
	CurrentResetMultiplierTimer = DelayResetMultiplier;
	TextRenderCooldownComponent->SetVisibility(true);

	GetWorldTimerManager().SetTimer(DelayResetTimerHandle, this, &AHole::CountDownResetCooldown,1.f,false);
	UpdateTextCooldownRender();
}

void AHole::CountDownResetCooldown()
{
	CurrentResetMultiplierTimer --;
	UpdateTextCooldownRender();
	if(CurrentResetMultiplierTimer > 0)
	{
		GetWorldTimerManager().SetTimer(DelayResetTimerHandle, this, &AHole::CountDownResetCooldown,1.f,false);
		return;
	}
	ResetMultiplier();
}


void AHole::ResetMultiplier()
{
	IsResetting = false;
	CurrentResetMultiplierTimer = DelayResetMultiplier;
	CurrentMultiplier = BaseMultiplier;
	UpdateTextRenderer();
	TextRenderCooldownComponent->SetVisibility(false);
}