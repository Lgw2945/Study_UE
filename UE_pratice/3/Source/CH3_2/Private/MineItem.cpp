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

void AMineItem::ActivateItem(AActor* Activator) // 타이머 핸들러 사용하기
{
	if (bHasExploded) return;

	Super::ActivateItem(Activator);

	GetWorld()->GetTimerManager().SetTimer(
		ExplosionTimerHandle, 
		this, 
		&AMineItem::Explode, 
		ExplosionDelay, 
		false); //셋팅할 타이머 이름, 이 객체, 실행할 함수, 시간, 반복여부

	bHasExploded = true;
}

void AMineItem::Explode() // 위의 ActivateItem가 실행되고 5초가 지나면 Explode 발동
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

	TArray<AActor*> OverlappingActors; // 범위 내 겹치는 액터 저장 배열
	ExplosionCollision->GetOverlappingActors(OverlappingActors); //오버래핑된 액터 전부 가져오기

	for (AActor* Actor : OverlappingActors) //오버래핑 액터를 for로 돌면서 tag 확인
	{
		UGameplayStatics::ApplyDamage(
			Actor, //데미지를 받을 액터
			ExplosionDamage, // 데미지의 양
			nullptr, //데미지를 유발한 `주체` (지뢰를 심은 주체)
			this, //데미지를 입힌 액터
			UDamageType::StaticClass() //데미지의 유형, 쓴건 가장 기본이 되는 유형
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