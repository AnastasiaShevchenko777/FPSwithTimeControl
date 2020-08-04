// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractInterface.h"
#include "PickUpItem.generated.h"

USTRUCT(BlueprintType) //Structer contain base information about item;
struct FItemStruct
{
	GENERATED_BODY()

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FText name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UTexture2D* icon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		bool isStackable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int maxStackSize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<AActor> classToSpawn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<APickUpItem> pickUpClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)//Use to keep information about additional parameters, for example ammo in weapon;
		int ammunitionQuantity;
};

UCLASS()
class DELAY_API APickUpItem : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpItem();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) //Item`s description, it will write to slot
		FItemStruct itemDiscr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) 
		int howMatchItemsGives = 1;

	int GetSlotNumber() const { return slotNumber; }
	USoundBase* GetPickupSound() const { return pickupSound; }

	UFUNCTION(BlueprintCallable)
		void SetAmmunitionQuantity(int newQuantity) { itemDiscr.ammunitionQuantity = newQuantity; };//Use this for weapon, allow to set ammo count after weapon was taken off;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface")
		void Interact();
	virtual void Interact_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface")
		int GetNumberOfSlot();
	virtual int GetNumberOfSlot_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface")
		USoundBase* GetInteractSound();
	virtual USoundBase* GetInteractSound_Implementation() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) //Sound when pickUp
		USoundBase* pickupSound = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int slotNumber = 0;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
