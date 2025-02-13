#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface  //���÷��� �������̽�
{
	GENERATED_BODY()
};

class CH3_2_API IItemInterface //������
{
	GENERATED_BODY()
	 
public:
	UFUNCTION()
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp, //�������� �߻� �� �ڱ� �ڽ� (�ݸ���)
		AActor* OtherActor, //�ε��� ��� (����, �÷��̾� ��)
		UPrimitiveComponent* OtherComp, //�浹�� ����Ų ���� (ĸ�� ������Ʈ(ĳ������ ���))
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) = 0; //Ȯ������ ���� actor (ĳ���͸� ����ϴ°� �ƴ�) // = 0; �� ���ٸ� �Լ����·� ��ӹ޴� Ŭ�������� override�� ������ ����������/ = 0;�� �ִٸ� �ݵ�� override�� �ٿ��߸� �� (���������Լ�)
	
	UFUNCTION()
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) = 0; 
	virtual void ActivateItem(AActor* Activator) = 0; //������ ��� ȣ��
	virtual FName GetItemType() const = 0; // ���ϸ� �� ���̱� ������ const 

};