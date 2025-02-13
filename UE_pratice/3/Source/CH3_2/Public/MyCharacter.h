#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;
struct FInputActionValue; //구조체 (큼, 복사 비용이 크기에 참조&)

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
		float DamageAmount, //데미지 양
		struct FDamageEvent const& DamageEvent, //데미지 유형
		AController* EventIntigator, //데미지를 발생시킨 주체 (데미지를 준 오브젝트가 아닌 결과가 된 주체)
		AActor* DamageCauser) override; //데미지를 준 오브젝트

	//아래 코드들은 EnhancedInput(입력 처리 시스템)에서 인식하려면 최소한 리플렉션 시스템이 붙어야 함
	UFUNCTION() //혹시 모를 문제를 예방하기 위한 최소한의 리플렉션 시스템은 필요함
	void Move(const FInputActionValue& value); // Value Type(BP에 2D 벡터) 값 / 참조된 객체 수정 못하게 const
	UFUNCTION()
	void StartJump(const FInputActionValue& value); // bool타입, 눌렸을 때
	UFUNCTION()
	void StopJump(const FInputActionValue& value);  // 뗐을 때
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
