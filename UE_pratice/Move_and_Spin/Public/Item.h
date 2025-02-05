      // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class CH3_1_API AItem : public AActor
{
	GENERATED_BODY()

public:
	AItem();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components1")
	USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Components2")
	UStaticMeshComponent* StaticComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Properties")
	float RotationSpeed;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
