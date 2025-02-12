#include "Pawn/PlayerPawn.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Actors/Ball.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);

	BallSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Ball"));
	BallSpawnPoint->SetupAttachment(Camera);
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	BallPlayerController = Cast<APlayerController>(GetController());
	if (BallPlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(BallPlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerContext, 0);
		}
	}
	GetWorldTimerManager().SetTimer(DelayBeforeNewPoints, this, &APlayerPawn::CreateLine,DelayNewPoints,true);
}

void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		PlayerController->GetHitResultUnderCursor(
			ECC_Visibility,
			false,
			HitResult);
		RotateShootDirection(HitResult.ImpactPoint);
	}
	if(IsPressing)
	{
		if(PressedTimer < DelayToMaxShoot)
		{
			PressedTimer += UGameplayStatics::GetWorldDeltaSeconds(this);
		}
	}
}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (FireAction)
		{
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ThisClass::StartFire);
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ThisClass::Fire);
		}
	}	
}

void APlayerPawn::RotateShootDirection(const FVector& LookAtTarget)
{
	FVector ToTarget = LookAtTarget - BallSpawnPoint->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
	BallSpawnPoint->SetWorldRotation(LookAtRotation);
}

void APlayerPawn::StartFire()
{
	IsPressing = true;
}

void APlayerPawn::Fire()
{
	IsPressing = false;
	float PressedLerped = FMath::Lerp(0,DelayToMaxShoot,PressedTimer);
	float PowerShoot = FMath::Lerp(0,MaxShootPower,PressedLerped);
	PressedTimer = 0;
	FVector BallSpawnPointLocation = BallSpawnPoint->GetComponentLocation();
	FRotator BallSpawnPointRotation = BallSpawnPoint->GetComponentRotation();
	FTransform SpawnBallTransform(BallSpawnPointRotation, BallSpawnPointLocation,FVector(1,1,1));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
			SpawnBallTransform.ToString());
	if (BallClass != nullptr)
	{
		auto Ball = GetWorld()->SpawnActor<ABall>(BallClass, SpawnBallTransform);
		Ball->SphereComp->AddImpulse(Ball->GetActorForwardVector()*PowerShoot);
	}
}

void APlayerPawn::CreateLine()
{
	for(float i = 0.0;i < 0.6; i = i + 0.1f)
	{
		if(AimLineVFX)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				this,
				AimLineVFX,
				FMath::Lerp(BallSpawnPoint->GetComponentLocation(),HitResult.ImpactPoint,i),
				BallSpawnPoint->GetComponentRotation());
		}
	}
	
}



