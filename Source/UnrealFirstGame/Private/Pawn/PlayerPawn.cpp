#include "Pawn/PlayerPawn.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Actors/Ball.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "GameMode/GameModeBallGame.h"

APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(BallSpawnPoint);

	BallSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Ball"));
	BallSpawnPoint->SetupAttachment(RootComponent);
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
	AGameModeBallGame* BallGameMode = Cast<AGameModeBallGame>(UGameplayStatics::GetGameMode(GetWorld()));
	BallGameMode->StartPlaying();
}

void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AGameModeBallGame* BallGameMode = Cast<AGameModeBallGame>(UGameplayStatics::GetGameMode(GetWorld()));
	if(BallGameMode->IsPlaying)
	{
		//Hitresult
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			FHitResult TempHitResult;
			PlayerController->GetHitResultUnderCursor(
				ECC_Visibility,
				false,
				TempHitResult);
			UPrimitiveComponent* Comp = TempHitResult.GetComponent();
			if (Comp != nullptr && !Comp->ComponentHasTag("TRANSPARENT"))
			{
				HitResult = TempHitResult;
				RotateShootDirection(HitResult.ImpactPoint);
			}
		}

		//ShootForce
		if(IsPressing)
		{
			IncrementPressedTimer();
		}

		//Timer
		BallGameMode->CurrentTime = BallGameMode->CurrentTime - UGameplayStatics::GetWorldDeltaSeconds(this);
		if(BallGameMode->CurrentTime <= 0.f)
		{
			BallGameMode->StopPlaying();
			IsPressing = false;
			IsIncreasing = false;
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
	AGameModeBallGame* BallGameMode = Cast<AGameModeBallGame>(UGameplayStatics::GetGameMode(GetWorld()));
	if(BallGameMode->IsPlaying)
	{
		IsPressing = true;
		IsIncreasing = true;
		FVector BallSpawnPointLocation = BallSpawnPoint->GetComponentLocation();
		FRotator BallSpawnPointRotation = BallSpawnPoint->GetComponentRotation();
		FTransform SpawnBallTransform(BallSpawnPointRotation, BallSpawnPointLocation,FVector(1,1,1));
		CurrentBall = GetWorld()->SpawnActor<ABall>(BallClass, SpawnBallTransform);
		CurrentBall->SphereComp->SetSimulatePhysics(false);
	}
	
}

void APlayerPawn::Fire()
{
	AGameModeBallGame* BallGameMode = Cast<AGameModeBallGame>(UGameplayStatics::GetGameMode(GetWorld()));
	if(BallGameMode->IsPlaying)
	{
		IsPressing = false;
		float PowerShoot = FMath::Lerp(0,MaxShootPower,PressedLerped);
		PressedTimer = 0;
		PressedLerped = 0;
		if (BallClass != nullptr)
		{
			CurrentBall->SphereComp->SetSimulatePhysics(true);
			CurrentBall->SphereComp->SetWorldRotation(BallSpawnPoint->GetComponentRotation());
			CurrentBall->SphereComp->AddImpulse(CurrentBall->GetActorForwardVector()*PowerShoot);
		}
	}
}

void APlayerPawn::CreateLine()
{
	for(float i = 0.0;i < 1; i = i + 0.2f)
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

void APlayerPawn::IncrementPressedTimer()
{
	if(PressedTimer < DelayToMaxShoot && IsIncreasing)
	{
		PressedTimer += UGameplayStatics::GetWorldDeltaSeconds(this);
		PressedLerped = FMath::Lerp(0,DelayToMaxShoot,PressedTimer);
		if(PressedTimer >=DelayToMaxShoot)
		{
			IsIncreasing = false;
		}
	}
	else if(PressedTimer > 0 && !IsIncreasing)
	{
		PressedTimer -= UGameplayStatics::GetWorldDeltaSeconds(this);
		PressedLerped = FMath::Lerp(0,DelayToMaxShoot,PressedTimer);
		if(PressedTimer <=0)
		{
			IsIncreasing = true;
		}
	}
}



