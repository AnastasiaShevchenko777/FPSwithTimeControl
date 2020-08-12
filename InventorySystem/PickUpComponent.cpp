// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpComponent.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"
#include "InventoryComponent.h"

// Sets default values for this component's properties
UPickUpComponent::UPickUpComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}
// Called when the game starts
void UPickUpComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...	
}
// Called every frame
void UPickUpComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	/*
	if (drawdebug)
	{
		FRotator playerViewPontRotation;
		FVector playerViewPontLocation;
		DrawDebug(playerViewPontLocation, playerViewPontRotation);
	}
	*/
}

void UPickUpComponent::DrawDebug(FVector& playerViewPontLocation, FRotator& playerViewPontRotation)
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerViewPontLocation, OUT playerViewPontRotation);
	FVector lineTraceEnd = playerViewPontLocation + (playerViewPontRotation.Vector() * Reach);
	DrawDebugLine(
		GetWorld(),
		//playerViewPontLocation,
		GetReachLineStart(),
		//lineTraceEnd,
		GetReachLineEnd(),
		FColor::White,
		false,
		0,
		0,
		2);
	DrawDebugBox(
		GetWorld(),
		lineTraceEnd,
		FVector(traceSize, traceSize, traceSize),
		FColor::White,
		false,
		0,
		0,
		2);
}

FVector UPickUpComponent::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);
	return PlayerViewPointLocation;
}

FVector UPickUpComponent::GetReachLineEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	FVector reachlineend;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);
	reachlineend = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	return reachlineend;
}

FHitResult UPickUpComponent::GetFirstPhysicsBodyInReach()
{
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	FHitResult hitResult;	//Ray-cast out to reach distance
	/*GetWorld()->SweepSingleByObjectType(
		OUT hitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FQuat(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		FCollisionShape::MakeBox(FVector(traceSize, traceSize, traceSize)),
		TraceParameters);*/
	GetWorld()->LineTraceSingleByObjectType(
		OUT hitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
		);
	return hitResult;
}

FName UPickUpComponent::GetHitName(FHitResult hitResult)
{
	FName name=TEXT("");
	AActor* ActorHit = hitResult.GetActor();
	if (ActorHit)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Trace hit some Actor"));
		APickUpItem* base_pickUp = Cast<APickUpItem>(ActorHit);
		if (base_pickUp) { name = FName(*(ActorHit->GetName())); }
		else { name = TEXT(""); }
	}
	else { name = TEXT(""); }
	return name;
}

void UPickUpComponent::TakeItem()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("InventoryManager work"));
	TArray<AActor*> overlapedItems;
	TSubclassOf<APickUpItem> filter = nullptr;
	GetOwner()->GetOverlappingActors(overlapedItems, filter);
	for (int i = 0; i < overlapedItems.Num(); i++)
	{
			if (overlapedItems[i]->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
			{
				int slotNumber = IInteractInterface::Execute_GetNumberOfSlot(overlapedItems[i]);
				IInteractInterface::Execute_Interact(overlapedItems[i]);

				UInventoryComponent* inventory = ((AActor*)(GetWorld()->GetAuthGameMode()))->FindComponentByClass<UInventoryComponent>();
				if (slotNumber >= 9)
				{
					inventory->ShowQuickRightPanel();
				}
				else
				{
					inventory->ShowQuickPanel(inventory->GetActiveWeaponSlot());
				}
				inventory->UpdateInventoryWidget.Broadcast();
				break;
			}
	}
}

int UPickUpComponent::GetSlotQuantity(int slotNumber)
{
	AGameModeBase* gameMode = GetWorld()->GetAuthGameMode();
	if (gameMode)
	{
		UInventoryComponent* inventory = ((AActor*)gameMode)->FindComponentByClass<UInventoryComponent>();
		if (inventory)
		{
			int quantity = inventory->GetInventoryArray()[slotNumber].quantity;
			return quantity;
		}
		else return 0;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Game mode not found"));
		return 0;
	}
}

void UPickUpComponent::DecreaseSlotQuantity(int slotNumber, int subtrahend/*=1*/)
{
	AGameModeBase* gameMode = GetWorld()->GetAuthGameMode();
	if (gameMode)
	{
		UInventoryComponent* inventory = ((AActor*)gameMode)->FindComponentByClass<UInventoryComponent>();
		if (inventory)
		{
			inventory->GetInventoryArray()[slotNumber].quantity -= subtrahend;
			if (inventory->GetInventoryArray()[slotNumber].quantity < 0)
				inventory->GetInventoryArray()[slotNumber].quantity = 0;
		}
	}
}

void UPickUpComponent::SwapGrenade()
{
	if (GetSlotQuantity(3) || GetSlotQuantity(4) || GetSlotQuantity(5))
	{
		int index = curGrenadeSlot;
		ChangeIndex(index);
	}
}

void UPickUpComponent::ChangeIndex(int& index)
{
	index++;
	if (GetSlotQuantity((index % 3) + 3))
	{
		curGrenadeSlot = (index % 3) + 3;
	}
	else
	{
		ChangeIndex(index);
	}
}

bool UPickUpComponent::IsThereAnItem(int slotNumer)
{
	if (GetSlotQuantity(slotNumer) > 0)
	{
		return true;
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("You have not enough"));
		return false;
	}
}

FItemStruct UPickUpComponent::GetSlotContent(int slotNumber)
{
	AGameModeBase* gameMode = GetWorld()->GetAuthGameMode();
	UInventoryComponent* inventory = ((AActor*)gameMode)->FindComponentByClass<UInventoryComponent>();
	if (inventory)
	{
		return inventory->GetInventoryArray()[slotNumber].Item;
	}
	else
	{
		FSlotStruct empty=FSlotStruct();
		return empty.Item;
	}
}

AActor* UPickUpComponent::SpawnFromSlot(int slotNumber, FVector location, FRotator rotation, USceneComponent* componentToAttach, FName socketName)
{
	FActorSpawnParameters SpawnInfo;
	AGameModeBase* gameMode = GetWorld()->GetAuthGameMode();
	UInventoryComponent* inventory = ((AActor*)gameMode)->FindComponentByClass<UInventoryComponent>();
	AActor* item = nullptr;
	if (inventory)
	{
		TSubclassOf<AActor> spawnClass = inventory->GetInventoryArray()[slotNumber].Item.classToSpawn;
		if (spawnClass != nullptr)
		{
			item = GetWorld()->SpawnActor<AActor>(
				spawnClass,
				location,
				rotation,
				SpawnInfo
				);
			item->AttachToComponent(componentToAttach, FAttachmentTransformRules::KeepWorldTransform, socketName);
			if (inventory->GetInventoryArray()[slotNumber].Item.isStackable)
				inventory->GetInventoryArray()[slotNumber].quantity -= 1;
		}
	}
	return item;
}

int UPickUpComponent::TakeItemByTrace()
{
	int slotNumber = -1;
	if (GetFirstPhysicsBodyInReach().GetActor())
	{
		if (GetFirstPhysicsBodyInReach().GetActor()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
		{
			//UE_LOG(LogTemp, Warning, TEXT("TakeItemByTrace hit something"));
			slotNumber = IInteractInterface::Execute_GetNumberOfSlot(GetFirstPhysicsBodyInReach().GetActor());
			/*if (!playingSound)
			{
				playingSound = IInteractInterface::Execute_GetInteractSound(GetFirstPhysicsBodyInReach().GetActor());
				UAudioComponent* audioComp= UGameplayStatics::PlaySoundAtLocation(GetWorld(), playingSound, GetOwner()->GetActorLocation(), 1.f, 1.f, 0.f, nullptr, nullptr);
				//audioComp->OnAudioFinished()
			}*/
			IInteractInterface::Execute_Interact(GetFirstPhysicsBodyInReach().GetActor());
			UInventoryComponent* inventory = ((AActor*)(GetWorld()->GetAuthGameMode()))->FindComponentByClass<UInventoryComponent>();
			if (slotNumber >= 9)
			{
				inventory->ShowQuickRightPanel();
			}
			else
			{
				inventory->ShowQuickPanel(inventory->GetActiveWeaponSlot());
			}
			inventory->UpdateInventoryWidget.Broadcast();
		}
	}
	return slotNumber;
}


