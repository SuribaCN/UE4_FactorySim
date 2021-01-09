// Fill out your copyright notice in the Description page of Project Settings.
//TODO 当碰撞箱接触自身碰撞体积时移动自身
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"

#include "ConveyorBelt.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FACTORIO_API UConveyorBelt : public UActorComponent
{
	GENERATED_BODY()

	public:	
	// Sets default values for this component's properties
	UConveyorBelt();

	protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	AActor* InPut();
	AActor* OutPut();
	UBoxComponent* Box;
	UPROPERTY(BlueprintReadWrite)
	float Speed = 0.8f;//传送带速度

	private:
	//变量声明区
	int TotalObjectCount=0;//传送带上当前传送目标数
	bool IsPowerSuppy;//TODO 是否有电力供应
	bool IsConveyorBeltBlocked;//TODO 是否被阻塞
	

	//函数声明区
	void StartTransport();
	void PauseTransport();
	void StopTransport();
	UFUNCTION( )
    void BeginOverlap(UPrimitiveComponent* OverlappedComponent, 
                      AActor* OtherActor, 
                      UPrimitiveComponent* OtherComp, 
                      int32 OtherBodyIndex, 
                      bool bFromSweep, 
                      const FHitResult &SweepResult );
	UFUNCTION( )
    void EndOverlap(UPrimitiveComponent* OverlappedComponent, 
                      AActor* OtherActor, 
                      UPrimitiveComponent* OtherComp, 
                      int32 OtherBodyIndex);
	void EnableComponentsSimulatePhysics(AActor* Actor);
	
	
	
	
		
};
//TODO 阻塞类型,不确定是否可以如此声明枚举
UENUM()
namespace EBlockType
{
	enum EMyEnum
	{
		EBT_Full,
        EBT_NoStorageSpace
    };
}
