#include "MyPlayerController.h"
#include "EnhancedInputSubsystems.h"

AMyPlayerController::AMyPlayerController() : InputMappingContext(nullptr),
  MoveAction(nullptr),
  JumpAction(nullptr),
  LookAction(nullptr),
  SprintAction(nullptr) //에디터 상에서 바꿀거라 그냥 소스파일에서 nullptr 처리
{

}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer()) // LocalPlayer = GetLocalPlayer() - '사용자, 플레이어의 입력, 화면 뷰를 관리하는 객체'를 가져와라.
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) // UEnhancedInputLocalPlayerSubsystem - IMC의 추가, 삭제 기능
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0); // InputMappingContext를 0번에 둔다.(낮을수록 우선순위가 높다.)
			}
		}
	}
}