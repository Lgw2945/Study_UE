#include "MineItem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"


AMineItem::AMineItem()
{
	ItemType = "Mine";
	ExplosionDelay = 0.5f;
	ExplosionRadius = 300.0f;
	ExplosionDamage = 30;
	bHasExploded = false;


	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionCollision->SetupAttachment(Scene);
	//ExplosionCollision->OnComponentBeginOverlap.AddDynamic(this, &AMineItem::OnItemOverlap); // 
}

void AMineItem::ActivateItem(AActor* Activator) // Ÿ�̸� �ڵ鷯 ����ϱ�
{
	if (bHasExploded) return;

	Super::ActivateItem(Activator);

	GetWorld()->GetTimerManager().SetTimer(
		ExplosionTimerHandle, 
		this, 
		&AMineItem::Explode, 
		ExplosionDelay, 
		false); //������ Ÿ�̸� �̸�, �� ��ü, ������ �Լ�, �ð�, �ݺ�����

	bHasExploded = true;
}

void AMineItem::Explode() // ���� ActivateItem�� ����ǰ� 5�ʰ� ������ Explode �ߵ�
{
	UParticleSystemComponent* Particle = nullptr;

	if (ExplosionParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ExplosionParticle,
			GetActorLocation(),
			GetActorRotation(),
			false
		);
	}

	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			ExplosionSound,
			GetActorLocation()
		);
	}

	TArray<AActor*> OverlappingActors; // ���� �� ��ġ�� ���� ���� �迭
	ExplosionCollision->GetOverlappingActors(OverlappingActors); //�������ε� ���� ���� ��������

	for (AActor* Actor : OverlappingActors) //�������� ���͸� for�� ���鼭 tag Ȯ��
	{
		UGameplayStatics::ApplyDamage(
			Actor, //�������� ���� ����
			ExplosionDamage, // �������� ��
			nullptr, //�������� ������ `��ü` (���ڸ� ���� ��ü)
			this, //�������� ���� ����
			UDamageType::StaticClass() //�������� ����, ���� ���� �⺻�� �Ǵ� ����
		);
	}

	DestroyItem();

	if (Particle)
	{
		FTimerHandle DestroyParticleTimerHandle;
		TWeakObjectPtr<UParticleSystemComponent> WeakParticle = Particle;

		GetWorld()->GetTimerManager().SetTimer(
			DestroyParticleTimerHandle,
			[WeakParticle]()
		{
			if (WeakParticle.IsValid())
			{
				WeakParticle->DestroyComponent();
			}
		},
			2.0f,
			false
		);
	}
}