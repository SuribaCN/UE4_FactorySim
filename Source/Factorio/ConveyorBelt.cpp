// Fill out your copyright notice in the Description page of Project Settings.


#include "ConveyorBelt.h"


#include "DrawDebugHelpers.h"
#include "BehaviorTree/BTFunctionLibrary.h"

// Sets default values for this component's properties
UConveyorBelt::UConveyorBelt()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void UConveyorBelt::BeginPlay()
{
	Super::BeginPlay();
	Box=GetOwner()->FindComponentByClass<UBoxComponent>();
	//检测是否有碰撞盒组件
	if(!Box)
	{UE_LOG(LogTemp,Error,TEXT("No box component"))}
	else
	{UE_LOG(LogTemp,Display,TEXT("Find box component"))}
	//盒体碰撞事件绑定函数
	Box->OnComponentBeginOverlap.AddDynamic(this,&UConveyorBelt::BeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this,&UConveyorBelt::EndOverlap);
}


// Called every frame
void UConveyorBelt::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//绘制传送方向的debug箭头
	FVector lineTracedEnd=GetOwner()->GetActorLocation()+GetOwner()->GetActorRotation().Vector()*50;
	DrawDebugDirectionalArrow(GetWorld(),GetOwner()->GetActorLocation(),lineTracedEnd,20.f,FColor(255,0,0),false,-1,1,5.f);
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//将传送带上的Actors沿传送带运行方向移动
	TArray<AActor*> BoxOverlapActors;
	Box->GetOverlappingActors(BoxOverlapActors);
	for(int i = 0;i<BoxOverlapActors.Num();i++)
	{
		//使Actor在传送带上移动,并且朝向与传送带一致
		//BoxOverlapActors[i]->DisableComponentsSimulatePhysics();
		BoxOverlapActors[i]->SetActorLocation((BoxOverlapActors[i]->GetActorLocation()+GetOwner()->GetActorRotation().Vector()*Speed));
		//UE_LOG(LogTemp,Error,TEXT("%f,%f,%f"),BoxOverlapActors[i]->GetActorRotation().Pitch,BoxOverlapActors[i]->GetActorRotation().Yaw,BoxOverlapActors[i]->GetActorRotation().Roll);
		FRotator ActorRotatorOnBelt(GetOwner()->GetActorRotation().Pitch,BoxOverlapActors[i]->GetActorRotation().Yaw,BoxOverlapActors[i]->GetActorRotation().Roll);
		//UE_LOG(LogTemp,Warning,TEXT("%f,%f,%f"),ActorRotatorOnBelt.Pitch,ActorRotatorOnBelt.Yaw,ActorRotatorOnBelt.Roll);
		BoxOverlapActors[i]->SetActorRotation(ActorRotatorOnBelt);
	}
}
///Actor进入box范围时触发,停用其物理效果
void  UConveyorBelt::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult )
{
	//UE_LOG(LogTemp,Display,TEXT("BeginOverlap:OtherActor: %s"),*OtherActor->GetName());
	OtherActor->DisableComponentsSimulatePhysics();
}
///Actor离开box范围时触发,启用其物理效果
void  UConveyorBelt::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp,Display,TEXT("EndOverlap:OtherActor: %s"),*OtherActor->GetName());
	EnableComponentsSimulatePhysics(OtherActor);
}
//启用Actor模拟物理
void UConveyorBelt::EnableComponentsSimulatePhysics(AActor* Actor)
{
	for (UActorComponent* Component : Actor->GetComponents())
	{
		if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(Component))
		{
			PrimComp->SetSimulatePhysics(true);
		}
	}
}