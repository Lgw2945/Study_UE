// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Move.generated.h"

UCLASS()
class CH3_1_API AMove : public AActor
{
	GENERATED_BODY()

public:
	AMove();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components1")
	USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Components2")
	UStaticMeshComponent* StaticComp;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Properties")
	FVector StartLocation;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Properties")
	FVector EndLocation;

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Properties")
	float MoveSpeed;*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Properties")
	float MaxRange;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Properties")
	float MoveSpeed;
	bool MForward;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
