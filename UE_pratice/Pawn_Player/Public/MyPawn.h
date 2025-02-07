#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue; // include랑 다른거 물어보기

UCLASS()
class CH3_2_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	AMyPawn();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UCapsuleComponent* CapsuleComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USkeletalMeshComponent* SkelMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxWalkSpeed = 600.f;

	// Pawn 회전 감도 (Yaw)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float TurnRate = 70.f;

	// 카메라 상하 회전 감도 (Pitch)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float LookUpRate = 70.f;

	// 이동 시 속도 보간 계수 (가속/감속 효과)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float VelocityInterpSpeed = 10.f;

	// 최종적으로 적용되는 이동 속도 (AnimBP에서 활용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	FVector Velocity;

	// --- 입력 값 저장 ---
	FVector2D Move; // 이동 입력 (X: 전후, Y: 좌우)
	FVector2D Look; // Look 입력 (X: Yaw, Y: Pitch)

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; 
	virtual void Tick(float DeltaTime) override;
	//virtual void BeginPlay() override;

private:
	void StartMove(const FInputActionValue& value);
	void StopMove(const FInputActionValue& value);
	void StartLook(const FInputActionValue& value);
	void StopLook(const FInputActionValue& value);
};
