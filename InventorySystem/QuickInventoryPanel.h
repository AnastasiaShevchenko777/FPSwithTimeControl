// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuickInventoryPanel.generated.h"

/**
 * 
 */
UCLASS()
class DELAY_API UQuickInventoryPanel : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UWidgetAnimation* SwapSlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UWidgetAnimation* Fade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UWidgetAnimation* Close;
public:
	class UWidgetAnimation* GetShowAnimation() const { return SwapSlot; }
	void SetShowAnimation(class UWidgetAnimation* val) { SwapSlot = val; }
};
