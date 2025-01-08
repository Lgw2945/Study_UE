        #include "MyActor.h"


        AMyActor::AMyActor()
        {
            PrimaryActorTick.bCanEverTick = true;
            start = FVector2D(0, 0);
        }

        void AMyActor::BeginPlay()
        {
            Super::BeginPlay();
            //move()불러오기
            UE_LOG(LogTemp, Warning, TEXT("start : %.0f, %.0f"), start.X, start.Y);
            move();
            createEvent();
        }

        void AMyActor::Tick(float DeltaTime)
        {
            Super::Tick(DeltaTime);

        }

        void AMyActor::move() {

            // 현재 위치 ing에 저장하기
            FVector2D ing = start;

            // 10회 이동
            for (int32 i = 0; i < 10; ++i) {
                // step() 함수 호출하기
                int32 xStep = step();
                int32 yStep = step();

                //위치 업데이트
                ing.X += xStep;
                ing.Y += yStep;

                // 현재 위치 LOG 띄우기
                UE_LOG(LogTemp, Warning, TEXT("Current Position: (%.0f, %.0f)"), ing.X, ing.Y);

                //
                distance(ing.X, ing.Y);
            }

            //for문 이후 최종 위치 업데이트
            start = ing;
        }

        int32 AMyActor::step() {
            //랜덤 0~ 1 정수 리턴하기
            return FMath::RandRange(0, 1);
        }
