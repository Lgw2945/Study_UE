#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface  //리플렉션 인터페이스
{
	GENERATED_BODY()
};

class CH3_2_API IItemInterface //구현부
{
	GENERATED_BODY()
	 
public:
	UFUNCTION()
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp, //오버랩이 발생 된 자기 자신 (콜리전)
		AActor* OtherActor, //부딪힌 상대 (액터, 플레이어 등)
		UPrimitiveComponent* OtherComp, //충돌을 일으킨 원인 (캡슐 컴포넌트(캐릭터의 경우))
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) = 0; //확정성을 위해 actor (캐릭터만 사용하는게 아님) // = 0; 이 없다면 함수형태로 상속받는 클래스에서 override의 선택이 가능하지만/ = 0;이 있다면 반드시 override를 붙여야만 함 (순수가상함수)
	
	UFUNCTION()
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) = 0; 
	virtual void ActivateItem(AActor* Activator) = 0; //아이템 사용 호출
	virtual FName GetItemType() const = 0; // 리턴만 할 것이기 때문에 const 

};