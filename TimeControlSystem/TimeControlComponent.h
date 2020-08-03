// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimeControlComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DELAY_API UTimeControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTimeControlComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
		void StopTime();
	UFUNCTION(BlueprintCallable)
		void SlowMo();
private:
	bool isTimeNormal = true;
	bool isTimeStoped = false;
	bool isTimeSlow = false;
	FTimerHandle slowMoTimer;
	FTimerHandle resDelayTimer;
	FTimerHandle freezTimer;
	float freezeTime = 2.f;
	float curDelay=100.f;
	UPROPERTY(EditDefaultsOnly)
		float maxDelay = 100.f;
	UPROPERTY(EditDefaultsOnly)
		float DelayDecriment = 0.005f;
	UPROPERTY(EditDefaultsOnly)
		float addDelayInSecond = 0.5f;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AActor> visualEffects;
	void IncreaseDelay();
	void SlowToNorm();
	void DecDelaySlowMo();
	void DecDelayStop();
	void FreezeTime(bool enable);	
	inline void MakeTimeSlower();
	void SlowMoEffects(bool enable);
	void DelayRessurection();
	void SetTimeToNormal();
};
