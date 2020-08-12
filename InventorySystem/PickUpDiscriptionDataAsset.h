// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/Texture2D.h"
#include "PickUpItem.h"
#include "Sound/SoundBase.h"
#include "PickUpDiscriptionDataAsset.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class DELAY_API UPickUpDiscriptionDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
FItemStruct GetItemInfo() const { return itemInfo; }
int GetHowMatchItemsGives() const{ return howMatchItemsGives; }
USoundBase* GetPickupSound() const { return pickupSound; }
int GetSlotNumber() const { return slotNumber; }
UStaticMesh* GetMesh() const { return mesh; }

protected:
    UPROPERTY(EditAnywhere)
        FItemStruct itemInfo;
	UPROPERTY(EditAnywhere)
		int howMatchItemsGives = 1;
	UPROPERTY(EditAnywhere) //Sound when pickUp
		USoundBase* pickupSound = nullptr;
	UPROPERTY(EditAnywhere)
		int slotNumber = 0;
	UPROPERTY(EditAnywhere)
		UStaticMesh* mesh;
};
