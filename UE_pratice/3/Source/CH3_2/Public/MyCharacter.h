#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;
struct FInputActionValue; //����ü (ŭ, ���� ����� ũ�⿡ ����&)

UCLASS()
class CH3_2_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverheadWidget;

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(
		float DamageAmount, //������ ��
		struct FDamageEvent const& DamageEvent, //������ ����
		AController* EventIntigator, //�������� �߻���Ų ��ü (�������� �� ������Ʈ�� �ƴ� ����� �� ��ü)
		AActor* DamageCauser) override; //�������� �� ������Ʈ

	//�Ʒ� �ڵ���� EnhancedInput(�Է� ó�� �ý���)���� �ν��Ϸ��� �ּ��� ���÷��� �ý����� �پ�� ��
	UFUNCTION() //Ȥ�� �� ������ �����ϱ� ���� �ּ����� ���÷��� �ý����� �ʿ���
	void Move(const FInputActionValue& value); // Value Type(BP�� 2D ����) �� / ������ ��ü ���� ���ϰ� const
	UFUNCTION()
	void StartJump(const FInputActionValue& value); // boolŸ��, ������ ��
	UFUNCTION()
	void StopJump(const FInputActionValue& value);  // ���� ��
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);

	void OnDeath();
	void UpdateOverheadHP();

private:
	float NormalSpeed;
	float SprintSpeedMultiplier;
	float SprintSpeed;
};
