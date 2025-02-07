#include "MyPawn.h"
#include "MyPlayerController.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
//#include "Components/SkeletalMeshComponent.h" //��� �Ǵ� ��
#include "GameFrameWork/SpringArmComponent.h"

AMyPawn::AMyPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);
	CapsuleComponent->SetSimulatePhysics(false);
	CapsuleComponent->InitCapsuleSize(37.f, 95.f); //ĸ���� ������, ������ radius�� ����height�ε�

	SkelMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skelmesh"));
	SkelMeshComponent->SetupAttachment(CapsuleComponent);
	SkelMeshComponent->SetSimulatePhysics(false);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f; // ��������(ī�޶� ����)�� �Ÿ�
	SpringArmComp->bUsePawnControlRotation = false; // ��Ʈ�ѷ��� ������ �� �������ϵ� ���� �����̰� �ش޶�

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);//, USpringArmComponent::SocketName); // �������� ������Ʈ�� ���κ�(SocketName)�� ����
	CameraComp->bUsePawnControlRotation = false;

	Move = FVector2D::ZeroVector;
	Look = FVector2D::ZeroVector;
	Velocity = FVector::ZeroVector;

}

void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::StartMove
				);
			}
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Completed,
					this,
					&AMyPawn::StopMove
				);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::StartLook
				);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Completed,
					this,
					&AMyPawn::StopLook
				);
			}
		}
	}
}

void AMyPawn::StartMove(const FInputActionValue& value)
{
	Move = value.Get<FVector2D>(); // AddMovementInput�� �⺻���� ���⿡ tick���� �ǽð� �̵� ó��
}
void AMyPawn::StopMove(const FInputActionValue& value)
{
	Move = FVector2D::ZeroVector;
}
void AMyPawn::StartLook(const FInputActionValue& value)
{
	Look = value.Get<FVector2D>();
}
void AMyPawn::StopLook(const FInputActionValue& value)
{
	Look = FVector2D::ZeroVector;
}

void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ī�޶� ȸ��
	FRotator NewRotation = GetActorRotation();
	FRotator SpringArmRotation = SpringArmComp->GetRelativeRotation();
	
	SpringArmRotation.Yaw = NewRotation.Yaw; // Yaw�� ������ ��, pitch�� ��� 0�� �Ǿ����
	
	SpringArmRotation.Pitch = FMath::Clamp(SpringArmRotation.Pitch - (Look.Y * LookUpRate * DeltaTime), -80.0f, 80.0f); // Pitch ���� �ɱ�
	
	SpringArmComp->AddLocalRotation(FRotator(0.f, Look.X, 0.f));
	
	SpringArmComp->SetRelativeRotation(SpringArmRotation);
	
	//ĳ���� ȸ��
	if (!FMath::IsNearlyZero(Look.X))
	{
		const float YawRotation = Look.X;
		AddActorLocalRotation(FRotator(0.f, YawRotation, 0.f));
	}
	
	//�̵�
	const FVector Forward = GetActorForwardVector();
	const FVector Right = GetActorRightVector();
	
	const FVector Direction = (Forward * Move.X) + (Right * Move.Y);
	const FVector DesiredVelocity = Direction.GetSafeNormal() * MaxWalkSpeed;
	Velocity = FMath::VInterpTo(Velocity, DesiredVelocity, DeltaTime, VelocityInterpSpeed);
	AddActorWorldOffset(Velocity * DeltaTime, /*bSweep=*/true);
}
