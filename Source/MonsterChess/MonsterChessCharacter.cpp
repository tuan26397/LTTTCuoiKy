// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterChessCharacter.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Runtime/Core/Public/Containers/Queue.h"
#include "BattleInfo.h"

UPaperFlipbook* LoadFlipbookReference(const FString & Path)
{
	FStringAssetReference assetRef(Path);
	return Cast<UPaperFlipbook>(assetRef.TryLoad());
}

FVector AMonsterChessCharacter::IndexToLocation_Implementation(const FVector2D & Index)
{
	return FVector((Index.X + (Index.X < 0 ? 0.5 : -0.5)) * 120, (Index.Y + (Index.Y < 0 ? 0.5 : -0.5)) * 120 + PlayerID * 2000, 1);
}

FVector2D AMonsterChessCharacter::LocationToIndex_Implementation(const FVector & Location)
{
	float X = Location.X, Y = Location.Y - PlayerID * 2000;
	return FVector2D(X < 0 ? FMath::FloorToFloat(X / 120) : FMath::CeilToFloat(X / 120), Y < 0 ? FMath::FloorToFloat(Y / 120) : FMath::CeilToFloat(Y / 120));
}

float AMonsterChessCharacter::Distance_Implementation(const FVector & A, const FVector & B)
{
	return FMath::Sqrt(FMath::Square(A.X - B.X) + FMath::Square(A.Y - B.Y));
}

float AMonsterChessCharacter::CalculateRotation_Implementation(const FVector2D & Velocity)
{
	float result = FMath::RadiansToDegrees(FMath::Atan2(Velocity.X, -Velocity.Y));
	return result + (result < 0 ? 360 : 0);
}

bool Found(FVector2D Current, FVector2D Root, FVector2D Finish, TMap<FVector2D, FVector2D> &Previous, TArray<FVector2D> &Visited, TQueue<FVector2D> &q)
{
	q.Enqueue(Current);
	Previous.Add(Current, Root);
	Visited.Add(Current);
	if (Current == Finish)return true;
	return false;
}

FVector2D AMonsterChessCharacter::BasedOnShortestPath_Implementation(const FVector2D & Index)
{
	TQueue<FVector2D> q;
	FVector2D Result, ActorIndex = LocationToIndex(GetActorLocation());
	q.Enqueue(ActorIndex);
	TArray<FVector2D> Visited = BattleInfo->OwnedIndex;
	TMap<FVector2D, FVector2D> Previous;
	int BaseX[4] = { -1, 0, 0, 1 }, BaseY[4] = { 0,-1,1,0 };

	if (ChessRole == "Tanker")
	{
		while (!q.IsEmpty())
		{
			FVector2D TempIndex;
			q.Dequeue(TempIndex);

			for (int x = TempIndex.X + 1 + (TempIndex.X + 1 == 0 ? 1 : 0); x <= 4 && !Visited.Contains(FVector2D(x, TempIndex.Y)); x = x + 1 + (x + 1 == 0 ? 1 : 0))
			{
				if (Found(FVector2D(x, TempIndex.Y), TempIndex, Index, Previous, Visited, q))
				{
					Result = FVector2D(x, TempIndex.Y);
					goto Finish;
				}
			}
			for (int x = TempIndex.X - 1 - (TempIndex.X - 1 == 0 ? 1 : 0); x >= -4 && !Visited.Contains(FVector2D(x, TempIndex.Y)); x = x - 1 - (x - 1 == 0 ? 1 : 0))
			{
				if (Found(FVector2D(x, TempIndex.Y), TempIndex, Index, Previous, Visited, q))
				{
					Result = FVector2D(x, TempIndex.Y);
					goto Finish;
				}
			}
			for (int y = TempIndex.Y + 1 + (TempIndex.Y + 1 == 0 ? 1 : 0); y <= 4 && !Visited.Contains(FVector2D(TempIndex.X, y)); y = y + 1 + (y + 1 == 0 ? 1 : 0))
			{
				if (Found(FVector2D(TempIndex.X, y), TempIndex, Index, Previous, Visited, q))
				{
					Result = FVector2D(TempIndex.X, y);
					goto Finish;
				}
			}
			for (int y = TempIndex.Y - 1 - (TempIndex.Y - 1 == 0 ? 1 : 0); y >= -4 && !Visited.Contains(FVector2D(TempIndex.X, y)); y = y - 1 - (y - 1 == 0 ? 1 : 0))
			{
				if (Found(FVector2D(TempIndex.X, y), TempIndex, Index, Previous, Visited, q))
				{
					Result = FVector2D(TempIndex.X, y);
					goto Finish;
				}
			}
		}

	}
	else if (ChessRole == "Warrior")
	{
		while (!q.IsEmpty())
		{
			FVector2D TempIndex;
			q.Dequeue(TempIndex);
			int id, x, y, X, Y;
			for (x = TempIndex.X + 1 + (TempIndex.X + 1 == 0 ? 1 : 0), y = TempIndex.Y + 1 + (TempIndex.Y + 1 == 0 ? 1 : 0), id = 0
				; id < 2 && x <= 4 && y <= 4 && !Visited.Contains(FVector2D(x, y))
				; x = x + 1 + (x + 1 == 0 ? 1 : 0), y = y + 1 + (y + 1 == 0 ? 1 : 0), id++)
			{
				X = x, Y = y;
			}
			if (id == 2)
			{
				if (Found(FVector2D(X, Y), TempIndex, Index, Previous, Visited, q))
				{
					Result = FVector2D(X, Y);
					goto Finish;
				}
			}
			for (x = TempIndex.X - 1 - (TempIndex.X - 1 == 0 ? 1 : 0), y = TempIndex.Y + 1 + (TempIndex.Y + 1 == 0 ? 1 : 0), id = 0
				; id < 2 && x >= -4 && y <= 4 && !Visited.Contains(FVector2D(x, y))
				; x = x - 1 - (x - 1 == 0 ? 1 : 0), y = y + 1 + (y + 1 == 0 ? 1 : 0), id++)
			{
				X = x, Y = y;
			}
			if (id == 2)
			{
				if (Found(FVector2D(X, Y), TempIndex, Index, Previous, Visited, q))
				{
					Result = FVector2D(X, Y);
					goto Finish;
				}
			}
			for (x = TempIndex.X + 1 + (TempIndex.X + 1 == 0 ? 1 : 0), y = TempIndex.Y - 1 - (TempIndex.Y - 1 == 0 ? 1 : 0), id = 0
				; id < 2 && x <= 4 && y >= -4 && !Visited.Contains(FVector2D(x, y))
				; x = x + 1 + (x + 1 == 0 ? 1 : 0), y = y - 1 - (y - 1 == 0 ? 1 : 0), id++)
			{
				X = x, Y = y;
			}
			if (id == 2)
			{
				if (Found(FVector2D(X, Y), TempIndex, Index, Previous, Visited, q))
				{
					Result = FVector2D(X, Y);
					goto Finish;
				}
			}
			for (x = TempIndex.X - 1 - (TempIndex.X - 1 == 0 ? 1 : 0), y = TempIndex.Y - 1 - (TempIndex.Y - 1 == 0 ? 1 : 0), id = 0
				; id < 2 && x >= -4 && y >= -4 && !Visited.Contains(FVector2D(x, y))
				; x = x - 1 - (x - 1 == 0 ? 1 : 0), y = y - 1 - (y - 1 == 0 ? 1 : 0), id++)
			{
				X = x, Y = y;
			}
			if (id == 2)
			{
				if (Found(FVector2D(X, Y), TempIndex, Index, Previous, Visited, q))
				{
					Result = FVector2D(X, Y);
					goto Finish;
				}
			}

			for (int i = 0; i < 4; i++)
			{
				x = TempIndex.X + BaseX[i] + (TempIndex.X + BaseX[i] == 0 ? (BaseX[i] < 0 ? -1 : 1) : 0)
					, y = TempIndex.Y + BaseY[i] + (TempIndex.Y + BaseY[i] == 0 ? (BaseY[i] < 0 ? -1 : 1) : 0);
				if (x >= -4 && x <= 4 && y >= -4 && y <= 4 && !Visited.Contains(FVector2D(x, y)))
				{
					if (Found(FVector2D(x, y), TempIndex, Index, Previous, Visited, q))
					{
						Result = FVector2D(x, y);
						goto Finish;
					}
				}
			}
			
		}

	}
	else if (ChessRole == "Hunter")
	{
		while (!q.IsEmpty())
		{
			FVector2D TempIndex;
			q.Dequeue(TempIndex);

			int id, x, y, X, Y;
			for (x = TempIndex.X + 1 + (TempIndex.X + 1 == 0 ? 1 : 0), y = TempIndex.Y + 1 + (TempIndex.Y + 1 == 0 ? 1 : 0), id = 0
				; id < 2 && x <= 4 && y <= 4
				; x = x + 1 + (x + 1 == 0 ? 1 : 0), id++)
			{
				X = x, Y = y;
			}
			if (!Visited.Contains(FVector2D(X, Y)) && id == 2)
			{
				if (Found(FVector2D(X, Y), TempIndex, Index, Previous, Visited, q))
				{
					Result = FVector2D(X, Y);
					goto Finish;
				}
			}
			for (x = TempIndex.X - 1 - (TempIndex.X - 1 == 0 ? 1 : 0), y = TempIndex.Y + 1 + (TempIndex.Y + 1 == 0 ? 1 : 0), id = 0
				; id < 2 && x >= -4 && y <= 4
				; x = x - 1 - (x - 1 == 0 ? 1 : 0), id++)
			{
				X = x, Y = y;
			}
			if (!Visited.Contains(FVector2D(X, Y)) && id == 2)
			{
				if (Found(FVector2D(X, Y), TempIndex, Index, Previous, Visited, q))
				{
					Result = FVector2D(X, Y);
					goto Finish;
				}
			}
			for (x = TempIndex.X + 1 + (TempIndex.X + 1 == 0 ? 1 : 0), y = TempIndex.Y - 1 - (TempIndex.Y - 1 == 0 ? 1 : 0), id = 0
				; id < 2 && x <= 4 && y >= -4
				; x = x + 1 + (x + 1 == 0 ? 1 : 0), id++)
			{
				X = x, Y = y;
			}
			if (!Visited.Contains(FVector2D(X, Y)) && id == 2)
			{
				if (Found(FVector2D(X, Y), TempIndex, Index, Previous, Visited, q))
				{
					Result = FVector2D(X, Y);
					goto Finish;
				}
			}
			for (x = TempIndex.X - 1 - (TempIndex.X - 1 == 0 ? 1 : 0), y = TempIndex.Y - 1 - (TempIndex.Y - 1 == 0 ? 1 : 0), id = 0
				; id < 2 && x >= -4 && y >= -4
				; x = x - 1 - (x - 1 == 0 ? 1 : 0), id++)
			{
				X = x, Y = y;
			}
			if (!Visited.Contains(FVector2D(X, Y)) && id == 2)
			{
				if (Found(FVector2D(X, Y), TempIndex, Index, Previous, Visited, q))
				{
					Result = FVector2D(X, Y);
					goto Finish;
				}
			}
			for (x = TempIndex.X + 1 + (TempIndex.X + 1 == 0 ? 1 : 0), y = TempIndex.Y + 1 + (TempIndex.Y + 1 == 0 ? 1 : 0), id = 0
				; id < 2 && x <= 4 && y <= 4
				; y = y + 1 + (y + 1 == 0 ? 1 : 0), id++)
			{
				X = x, Y = y;
			}
			if (!Visited.Contains(FVector2D(X, Y)) && id == 2)
			{
				if (Found(FVector2D(X, Y), TempIndex, Index, Previous, Visited, q))
				{
					Result = FVector2D(X, Y);
					goto Finish;
				}
			}
			for (x = TempIndex.X + 1 + (TempIndex.X + 1 == 0 ? 1 : 0), y = TempIndex.Y - 1 - (TempIndex.Y - 1 == 0 ? 1 : 0), id = 0
				; id < 2 && x <= 4 && y >= -4
				; y = y - 1 - (y - 1 == 0 ? 1 : 0), id++)
			{
				X = x, Y = y;
			}
			if (!Visited.Contains(FVector2D(X, Y)) && id == 2)
			{
				if (Found(FVector2D(X, Y), TempIndex, Index, Previous, Visited, q))
				{
					Result = FVector2D(X, Y);
					goto Finish;
				}
			}
			for (x = TempIndex.X - 1 - (TempIndex.X - 1 == 0 ? 1 : 0), y = TempIndex.Y + 1 + (TempIndex.Y + 1 == 0 ? 1 : 0), id = 0
				; id < 2 && x >= -4 && y <= 4
				; y = y + 1 + (y + 1 == 0 ? 1 : 0), id++)
			{
				X = x, Y = y;
			}
			if (!Visited.Contains(FVector2D(X, Y)) && id == 2)
			{
				if (Found(FVector2D(X, Y), TempIndex, Index, Previous, Visited, q))
				{
					Result = FVector2D(X, Y);
					goto Finish;
				}
			}
			for (x = TempIndex.X - 1 - (TempIndex.X - 1 == 0 ? 1 : 0), y = TempIndex.Y - 1 - (TempIndex.Y - 1 == 0 ? 1 : 0), id = 0
				; id < 2 && x >= -4 && y >= -4
				; y = y - 1 - (y - 1 == 0 ? 1 : 0), id++)
			{
				X = x, Y = y;
			}
			if (!Visited.Contains(FVector2D(X, Y)) && id == 2)
			{
				if (Found(FVector2D(X, Y), TempIndex, Index, Previous, Visited, q))
				{
					Result = FVector2D(X, Y);
					goto Finish;
				}
			}

			for (int i = 0; i < 4; i++)
			{
				x = TempIndex.X + BaseX[i] + (TempIndex.X + BaseX[i] == 0 ? (BaseX[i] < 0 ? -1 : 1) : 0)
					, y = TempIndex.Y + BaseY[i] + (TempIndex.Y + BaseY[i] == 0 ? (BaseY[i] < 0 ? -1 : 1) : 0);
				if (x >= -4 && x <= 4 && y >= -4 && y <= 4 && !Visited.Contains(FVector2D(x, y)))
				{
					if (Found(FVector2D(x, y), TempIndex, Index, Previous, Visited, q))
					{
						Result = FVector2D(x, y);
						goto Finish;
					}
				}
			}
		}

	}
	else if (ChessRole == "Warlock")
	{
		while (!q.IsEmpty())
		{
			FVector2D TempIndex;
			q.Dequeue(TempIndex);

			for (int i = 0; i < 4; i++)
			{
				int x = TempIndex.X + BaseX[i] + (TempIndex.X + BaseX[i] == 0 ? (BaseX[i] < 0 ? -1 : 1) : 0)
					, y = TempIndex.Y + BaseY[i] + (TempIndex.Y + BaseY[i] == 0 ? (BaseY[i] < 0 ? -1 : 1) : 0);
				if (x >= -4 && x <= 4 && y >= -4 && y <= 4 && !Visited.Contains(FVector2D(x, y)))
				{
					if (Found(FVector2D(x, y), TempIndex, Index, Previous, Visited, q))
					{
						Result = FVector2D(x, y);
						goto Finish;
					}
				}
			}
		}

	}
	else if (ChessRole == "Mage")
	{
		while (!q.IsEmpty())
		{
			FVector2D TempIndex;
			q.Dequeue(TempIndex);

			int x, y;
			x = TempIndex.X + 1 + (TempIndex.X + 1 == 0 ? 1 : 0), y = TempIndex.Y + 1 + (TempIndex.Y + 1 == 0 ? 1 : 0);
			if (x <= 4 && y <= 4 && !Visited.Contains(FVector2D(x, y)))
			{
				if (Found(FVector2D(x, y), TempIndex, Index, Previous, Visited, q))
				{
					Result = FVector2D(x, y);
					goto Finish;
				}
			}
			x = TempIndex.X - 1 - (TempIndex.X - 1 == 0 ? 1 : 0), y = TempIndex.Y + 1 + (TempIndex.Y + 1 == 0 ? 1 : 0);
			if (x >= -4 && y <= 4 && !Visited.Contains(FVector2D(x, y)))
			{
				if (Found(FVector2D(x, y), TempIndex, Index, Previous, Visited, q))
				{
					Result = FVector2D(x, y);
					goto Finish;
				}
			}
			x = TempIndex.X + 1 + (TempIndex.X + 1 == 0 ? 1 : 0), y = TempIndex.Y - 1 - (TempIndex.Y - 1 == 0 ? 1 : 0);
			if (x <= 4 && y >= -4 && !Visited.Contains(FVector2D(x, y)))
			{
				if (Found(FVector2D(x, y), TempIndex, Index, Previous, Visited, q))
				{
					Result = FVector2D(x, y);
					goto Finish;
				}
			}
			x = TempIndex.X - 1 - (TempIndex.X - 1 == 0 ? 1 : 0), y = TempIndex.Y - 1 - (TempIndex.Y - 1 == 0 ? 1 : 0);
			if (x >= -4 && y >= -4 && !Visited.Contains(FVector2D(x, y)))
			{
				if (Found(FVector2D(x, y), TempIndex, Index, Previous, Visited, q))
				{
					Result = FVector2D(x, y);
					goto Finish;
				}
			}

			for (int i = 0; i < 4; i++)
			{
				int x = TempIndex.X + BaseX[i] + (TempIndex.X + BaseX[i] == 0 ? (BaseX[i] < 0 ? -1 : 1) : 0)
					, y = TempIndex.Y + BaseY[i] + (TempIndex.Y + BaseY[i] == 0 ? (BaseY[i] < 0 ? -1 : 1) : 0);
				if (x >= -4 && x <= 4 && y >= -4 && y <= 4 && !Visited.Contains(FVector2D(x, y)))
				{
					if (Found(FVector2D(x, y), TempIndex, Index, Previous, Visited, q))
					{
						Result = FVector2D(x, y);
						goto Finish;
					}
				}
			}
		}

	}
	else
	{
		if(Index == FVector2D(0, 0))return ActorIndex;
		return Index;
	}

	return ActorIndex;

Finish:
	while (Previous.FindRef(Result) != ActorIndex)
	{
		Result = Previous.FindRef(Result);
	}
	return Result;
}

AMonsterChessCharacter::AMonsterChessCharacter(const FObjectInitializer& ObjectInitializer)
{
	//Component replicated
	GetCapsuleComponent()->SetIsReplicated(true);
	GetSprite()->SetIsReplicated(true);
	GetCharacterMovement()->SetIsReplicated(true);

	//Effect
	HealthEffect = 1;
	ArmorEffect = 1;
	SpeedEffect = 1;
	PhysicalDamageEffect = 1;
	BreakDamageEffect = 1;
	MagicalDamageEffect = 1;
}

void AMonsterChessCharacter::UpdateAnimation_Implementation()
{
	FString Path = "/Game/Character/Animation/" + Name;
	Path += (char)(Level + 48);
	Path += "/" + Name + (UsingSS?"SS":(UsingSkill?"Skill":"Idle"));

	if (Rotation > 22.5&&Rotation <= 67.5)GetSprite()->SetFlipbook(LoadFlipbookReference(Path + "RightBackward"));
	else if (Rotation > 67.5&&Rotation <= 112.5)GetSprite()->SetFlipbook(LoadFlipbookReference(Path + "Right"));
	else if (Rotation > 122.5&&Rotation <= 157.5)GetSprite()->SetFlipbook(LoadFlipbookReference(Path + "RightForward"));
	else if (Rotation > 157.5&&Rotation <= 202.5)GetSprite()->SetFlipbook(LoadFlipbookReference(Path + "Forward"));
	else if (Rotation > 202.5&&Rotation <= 247.5)GetSprite()->SetFlipbook(LoadFlipbookReference(Path + "LeftForward"));
	else if (Rotation > 247.5&&Rotation <= 292.5)GetSprite()->SetFlipbook(LoadFlipbookReference(Path + "Left"));
	else if (Rotation > 292.5&&Rotation <= 337.5)GetSprite()->SetFlipbook(LoadFlipbookReference(Path + "LeftBackward"));
	else GetSprite()->SetFlipbook(LoadFlipbookReference(Path + "Backward"));
}

void AMonsterChessCharacter::UseSkill_Implementation()
{

}

void AMonsterChessCharacter::UseSS_Implementation()
{

}

bool AMonsterChessCharacter::CanUseJump_Implementation()
{
	return !MoveDisable && !ActionDisable && CurrentHealth > 0;
}

bool AMonsterChessCharacter::CanUseSkill_Implementation()
{
	return !AttackDisable && !ActionDisable && CurrentHealth > 0;
}

bool AMonsterChessCharacter::CanUseSS_Implementation()
{
	return false;
}

void AMonsterChessCharacter::ApplyDamage_Implementation(const int & PhysicalDamage, const int & BreakDamage, const int & MagicalDamage)
{
	CurrentHealth = FMath::Max(CurrentHealth - MagicalDamage, 0);
	CurrentArmor = FMath::Max(CurrentArmor - BreakDamage, 0);

	int TempDamage = PhysicalDamage;
	TempDamage = FMath::Max(PhysicalDamage - CurrentArmor, 0);
	CurrentArmor = FMath::Max(CurrentArmor - PhysicalDamage, 0);
	CurrentHealth = FMath::Max(CurrentHealth - TempDamage, 0);

	if (FullEnergy)CurrentEnergy = FMath::Min(CurrentEnergy + 1, FullEnergy);
}

FVector2D AMonsterChessCharacter::FindIndex_Implementation(const float & Radius, const FVector2D & Target, bool & Cannot)
{
	FVector2D ActorIndex = LocationToIndex(GetActorLocation()), Result = ActorIndex;
	Cannot = false;
	if (Distance(IndexToLocation(ActorIndex), IndexToLocation(Target)) > Radius)
	{
		if (ChessRole == "Assassin")
		{
			float MaxDistanceToActor = 0;
			for (int i = -4; i <= 4; i++)
				if (i)
					for (int j = -4; j <= 4; j++)
						if (j && !(BattleInfo->OwnedIndex.Contains(FVector2D(i, j))))
						{
							float DistanceToTarget = Distance(IndexToLocation(FVector2D(i, j)), IndexToLocation(Target))
								, DistanceToActor = Distance(IndexToLocation(FVector2D(i, j)), IndexToLocation(ActorIndex));
							if (DistanceToTarget <= Radius && DistanceToActor > MaxDistanceToActor)
							{
								MaxDistanceToActor = DistanceToActor;
								Result = FVector2D(i, j);
							}
						}
		}
		else
		{
			float MinDistanceToActor = 1e9;
			for (int i = -4; i <= 4; i++)
				if (i)
					for (int j = -4; j <= 4; j++)
						if (j && !(BattleInfo->OwnedIndex.Contains(FVector2D(i, j))))
						{
							float DistanceToTarget = Distance(IndexToLocation(FVector2D(i, j)), IndexToLocation(Target))
								, DistanceToActor = Distance(IndexToLocation(FVector2D(i, j)), IndexToLocation(ActorIndex));
							if (DistanceToTarget <= Radius && DistanceToActor < MinDistanceToActor)
							{
								MinDistanceToActor = DistanceToActor;
								Result = FVector2D(i, j);
							}
						}
			if (MinDistanceToActor == 1e9)Cannot = true;
			else Cannot = false;
		}
	}
	return Result;
}