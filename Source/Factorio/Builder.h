// Fill out your copyright notice in the Description page of Project Settings.
//判断视线与地面的交点,生成物品
#pragma once

#include "CoreMinimal.h"

#include "MachineBase.h"
#include "Components/ActorComponent.h"
#include "Builder.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FACTORIO_API UBuilder : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuilder();
	float BuildDistance = 400.f;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//生成的蓝图Actor的父级C++类
	UPROPERTY(EditDefaultsOnly, Category = Custom)
	TSubclassOf<AMachineBase> BlueprintVar;
	
private:
	bool GetFirstPhysicsBodyInReach(FHitResult& Hit);
	void GetSurfacePosition();
	void SpawnActor(FVector Location,FRotator Rotation);
	void BindInputComponent();
	FVector GetEndLocation();
};
