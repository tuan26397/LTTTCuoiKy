// Fill out your copyright notice in the Description page of Project Settings.

#include "AssetProvider.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"

UTexture2D* UAssetProvider::LoadTexture2DReference_Implementation(const FString & Path)
{
	FStringAssetReference assetRef(Path);
	return Cast<UTexture2D>(assetRef.TryLoad());
	//return Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *(Path)));
	//return LoadObject<UTexture2D>(NULL, *Path, NULL, LOAD_None, NULL);
}


