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
	// default constructor;
	FItemStruct() :
		name(TEXT("default")),icon(nullptr),isStackable(true),maxStackSize(5),classToSpawn(nullptr),pickUpClass(nullptr),ammunitionQuantity(0) 	{}
	// constructor with parametrs;
	FItemStruct(FName _name, UTexture2D* _icon, bool _isStackable, int _maxStackSize, TSubclassOf<AActor> _classToSpawn, TSubclassOf<APickUpItem> _pickUpClass, int _ammunitionQuantity) : 
		name(_name), icon(_icon), isStackable(_isStackable), maxStackSize(_maxStackSize), classToSpawn(_classToSpawn), pickUpClass(_pickUpClass), ammunitionQuantity(_ammunitionQuantity) {}
	// copy constructor;
	FItemStruct(const FItemStruct& item) :
		name(item.name), icon(item.icon), isStackable(item.isStackable), maxStackSize(item.maxStackSize), classToSpawn(item.classToSpawn), pickUpClass(item.pickUpClass), ammunitionQuantity(item.ammunitionQuantity) {}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FName name;
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

class UPickUpDiscriptionDataAsset;

UCLASS()
class DELAY_API APickUpItem : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:
	virtual void OnConstruction(const FTransform& Transform) override;

	// Sets default values for this actor's properties
	APickUpItem();
	
	UFUNCTION(BlueprintCallable)
		FItemStruct GetItemInfo() const { return itemDiscr; }
	UFUNCTION(BlueprintCallable)
		void SetItemProperties(FItemStruct _newVal) { itemDiscr = _newVal; }
	
	UFUNCTION(BlueprintCallable)
		int GetSlotNumber() const { return slotNumber; }
	UFUNCTION(BlueprintCallable)
		void SetSlotNumber(int _slotNumber) { slotNumber = _slotNumber; }

	UFUNCTION(BlueprintCallable)
		int GetHowMatchItemsGives() const { return howMatchItemsGives; }
	
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

	USoundBase* GetPickupSound() const { return pickupSound; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;				
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UPickUpDiscriptionDataAsset* discription=nullptr;
	
	FItemStruct itemDiscr; //Item`s description, it will write to slot
	USoundBase* pickupSound = nullptr;//Sound when pickUp
	int howMatchItemsGives = 1;
	int slotNumber = 0;
	
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* mesh = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	void InitializeInfo();
};
