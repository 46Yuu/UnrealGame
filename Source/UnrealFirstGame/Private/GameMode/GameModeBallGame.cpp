
#include "GameMode/GameModeBallGame.h"

#include "Blueprint/UserWidget.h"

void AGameModeBallGame::StartPlaying()
{
	IsPlaying = true;
	GameEnded = false;
	CurrentTime = TotalTime;
}

void AGameModeBallGame::StopPlaying()
{
	GameEnded= true;
	IsPlaying = false;
}

