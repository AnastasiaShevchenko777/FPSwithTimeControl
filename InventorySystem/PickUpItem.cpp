// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpItem.h"
#include "InventoryComponent.h"
#include "GameFramework/GameModeBase.h"
#include "PickUpDiscriptionDataAsset.h"

void APickUpItem::OnConstruction(const FTransform& Transform)
{
	InitializeInfo();
}

// Sets default values
APickUpItem::APickUpItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = mesh;

}

// Called when the game starts or when spawned
void APickUpItem::BeginPlay()
{
	Super::BeginPlay();
	InitializeInfo();
	//UE_LOG(LogTemp, Warning, TEXT(" OnBeginn name is %s"), *(itemDiscr.name.ToString()));
	mesh->SetSimulatePhysics(true);
	mesh->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
}

// Called every frame
void APickUpItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickUpItem::InitializeInfo()
{
	if (discription)
	{
		this->itemDiscr = FItemStruct(discription->GetItemInfo().name,
			discription->GetItemInfo().icon,
			discription->GetItemInfo().isStackable,
			discription->GetItemInfo().maxStackSize,
			discription->GetItemInfo().classToSpawn,
			this->GetClass(),
			discription->GetItemInfo().ammunitionQuantity);
		howMatchItemsGives = discription->GetHowMatchItemsGives();
		pickupSound = discription->GetPickupSound();
		slotNumber = discription->GetSlotNumber();
		mesh->SetStaticMesh(discription->GetMesh());
		UE_LOG(LogTemp, Warning, TEXT("%s OnConstruction called"), *(this->GetName()));
		UE_LOG(LogTemp, Warning, TEXT(" OnConstruction name is %s"), *(itemDiscr.name.ToString()));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PickUp item %s has no info data asset!"), *(this->GetName()));
	}
}

void APickUpItem::Interact_Implementation()
{
	AGameModeBase* gameMode = GetWorld()->GetAuthGameMode();

	if (gameMode)
	{
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
			UE_LOG(LogTemp, Error, TEXT("GameMode has no inventory component"));
	}
	else
		UE_LOG(LogTemp, Error, TEXT("GameMode is not found"));
}

int APickUpItem::GetNumberOfSlot_Implementation()
{
	return this->GetSlotNumber();
}

USoundBase* APickUpItem::GetInteractSound_Implementation()
{
	return this->GetPickupSound();
}