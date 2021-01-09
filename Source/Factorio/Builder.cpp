// Fill out your copyright notice in the Description page of Project Settings.


#include "Builder.h"

#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UBuilder::UBuilder()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//构造生成蓝图actor所必需的准备
	static ConstructorHelpers::FObjectFinder<UBlueprint> PutNameHere(TEXT("Blueprint'/Game/ConveyorBelt_BP.ConveyorBelt_BP'"));  
	if (PutNameHere.Object)   
	{
		UE_LOG(LogTemp,Error,TEXT("NOT A ERROR,ObjectFindSuccess"));
		BlueprintVar = (UClass*)PutNameHere.Object->GeneratedClass;  
	}  
	
	// ...
}


// Called when the game starts
void UBuilder::BeginPlay()
{
	Super::BeginPlay();
	UWorld* const World = GetWorld(); // get a reference to the world  
	if (World)   
	{  
		// if world exists  
		AMachineBase* YC = World->SpawnActor<AMachineBase>(BlueprintVar, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());  
	}  
	
}


// Called every frame
void UBuilder::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//获取与地面的交线
const FHitResult UBuilder::GetFirstPhysicsBodyInReach()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
        OUT PlayerViewPointLocation,
        OUT PlayerViewPointRotation);
	FVector lineTracedEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * BuildDistance;
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

///TODO 获取长度为BuildDistance的方向向量与待建筑平面的交点
void UBuilder::GetSurfacePosition()
{
	//DrawDebugLine(GetWorld(),GetOwner()->GetActorLocation(),GetEndLocation(),FColor(255,255,0));
}
///TODO 在指定位置生成Actor
void UBuilder::SpawnActor(FHitResult Hit)
{
	Hit.Location;
	
}
///定义朝向向量
FVector UBuilder::GetEndLocation()
{
	return GetOwner()->GetActorLocation()+GetOwner()->GetActorRotation().Vector()*BuildDistance;
}