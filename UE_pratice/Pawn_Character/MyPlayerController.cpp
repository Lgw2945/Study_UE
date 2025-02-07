#include "MyPlayerController.h"
#include "EnhancedInputSubsystems.h"

AMyPlayerController::AMyPlayerController() : InputMappingContext(nullptr),
  MoveAction(nullptr),
  JumpAction(nullptr),
  LookAction(nullptr),
  SprintAction(nullptr) //������ �󿡼� �ٲܰŶ� �׳� �ҽ����Ͽ��� nullptr ó��
{

}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer()) // LocalPlayer = GetLocalPlayer() - '�����, �÷��̾��� �Է�, ȭ�� �並 �����ϴ� ��ü'�� �����Ͷ�.
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) // UEnhancedInputLocalPlayerSubsystem - IMC�� �߰�, ���� ���
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0); // InputMappingContext�� 0���� �д�.(�������� �켱������ ����.)
			}
		}
	}
}