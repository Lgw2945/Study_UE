#include "MyGameMode.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"
#include "MyGameState.h"


AMyGameMode::AMyGameMode()
{
	DefaultPawnClass = AMyCharacter::StaticClass(); // Ŭ���� �̸����� ȣ��::��ü ������ ���� �ʰ� ����, ��ȯ (����ƽ)
	PlayerControllerClass = AMyPlayerController::StaticClass();
	GameStateClass = AMyGameState::StaticClass();
}