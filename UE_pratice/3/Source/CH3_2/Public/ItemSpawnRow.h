#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemSpawnRow.generated.h"

USTRUCT(BlueprintType)
struct FItemSpawnRow : public FTableRowBase //데이터 테이블의 행으로 이것을 쓸 것이다.
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName ItemName; // 아이템 네임 선언
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemClass; // 아이템 클래스 가져오기
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Spawnchance; // 확률
};
