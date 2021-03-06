// Fill out your copyright notice in the Description page of Project Settings.
//This is simple interface for creating visual effect while time control function using;
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TimeEffectsInterface.generated.h"

UINTERFACE(MinimalAPI)
class UTimeEffectsInterface : public UInterface
{
	GENERATED_BODY()
};

class DELAY_API ITimeEffectsInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface")
		void SlowMoEffectIn();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface")
		void SlowMoEffectOut();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface")
		void StopTimeEffectIn();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface")
		void StopTimeEffectOut();
};
