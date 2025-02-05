#include "Move.h"

// Sets default values
AMove::AMove()
{
    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    StaticComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticComp->SetupAttachment(SceneRoot);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("'/Game/Resources/Props/Floor_400x400.Floor_400x400'"));
    if (MeshAsset.Succeeded())
    {
        StaticComp->SetStaticMesh(MeshAsset.Object);
    }
    static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("'/Game/Resources/Materials/M_Brick_Clay_Beveled.M_Brick_Clay_Beveled'"));
    if (MaterialAsset.Succeeded())
    {
        StaticComp->SetMaterial(0, MaterialAsset.Object);
    }

    PrimaryActorTick.bCanEverTick = true;
    MaxRange = 100.0f;
    MoveSpeed = 50.0f;
    MForward = true;
}

void AMove::BeginPlay()
{
    Super::BeginPlay();

    StartLocation = GetActorLocation();
    EndLocation = StartLocation + FVector(MaxRange, 0, 0);
}

// Called every frame
void AMove::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector CurrentLocation = GetActorLocation();

    if (!FMath::IsNearlyZero(MaxRange))
    {
        if (MForward)
        {
            CurrentLocation += FVector(MoveSpeed * DeltaTime, 0, 0);
            if (FVector::Dist(CurrentLocation, EndLocation) < 3.0f)
            {
                CurrentLocation = EndLocation;
                MForward = false;
            }
        }
        else
        {
            CurrentLocation -= FVector(MoveSpeed * DeltaTime, 0, 0);
            if (FVector::Dist(CurrentLocation, StartLocation) < 3.0f)
            {
                CurrentLocation = StartLocation;
                MForward = true;
            }
        }

        SetActorLocation(CurrentLocation);
    }
}
