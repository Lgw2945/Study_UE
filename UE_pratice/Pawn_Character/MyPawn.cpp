#include "MyPawn.h"
#include "MyPlayerController.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
//#include "Components/SkeletalMeshComponent.h" //없어도 되는 듯
#include "GameFrameWork/SpringArmComponent.h"

AMyPawn::AMyPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);
	CapsuleComponent->SetSimulatePhysics(false);
	CapsuleComponent->InitCapsuleSize(37.f, 95.f); //캡슐의 사이즈, 반지름 radius와 높이height인듯

	SkelMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skelmesh"));
	SkelMeshComponent->SetupAttachment(CapsuleComponent);
	SkelMeshComponent->SetSimulatePhysics(false);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f; // 스프링암(카메라 시점)의 거리
	SpringArmComp->bUsePawnControlRotation = false; // 컨트롤러가 움직일 때 스프링암도 같이 움직이게 해달라

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);//, USpringArmComponent::SocketName); // 스프링암 컴포넌트의 끝부분(SocketName)에 부착
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
	Move = value.Get<FVector2D>(); // AddMovementInput가 기본으로 없기에 tick으로 실시간 이동 처리
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

	//카메라 회전
	FRotator NewRotation = GetActorRotation();
	FRotator SpringArmRotation = SpringArmComp->GetRelativeRotation();
	
	SpringArmRotation.Yaw = NewRotation.Yaw; // Yaw만 적용할 것, pitch도 계속 0이 되어버림
	
	SpringArmRotation.Pitch = FMath::Clamp(SpringArmRotation.Pitch - (Look.Y * LookUpRate * DeltaTime), -80.0f, 80.0f); // Pitch 제한 걸기
	
	SpringArmComp->AddLocalRotation(FRotator(0.f, Look.X, 0.f));
	
	SpringArmComp->SetRelativeRotation(SpringArmRotation);
	
	//캐릭터 회전
	if (!FMath::IsNearlyZero(Look.X))
	{
		const float YawRotation = Look.X;
		AddActorLocalRotation(FRotator(0.f, YawRotation, 0.f));
	}
	
	//이동
	const FVector Forward = GetActorForwardVector();
	const FVector Right = GetActorRightVector();
	
	const FVector Direction = (Forward * Move.X) + (Right * Move.Y);
	const FVector DesiredVelocity = Direction.GetSafeNormal() * MaxWalkSpeed;
	Velocity = FMath::VInterpTo(Velocity, DesiredVelocity, DeltaTime, VelocityInterpSpeed);
	AddActorWorldOffset(Velocity * DeltaTime, /*bSweep=*/true);
}
