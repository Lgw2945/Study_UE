#include "MyGameState.h"
#include "SpawnVolume.h"
#include "CoinItem.h"
#include "MyGameInstance.h"
#include "Engine/Engine.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"

AMyGameState::AMyGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 15.0f;
	CurrentLevelIndex = 0;
	MaxLevels = 3;
	ItemWave = 0;
	AddWaveItem = 0;
}

void AMyGameState::BeginPlay()
{
	Super::BeginPlay();

	UpdateHUD();
	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle, //HUDUpdateTimerHandle를
		this, // 이 객체에서
		&AMyGameState::UpdateHUD, //이 함수를 불러라
		0.1f, // 0.1초마다
		true //반복해라
	);
}

int32 AMyGameState::GetScore() const
{
	return Score;
}

void AMyGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
		if (MyGameInstance)
		{
			MyGameInstance->AddToScore(Amount);
		}
	}
}

void AMyGameState::StartLevel() //레벨을 불러올 때 다시 초기화
{
	//HUD 보여주기
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* SpartaPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			SpartaPlayerController->ShowGameHUD();
		}
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
		if (MyGameInstance)
		{
			CurrentLevelIndex = MyGameInstance->CurrentLevelIndex;
			ItemWave = MyGameInstance->ItemWave;
			AddWaveItem = MyGameInstance->AddWaveItem;
		}
	}

	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes); // 월드에서, 볼륨클래스에서 찾은 것을, 여기에 넣는다.

	const int32 ItemToSpawn = ((ItemWave + 1) * 4) + (40 + AddWaveItem); //스폰할 개수

	for (int32 i = 0; i < ItemToSpawn; i++) //코인 카운트 확인
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass())) // IsA - SpawnedActor가 CoinItem 클래스인지 확인, 하위 클래스도 확인 해 줌(Small, Big)
				{
					SpawnedCoinCount++;
				}
			}
		}
	}

	//UpdateHUD();

	GetWorldTimerManager().SetTimer(
		LevelTimerHandle, //타이머 핸들러
		this, //객체
		&AMyGameState::OnLevelTimeUp, //타이머가 끝날 경우 호출할 함수
		LevelDuration, // 타이머 시간
		false //반복여부
	);

	UE_LOG(LogTemp, Warning, TEXT("Level %d Start!, Spawned %d coin"), CurrentLevelIndex + 1, SpawnedCoinCount);
	UE_LOG(LogTemp, Warning, TEXT("Wave %d Start!"), ItemWave + 1); //Wave
}

void AMyGameState::OnLevelTimeUp() //시간 만료, 다음 레벨로
{
	EndLevel();
}

void AMyGameState::OnCoinCollected()
{
	CollectedCoinCount++;
	UE_LOG(LogTemp, Warning, TEXT("Coin Collected : %d / %d"), CollectedCoinCount, SpawnedCoinCount);

	if (SpawnedCoinCount >= 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndLevel();
	}
}


void AMyGameState::AddWave(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
		if (MyGameInstance)
		{
			MyGameInstance->AddToWave(Amount);
		}
	}
}


void AMyGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	if (ItemWave < 2)
	{
		int32 a = ++ItemWave;
		AddWave(a);

		if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
		{
			UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]); //해당 맵을 오픈해달라.
		}
	}
	else
	{
		AddWave(0);


		if (UGameInstance* GameInstance = GetGameInstance())
		{
			UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
			if (MyGameInstance)
			{
				AddScore(Score);
				CurrentLevelIndex++;
				MyGameInstance->CurrentLevelIndex = CurrentLevelIndex;
			}
		}

		if (CurrentLevelIndex >= MaxLevels)
		{
			OnGameOver();
			return;
		}
		if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
		{
			UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]); //해당 맵을 오픈해달라.
		}
		else
		{
			OnGameOver();
		}
	}
}

void AMyGameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			MyPlayerController->SetPause(true);//게임 자체를 멈춤
			MyPlayerController->ShowMainMenu(true);
		}
	}
	//UpdateHUD();
}

void AMyGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = MyPlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time")))) //시간
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score")))) // 점수
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
						if (MyGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), MyGameInstance->TotalScore)));
						}
					}
				}

				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level")))) //레벨
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex + 1)));
				}

				if (UTextBlock* WaveIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Wave")))) //웨이브
				{
					WaveIndexText->SetText(FText::FromString(FString::Printf(TEXT("Wave: %d"), ItemWave + 1)));
				}
			}
		}
	}
}
