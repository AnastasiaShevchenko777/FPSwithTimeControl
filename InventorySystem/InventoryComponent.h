// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include <Runtime\UMG\Public\Components\CanvasPanel.h>
#include "PickUpItem.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayWidgetAnimation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetActiveSlot, int, slotNumber);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateInventoryWidget);

USTRUCT(BlueprintType)

struct FSlotStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FItemStruct Item;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)//How match item of this type contains in slot;
		int quantity; 
};

class UQuickInventoryPanel;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DELAY_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();
	UFUNCTION(BlueprintCallable)
		TArray<FSlotStruct> GetInventoryArray() const { return inventoryArray; }
	UFUNCTION(BlueprintCallable)
		void SetInventoryArray(TArray<FSlotStruct> val) { inventoryArray = val; }

	UFUNCTION(BlueprintCallable)	//Returns current slot count
		int GetSlotsCount() const { return slotsCount; }
	
	UFUNCTION(BlueprintCallable)	//Pointer for inventory main widget
		class UUserWidget* GetCurrentInventory() const { return currentInventory; }
	
	UFUNCTION(BlueprintCallable)
		void ToggleInventory();
	
	UFUNCTION(BlueprintCallable) //Show main inventory panel 
		void ShowQuickPanel(int activeSlot);
	
	UFUNCTION(BlueprintCallable) //Show right inventory panel
		void ShowQuickRightPanel();
	
	UFUNCTION(BlueprintCallable)
		bool AddToInventory(FSlotStruct& contentToAdd, int slotIndex);
	
	UFUNCTION(BlueprintCallable)
		APickUpItem* DropItem();
	
	UPROPERTY(BlueprintAssignable) //Widgets events for animation call
		FPlayWidgetAnimation playWidgetAnim;
	
	UPROPERTY(BlueprintAssignable)
		FSetActiveSlot setSlotActive;
	
	UPROPERTY(BlueprintAssignable)
		FUpdateInventoryWidget UpdateInventoryWidget;
	
	UFUNCTION(BlueprintCallable)
		void SetPQuickPanel(UQuickInventoryPanel* val) { PQuickPanel = val; }
	
	UFUNCTION(BlueprintCallable)
		UTexture2D* GetEmptySlotIcon() const { return emptySlotIcon; }
	
	UFUNCTION(BlueprintCallable)//slot, that should me marked as active
		int GetActiveWeaponSlot() const { return activeWeaponSlot; }
	
	UFUNCTION(BlueprintCallable)
		void SetActiveWeaponSlot(int val) { activeWeaponSlot = val; }
	
	UFUNCTION(BlueprintCallable)
		void ClearQuickRightMenu();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;		

private:
	UPROPERTY(EditDefaultsOnly)	//Cell contents
		TArray <FSlotStruct> inventoryArray;		
	UPROPERTY(EditDefaultsOnly)	// Reference UMG Asset in the Editor(main window)
		TSubclassOf<class UUserWidget> InventoryUI;
	UPROPERTY(EditDefaultsOnly)	// Reference UMG Asset in the Editor(quick lower panel)
		TSubclassOf<class UQuickInventoryPanel> QuickPanelUI;
	UPROPERTY(EditDefaultsOnly)	// Reference UMG Asset in the Editor(quick right panel)
		TSubclassOf<class UQuickInventoryPanel> QuickRightPanelUI;
	///Widgets pointers
	UQuickInventoryPanel* PQuickPanel = nullptr;
	UQuickInventoryPanel* PQuickRightPanel = nullptr;
	class UUserWidget* currentInventory;// Variable to hold the widget After Creating it.

	void ClearQuickMenu();
	void CreateInventoryWidget(); //Creating inventory widget and attached to main HUD
	void RemoveInventoryWidget();

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
		int slotsCount = 14;
	UPROPERTY(EditDefaultsOnly, Category = "Properties")
		UTexture2D* emptySlotIcon;

	void PrepareInventory();
	void CreateStack(FSlotStruct contentToAdd, int slotIndex);
	bool AddToStack(FSlotStruct& content, int slotIndex);
	int activeWeaponSlot = 0;
};
