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
};
