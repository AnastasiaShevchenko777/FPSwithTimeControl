// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UInventoryComponent::ToggleInventory()
{
}

void UInventoryComponent::ShowQuickPanel(int activeSlot)
{
}

void UInventoryComponent::ShowQuickRightPanel()
{
}

bool UInventoryComponent::AddToInventory(FSlotStruct& contentToAdd, int slotIndex)
{
	return false;
}

APickUpItem* UInventoryComponent::DropItem()
{
	return nullptr;
}

void UInventoryComponent::ClearQuickRightMenu()
{
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

