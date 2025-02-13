#include "HealingItem.h"
#include "MyCharacter.h"

AHealingItem::AHealingItem()
{
	ItemType = "Healing";
	HealAmount = 20;
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (AMyCharacter* PlayerCharacter = Cast<AMyCharacter>(Activator)) //AActor* Activator를 캐릭터클래스에 맞게 캐스팅
		{
			PlayerCharacter->AddHealth(HealAmount);
		}
		DestroyItem();
	}
}
