#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeBallGame.generated.h"

UCLASS()
class UNREALFIRSTGAME_API AGameModeBallGame : public AGameModeBase
{
	GENERATED_BODY()
public :
	UPROPERTY(BlueprintReadWrite, Category = "Score Properties")
	float CurrentScore;
	UPROPERTY(BlueprintReadWrite, Category = "Score Properties")
	int CurrentCombo;
	
	int CountBallInHole;

	UPROPERTY(EditAnywhere, Category = "Game Properties")
	float TotalTime = 120.f;

	UPROPERTY(BlueprintReadWrite, Category = "Time Properties")
	float CurrentTime;
	
	void StartPlaying();
	void StopPlaying();
	
	UPROPERTY(BlueprintReadWrite, Category = "Score Properties")
	bool GameEnded = false;
	
	bool IsPlaying = false;
	
};
