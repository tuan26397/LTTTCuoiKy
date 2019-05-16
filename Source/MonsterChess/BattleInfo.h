// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BattleInfo.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class MONSTERCHESS_API UBattleInfo : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		TArray<FVector2D> OwnedIndex;
	
	
};
