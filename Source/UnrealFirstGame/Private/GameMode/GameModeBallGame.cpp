
#include "GameMode/GameModeBallGame.h"

#include "Blueprint/UserWidget.h"

void AGameModeBallGame::StartPlaying()
{
	IsPlaying = true;
	GameEnded = false;
	CurrentTime = TotalTime;
	GetWorldTimerManager().SetTimer(TotalTimeHandle, this, &AGameModeBallGame::StopPlaying, TotalTime,false);
}

void AGameModeBallGame::StopPlaying()
{
	GameEnded= true;
	IsPlaying = false;
}

