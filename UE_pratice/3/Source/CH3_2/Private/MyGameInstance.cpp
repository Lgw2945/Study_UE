#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
	ItemWave = 0;
}

void UMyGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Total Score Updated : %d"), TotalScore);
}

void UMyGameInstance::AddToWave(int32 Amount)
{
	ItemWave = Amount;
	AddWaveItem = Amount * 10;
	//UE_LOG(LogTemp, Warning, TEXT("123 %d"), ItemWave);
}