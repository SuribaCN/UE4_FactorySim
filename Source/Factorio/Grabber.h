// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FACTORIO_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

	public:	
	// Sets default values for this component's properties
	UGrabber();
	UPROPERTY(BlueprintReadWrite)
	float Reach = 400.f;

	protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	private:
	
	float ReachMax = 4000.f;
	float ReachMin = 50.f;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;
	
	void Grab();
	void Release();
	void FindPhysicsComponent();
	void SetupInputCoponent();
	const FHitResult GetFirstPhysicsBodyInReach();
	void AdjustReach(int ReachChange);
	void GetSurfacePosition();
	void SpawnActor();
	FVector GetEndLocation();
	

		
};
