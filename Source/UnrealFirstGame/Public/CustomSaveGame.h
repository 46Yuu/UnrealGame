#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CustomSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FPlayerStat
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Username;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Score;
};

UCLASS()
class UNREALFIRSTGAME_API UCustomSaveGame : public USaveGame
{
	GENERATED_BODY()
public :

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	float Score;
};
