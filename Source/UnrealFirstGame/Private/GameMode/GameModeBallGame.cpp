
#include "GameMode/GameModeBallGame.h"

#include "Blueprint/UserWidget.h"

void AGameModeBallGame::StartPlaying()
{
	IsPlaying = true;
	GameEnded = false;
	GetWorldTimerManager().SetTimer(TotalTimeHandle, this, &AGameModeBallGame::StopPlaying, TotalTime,false);
}

void AGameModeBallGame::StopPlaying()
{
	GameEnded= true;
	IsPlaying = false;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue,
	TEXT("HERE"));
}

