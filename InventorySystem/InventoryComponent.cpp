// Fill out your copyright notice in the Description page of Project Settings.
#include "InventoryComponent.h"
#include "QuickInventoryPanel.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrepareInventory();
	// ...
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();	
	// ...	
	PrepareInventory();
}
// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

void UInventoryComponent::CreateInventoryWidget()
{
	auto playerController = GetWorld()->GetFirstPlayerController();
	if (InventoryUI)
	{
		currentInventory = CreateWidget<UUserWidget>(playerController, InventoryUI);
		currentInventory->AddToViewport();
		UE_LOG(LogTemp, Warning, TEXT("Inventory widget created succesfuly"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UI For Inventroy not chosen"));
	}
}

void UInventoryComponent::RemoveInventoryWidget()
{
	if (currentInventory)
	{
		currentInventory->RemoveFromViewport();
		currentInventory = nullptr;
		//GetOwner()->GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
		UE_LOG(LogTemp, Warning, TEXT("Inventory closed"));
	}
}

void UInventoryComponent::ToggleInventory()
{
	if (!currentInventory)
	{
		CreateInventoryWidget();
	}
	else
	{
		playWidgetAnim.Broadcast();	//close animation	
		currentInventory = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("Inventory closed"));
	}
}

void UInventoryComponent::ShowQuickPanel(int activeSlot)
{
	if (!currentInventory)
	{
		auto playerController = GetWorld()->GetFirstPlayerController();
		if (QuickPanelUI)
		{
			FWidgetAnimationDynamicEvent ClearQuickMenuDelegate;
			ClearQuickMenuDelegate.BindUFunction(this, "ClearQuickMenu");
			if (PQuickPanel)
			{
				ClearQuickMenu();
			}
			PQuickPanel = CreateWidget<UQuickInventoryPanel>(playerController, QuickPanelUI);
			PQuickPanel->AddToViewport();
			setSlotActive.Broadcast(activeSlot);
			PQuickPanel->BindToAnimationFinished(PQuickPanel->GetShowAnimation(), ClearQuickMenuDelegate);
		}
	}
}

void UInventoryComponent::ShowQuickRightPanel()
{
	if (!currentInventory)
	{
		if (QuickRightPanelUI)
		{
			auto playerController = GetWorld()->GetFirstPlayerController();
			FWidgetAnimationDynamicEvent ClearQuickRightMenuDelegate;
			ClearQuickRightMenuDelegate.BindUFunction(this, "ClearQuickRightMenu");
			if (PQuickRightPanel)
			{
				ClearQuickRightMenu();
			}
			PQuickRightPanel = CreateWidget<UQuickInventoryPanel>(playerController, QuickRightPanelUI);
			PQuickRightPanel->AddToViewport();
			PQuickRightPanel->BindToAnimationFinished(PQuickRightPanel->GetShowAnimation(), ClearQuickRightMenuDelegate);
		}
	}
}

void UInventoryComponent::ClearQuickRightMenu()
{
	PQuickRightPanel->RemoveFromParent();
	PQuickRightPanel = nullptr;
	UE_LOG(LogTemp, Warning, TEXT("closed"));
}

void UInventoryComponent::ClearQuickMenu()
{
	PQuickPanel->RemoveFromParent();
	PQuickPanel = nullptr;
}

bool UInventoryComponent::AddToInventory(FSlotStruct& contentToAdd, int slotIndex)
{
	if (contentToAdd.Item.isStackable)
	{
		if (AddToStack(contentToAdd, slotIndex))
			return true;
		else return false;
	}
	else
		CreateStack(contentToAdd, slotIndex);
	return true;
}

APickUpItem* UInventoryComponent::DropItem()
{
	APickUpItem* newItem = nullptr;
	FActorSpawnParameters SpawnInfo;
	TSubclassOf<AActor> spawnClass = inventoryArray[activeWeaponSlot].Item.pickUpClass;//choose class
	if (spawnClass != nullptr)
	{
		newItem = GetWorld()->SpawnActor<APickUpItem>(
			spawnClass,
			GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(),
			GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorRotation(),
			SpawnInfo
			);

		/* ///For player character, set ammo count to dropped weapon;
		AHumanCharacter* character = Cast<AHumanCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		if (character)
		{
			int ammoCount = 0;
			newItem->SetAmmunitionQuantity(IIFireWeapon::Execute_GetCurrentAmmoCount(character->GetCurrentWeapon()));
		}*/
		inventoryArray[GetActiveWeaponSlot()].quantity -= 1;
		FItemStruct empty = FItemStruct();
		inventoryArray[GetActiveWeaponSlot()].Item = empty;
	}
	ShowQuickPanel(0);
	return newItem;
}

void UInventoryComponent::PrepareInventory()
{
	inventoryArray.SetNum(slotsCount, true);
}

void UInventoryComponent::CreateStack(FSlotStruct contentToAdd, int slotIndex)
{
	inventoryArray[slotIndex] = contentToAdd;
}

bool UInventoryComponent::AddToStack(FSlotStruct& content, int slotIndex)
{
	int currentStackQuantity = inventoryArray[slotIndex].quantity;
	if (currentStackQuantity > 0)//if slot contain item
	{
		if ((currentStackQuantity + content.quantity) <= inventoryArray[slotIndex].Item.maxStackSize)//if slot has place, increase quantity;
		{
			inventoryArray[slotIndex].quantity += content.quantity;
			return true;
		}
		else //set slot quantity to max, decrease item quantity;
		{
			inventoryArray[slotIndex].quantity = inventoryArray[slotIndex].Item.maxStackSize;
			content.quantity -= (inventoryArray[slotIndex].Item.maxStackSize - currentStackQuantity);
			return false;
		}
	}
	else
	{
		CreateStack(content, slotIndex);
		return true;
	}
}