#include "MyGameMode.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"
#include "MyGameState.h"


AMyGameMode::AMyGameMode()
{
	DefaultPawnClass = AMyCharacter::StaticClass(); // 클래스 이름으로 호출::객체 생성을 하지 않고 선언, 반환 (스태틱)
	PlayerControllerClass = AMyPlayerController::StaticClass();
	GameStateClass = AMyGameState::StaticClass();
}