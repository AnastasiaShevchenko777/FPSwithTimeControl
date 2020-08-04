// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeControlComponent.h"
#include <GameFramework/PawnMovementComponent.h>
#include "TimeEffectsInterface.h"
// Sets default values for this component's properties
UTimeControlComponent::UTimeControlComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}

// Called when the game starts
void UTimeControlComponent::BeginPlay()
{
	Super::BeginPlay();
	this->SetComponentTickEnabled(false);
	// ...
}

// Called every frame
void UTimeControlComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (isTimeStoped)
	{
		float curTime = GetWorld()->GetUnpausedTimeSeconds();
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT(" %f"), curTime));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT(" %f"), freezeTime));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT(" %f"), curTime-freezeTime));
		if ( curTime - freezeTime > 1.f && curTime - freezeTime < 2.f)
		{
			freezeTime = GetWorld()->GetUnpausedTimeSeconds();
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("2 seconds passed")));
			DecDelayStop();
		}
	}
}

void UTimeControlComponent::IncreaseDelay()
{
	if (curDelay < maxDelay)
	{
		curDelay += addDelayInSecond;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("current delay is %f"), curDelay));
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(resDelayTimer);
		return;
	}
}

void UTimeControlComponent::SlowToNorm()
{
	GetWorld()->GetTimerManager().ClearTimer(slowMoTimer);
	GetWorld()->GetWorldSettings(false, true)->SetTimeDilation(1.f);
	isTimeNormal = true;
	isTimeSlow = false;
	this->SetComponentTickEnabled(false);
	//ITimeEffectsInterface::Execute_SlowMoEffectOut(visualEffects);
}

void UTimeControlComponent::SetTimeToNormal()
{
	if (isTimeSlow)
	{
		SlowToNorm();
	}
	else
	{
		FreezeTime(false);
	}
	//interface call
	DelayRessurection();
}

void UTimeControlComponent::FreezeTime(bool enable)
{
	this->SetComponentTickEnabled(enable);
	isTimeNormal = !enable;
	isTimeSlow = false;
	isTimeStoped = enable;
	this->SetTickableWhenPaused(enable);
	//GetWorld()->GetFirstPlayerController()->ShouldPerformFullTickWhenPaused() TODO
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->SetTickableWhenPaused(enable);
	GetWorld()->GetFirstPlayerController()->SetPause(enable);
	GetWorld()->GetFirstPlayerController()->GetPawn()->GetMovementComponent()->SetTickableWhenPaused(enable);
	if (!enable)
	{
		GetWorld()->GetTimerManager().ClearTimer(freezTimer);
		//ITimeEffectsInterface::Execute_StopTimeEffectOut(visualEffects);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(freezTimer, this, &UTimeControlComponent::DecDelayStop, 0.1f, true);
		freezeTime = GetWorld()->GetUnpausedTimeSeconds();
		//ITimeEffectsInterface::Execute_StopTimeEffectIn(visualEffects);
	}
}

void UTimeControlComponent::SlowMo()
{
	if (curDelay > 1)
	{
		if (isTimeNormal)
		{
			MakeTimeSlower();
		}
		else
		{
			if (isTimeSlow)
			{
				SlowToNorm();
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Time un slow"));
				SlowMoEffects(false);
				DelayRessurection();
			}
			else
			{
				FreezeTime(false);
				MakeTimeSlower();
			}
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Delay is empty"));
	}
}

inline void UTimeControlComponent::MakeTimeSlower()
{
	this->SetComponentTickEnabled(true);
	isTimeNormal = false;
	isTimeSlow = true;
	GetWorld()->GetWorldSettings(false, true)->SetTimeDilation(0.3f);
	GetWorld()->GetTimerManager().SetTimer(slowMoTimer, this, &UTimeControlComponent::DecDelaySlowMo, 0.1f, true);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Time slower"));
	///TODO call interface
	//ITimeEffectsInterface::Execute_SlowMoEffectIn(visualEffects);
}

void UTimeControlComponent::SlowMoEffects(bool enable)
{
}

void UTimeControlComponent::DelayRessurection()
{
	GetWorld()->GetTimerManager().SetTimer(resDelayTimer, this, &UTimeControlComponent::IncreaseDelay, 3.f, true);
}

void UTimeControlComponent::DecDelaySlowMo()
{
	if (curDelay <= 10)
	{
		SetTimeToNormal();
	}
	else
	{
		curDelay -= DelayDecriment;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("current delay is %f"), curDelay));
	}
}
void UTimeControlComponent::DecDelayStop()
{
	if (curDelay <= 0)
	{
		SetTimeToNormal();
	}
	else
	{
		curDelay -= DelayDecriment * 100;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("current delay is %f"), curDelay));
	}
}

	void UTimeControlComponent::StopTime()
	{
		if (curDelay > 1)
		{
			if (isTimeNormal)
			{
				GetWorld()->GetTimerManager().ClearTimer(resDelayTimer);
				FreezeTime(true);
				SlowMoEffects(true);
			}
			else
			{
				if (isTimeStoped)
				{
					FreezeTime(false);
					SlowMoEffects(false);
					DelayRessurection();
				}
				else
				{
					GetWorld()->GetTimerManager().ClearTimer(resDelayTimer);
					SlowToNorm();
					FreezeTime(true);
				}
			}
		}
	}

