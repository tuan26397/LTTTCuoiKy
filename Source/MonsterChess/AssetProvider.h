// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "AssetProvider.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class MONSTERCHESS_API UAssetProvider : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		UTexture2D* LoadTexture2DReference(const FString & Path);
	
	
};
