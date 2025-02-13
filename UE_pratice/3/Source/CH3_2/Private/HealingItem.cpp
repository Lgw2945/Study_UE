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
		if (AMyCharacter* PlayerCharacter = Cast<AMyCharacter>(Activator)) //AActor* Activator�� ĳ����Ŭ������ �°� ĳ����
		{
			PlayerCharacter->AddHealth(HealAmount);
		}
		DestroyItem();
	}
}
