#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue; // include�� �ٸ��� �����

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

	// Pawn ȸ�� ���� (Yaw)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float TurnRate = 70.f;

	// ī�޶� ���� ȸ�� ���� (Pitch)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float LookUpRate = 70.f;

	// �̵� �� �ӵ� ���� ��� (����/���� ȿ��)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float VelocityInterpSpeed = 10.f;

	// ���������� ����Ǵ� �̵� �ӵ� (AnimBP���� Ȱ��)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	FVector Velocity;

	// --- �Է� �� ���� ---
	FVector2D Move; // �̵� �Է� (X: ����, Y: �¿�)
	FVector2D Look; // Look �Է� (X: Yaw, Y: Pitch)

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; 
	virtual void Tick(float DeltaTime) override;
	//virtual void BeginPlay() override;

private:
	void StartMove(const FInputActionValue& value);
	void StopMove(const FInputActionValue& value);
	void StartLook(const FInputActionValue& value);
	void StopLook(const FInputActionValue& value);
};
