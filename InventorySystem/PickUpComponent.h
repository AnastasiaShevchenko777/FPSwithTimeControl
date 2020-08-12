// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PickUpItem.h"
#include "PickUpComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTakeWeapon);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DELAY_API UPickUpComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPickUpComponent();
	UPROPERTY(BlueprintReadOnly)
		AActor* playerlooksat = nullptr;
	UPROPERTY(BlueprintReadOnly)
		AActor* canTake = nullptr;
	UPROPERTY(BlueprintReadWrite)
		AActor* takenWeapon = nullptr;
	UPROPERTY(BlueprintAssignable)
		FTakeWeapon TakeWeapon;
	UPROPERTY(BlueprintReadOnly)
		int curGrenadeSlot = 3;
	
	UFUNCTION(BlueprintCallable)
		void TakeItem();
	UFUNCTION(BlueprintCallable)
		int GetSlotQuantity(int slotNumber);
	UFUNCTION(BlueprintCallable)
		bool IsThereAnItem(int slotNumer);
	UFUNCTION(BlueprintCallable)
		FItemStruct GetSlotContent(int slotNumber);
	UFUNCTION(BlueprintCallable)
		AActor* SpawnFromSlot(int slotNumber, FVector location, FRotator rotation, USceneComponent* componentToAttach, FName socketName);
	UFUNCTION(BlueprintCallable)
		int TakeItemByTrace();
	UFUNCTION(BlueprintCallable)
		void DecreaseSlotQuantity(int slotNumber, int subtrahend = 1);
	UFUNCTION(BlueprintCallable)
		void SwapGrenade();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void DrawDebug(FVector& playerViewPontLocation, FRotator& playerViewPontRotation);
	FVector GetReachLineEnd();
	FVector GetReachLineStart();
	void ChangeIndex(int& index);

	UPROPERTY(EditAnywhere)
		float Reach = 200; //how far of the player can we reach in cm
	UPROPERTY(EditAnywhere)
		float traceSize = 10.0f;
	UPROPERTY(EditAnywhere)
		bool drawdebug = false;
	//USoundBase* playingSound=nullptr;
	FHitResult GetFirstPhysicsBodyInReach();
	FName GetHitName(FHitResult hitResult);
};
