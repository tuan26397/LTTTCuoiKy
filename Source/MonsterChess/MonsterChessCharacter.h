// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperFlipbook.h"
#include "Object.h"
#include "BattleInfo.h"
#include "Runtime/Engine/Classes/Components/ChildActorComponent.h"
#include "MonsterChessCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MONSTERCHESS_API AMonsterChessCharacter : public APaperCharacter
{
	GENERATED_UCLASS_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		int PlayerID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		bool Busy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		bool UsingSS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		bool UsingSkill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		bool ActionDisable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		bool AttackDisable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		bool MoveDisable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		FVector2D JumpIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		int Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		int Rating;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		float Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		int FullHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		int CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		float HealthEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		int FullArmor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		int CurrentArmor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		float ArmorEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		int FullEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		int CurrentEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		float SpeedEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		float PhysicalDamageEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		float BreakDamageEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		float MagicalDamageEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		FString Skill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		FString SpecialSkill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		FString ChessRole;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		FString Race;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		TArray<FVector2D> OwnedIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
		UBattleInfo* BattleInfo;

	//Function support-------------------------------------------------------
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Information)
		FVector IndexToLocation(const FVector2D & Index);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Information)
		FVector2D LocationToIndex(const FVector & Location);

	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = Information)
		float Distance(const FVector & A, const FVector & B);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Information)
		float CalculateRotation(const FVector2D & Velocity);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Information)
		FVector2D BasedOnShortestPath(const FVector2D & Index);

	//-----------------------------------------------------------------------

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent ,Category = Information)
		void UpdateAnimation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Information)
		void UseSkill();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Information)
		void UseSS();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Information)
		bool CanUseJump();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Information)
		bool CanUseSkill();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Information)
		bool CanUseSS();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Information)
		void ApplyDamage(const int & PhysicalDamage, const int & BreakDamage, const int & MagicalDamage);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Information)
		FVector2D FindIndex(const float & Radius, const FVector2D & Target, bool & Cannot);


	/*
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Information)
		AMonsterChessCharacter* FindSkillTarget();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Information)
		template<T> FindSSTarget();
		*/
};
