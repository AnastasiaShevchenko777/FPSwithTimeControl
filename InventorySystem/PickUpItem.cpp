// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpItem.h"

// Sets default values
APickUpItem::APickUpItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APickUpItem::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void APickUpItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickUpItem::Interact_Implementation()
{
	AGameModeBase* gameMode = GetWorld()->GetAuthGameMode();

	if (gameMode)
	{/*
		UInventoryComponent* inventory = gameMode->FindComponentByClass<UInventoryComponent>();
		if (inventory)
		{
			//UE_LOG(LogTemp, Warning, TEXT("called pickUpItem Interact"));
			FSlotStruct content = FSlotStruct{ this->itemDiscr, howMatchItemsGives };
			if (inventory->AddToInventory(content, slotNumber))
			{
				//UGameplayStatics::PlaySound2D(GetWorld(), GetPickupSound());
				this->Destroy();
			}
		}
		else
			UE_LOG(LogTemp, Error, TEXT("GameMode has no inventory component"));*/
	}
	else
		UE_LOG(LogTemp, Error, TEXT("GameMode is not found"));
}

int APickUpItem::GetNumberOfSlot_Implementation()
{
	return this->slotNumber;
}

USoundBase* APickUpItem::GetInteractSound_Implementation()
{
	return this->pickupSound;
}