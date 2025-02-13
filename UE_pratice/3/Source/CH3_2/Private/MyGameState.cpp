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
		HUDUpdateTimerHandle, //HUDUpdateTimerHandle��
		this, // �� ��ü����
		&AMyGameState::UpdateHUD, //�� �Լ��� �ҷ���
		0.1f, // 0.1�ʸ���
		true //�ݺ��ض�
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

void AMyGameState::StartLevel() //������ �ҷ��� �� �ٽ� �ʱ�ȭ
{
	//HUD �����ֱ�
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
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes); // ���忡��, ����Ŭ�������� ã�� ����, ���⿡ �ִ´�.

	const int32 ItemToSpawn = ((ItemWave + 1) * 4) + (40 + AddWaveItem); //������ ����

	for (int32 i = 0; i < ItemToSpawn; i++) //���� ī��Ʈ Ȯ��
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass())) // IsA - SpawnedActor�� CoinItem Ŭ�������� Ȯ��, ���� Ŭ������ Ȯ�� �� ��(Small, Big)
				{
					SpawnedCoinCount++;
				}
			}
		}
	}

	//UpdateHUD();

	GetWorldTimerManager().SetTimer(
		LevelTimerHandle, //Ÿ�̸� �ڵ鷯
		this, //��ü
		&AMyGameState::OnLevelTimeUp, //Ÿ�̸Ӱ� ���� ��� ȣ���� �Լ�
		LevelDuration, // Ÿ�̸� �ð�
		false //�ݺ�����
	);

	UE_LOG(LogTemp, Warning, TEXT("Level %d Start!, Spawned %d coin"), CurrentLevelIndex + 1, SpawnedCoinCount);
	UE_LOG(LogTemp, Warning, TEXT("Wave %d Start!"), ItemWave + 1); //Wave
}

void AMyGameState::OnLevelTimeUp() //�ð� ����, ���� ������
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
			UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]); //�ش� ���� �����ش޶�.
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
			UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]); //�ش� ���� �����ش޶�.
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
			MyPlayerController->SetPause(true);//���� ��ü�� ����
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
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time")))) //�ð�
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score")))) // ����
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

				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level")))) //����
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex + 1)));
				}

				if (UTextBlock* WaveIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Wave")))) //���̺�
				{
					WaveIndexText->SetText(FText::FromString(FString::Printf(TEXT("Wave: %d"), ItemWave + 1)));
				}
			}
		}
	}
}
