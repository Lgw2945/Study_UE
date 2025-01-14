// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    start = FVector2D(0, 0);
    sum = 0;
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("start : %.0f, %.0f"), start.X, start.Y);
    Move();
}


// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyActor::Move() {

    // 현재 위치 current에 저장하기
    FVector2D current = start;

    // 10회 이동
    for (int32 i = 0; i < 10; ++i) {
        // Step() 함수 호출하기
        int32 xStep = Step(0, 1);
        int32 yStep = Step(0, 1);

        //위치 업데이트
        current.X += xStep;
        current.Y += yStep;

        // 현재 위치 LOG 띄우기
        UE_LOG(LogTemp, Warning, TEXT("Current Position: (%.0f, %.0f)"), current.X, current.Y);

        //도전(이동거리 표기)
        distance(current, start);

        start = current;

        //도전(50% 확률로 실행)
        TriggerEventWithProbability(50.0f);

    }
    //도전
    UE_LOG(LogTemp, Warning, TEXT("sum : %d \n"), sum);
    UE_LOG(LogTemp, Warning, TEXT("totDist : %.3f \n"), totDist);
}

int32 AMyActor::Step(int32 a, int32 b) {
    //랜덤 0~ 1 정수 리턴하기
    return FMath::RandRange(a, b);
}

float AMyActor::distance(FVector2D first, FVector2D second) {
    float dx = first.X - second.X;
    float dy = first.Y - second.Y;
    float dist = FMath::Sqrt(dx * dx + dy * dy);
    UE_LOG(LogTemp, Warning, TEXT("Distance between %.3f"), dist);
    totDist += dist;
    return dist;
}

void AMyActor::TriggerEventWithProbability(float Probability)
{
    int32 RandomValue = FMath::RandRange(1, 100); // 1 ~ 100의 난수 생성
    if (RandomValue <= Probability)
    {
        UE_LOG(LogTemp, Log, TEXT("Event Triggered!"));
        sum++;
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Event Not Triggered"));
    }
}
