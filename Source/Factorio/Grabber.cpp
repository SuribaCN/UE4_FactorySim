// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "DrawDebugHelpers.h"

#include "PhysicsEngine/PhysicsHandleComponent.h"

#define OUT

/// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	// Look for attached Input Component (Only appears at run time)
	FindPhysicsComponent();
	SetupInputCoponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
        OUT PlayerViewPointLocation,
        OUT PlayerViewPointRotation);
	FVector LineTracedEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	//定义玩家指向
	if(PhysicsHandle)
	{
		if(PhysicsHandle->GrabbedComponent)
		{
			PhysicsHandle->SetTargetLocation(LineTracedEnd);
		}
	}
	GetSurfacePosition();
	
}
//私有函数定义区
///绑定输入到事件
void UGrabber::SetupInputCoponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComponent)
	{
		//绑定按键行为到执行函数
		//四个参数分别为 输入操作名称,输入方式(按下,轴等),执行函数所处类,具体函数
		///绑定拾起,放下物品
		InputComponent->BindAction("Grab",IE_Pressed,this,&UGrabber::Grab);
		InputComponent->BindAction("Grab",IE_Released,this,&UGrabber::Release);
		//绑定调节手中物品距离,绑定含参函数需要通过代理传参
		DECLARE_DELEGATE_OneParam(ReachAdjustDelegate, const int);
		InputComponent->BindAction<ReachAdjustDelegate>("WheelUp", IE_Pressed, this,&UGrabber::AdjustReach,20);
		InputComponent->BindAction<ReachAdjustDelegate>("WheelDown",IE_Pressed,this,&UGrabber::AdjustReach,-20);
		//绑定生成Actor的键位
		InputComponent->BindAction("Build",IE_Pressed,this,&UGrabber::SpawnActor);
	}
	else
	{
		//当Actor无输入组件时报错
		UE_LOG(LogTemp,Error,TEXT("[%s]'s 'Grabber' INPUTCOMPONENT NOT FOUND!"),*GetOwner()->GetName());
	}
}
///缺少physics handle组件时报错
void UGrabber::FindPhysicsComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		// Physics handle is found
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName())
	}
}
///拾起物品
void UGrabber::Grab()
{
	UE_LOG(LogTemp,Warning,TEXT("pressed"));
	auto HitResult =GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp,Warning,TEXT("Actor Hit!"));
		//抓取物品,不受物理系统影响旋转(碰撞时扔会旋转)
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			GetOwner()->GetActorRotation()
			);
		//抓取物品,不限制旋转
		/*
		PhysicsHandle->GrabComponentAtLocation(
            ComponentToGrab,
            NAME_None,
            ComponentToGrab->GetOwner()->GetActorLocation()
            );
         */
		
	}
}
///释放物品
void UGrabber::Release()
{
	if (PhysicsHandle->GetGrabbedComponent())
	{
		PhysicsHandle->ReleaseComponent();
	}
}
///检测首个命中结果
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
        OUT PlayerViewPointLocation,
        OUT PlayerViewPointRotation);
	FVector lineTracedEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	//生成debug红线
	DrawDebugLine(GetWorld(),PlayerViewPointLocation,lineTracedEnd,FColor(255,255,0),0.f,0.f,0.f,5.f);

	///setuo query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")),false,GetOwner());

	//射线追踪(AKA ray-cast)
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
        OUT Hit,
        PlayerViewPointLocation,
        lineTracedEnd,
        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
        TraceParameters
    );
	AActor* ActorHit = Hit.GetActor();
	if(ActorHit)
	{
		UE_LOG(LogTemp,Warning,TEXT("trace target:%s"),*ActorHit->GetName());
	}

	return Hit;
}
///调节手中物品距离
void UGrabber::AdjustReach(int ReachChange)
{
	if(Reach+ReachChange>70&&ReachChange<0)
	Reach +=ReachChange;
	if(Reach+ReachChange<300&&ReachChange>0)
	Reach +=ReachChange;
}
///TODO 获取长度为BuildDistance的方向向量与待建筑平面的交点
void UGrabber::GetSurfacePosition()
{
	DrawDebugLine(GetWorld(),GetOwner()->GetActorLocation(),GetEndLocation(),FColor(255,255,0));
}
///TODO 在指定位置生成Actor
void UGrabber::SpawnActor()
{
	
}
///定义朝向向量
FVector UGrabber::GetEndLocation()
{
	return GetOwner()->GetActorLocation()+GetOwner()->GetActorRotation().Vector()*BuildDistance;
}