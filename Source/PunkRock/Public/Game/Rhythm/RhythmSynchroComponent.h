// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RhythmTypes.h"
#include "RhythmSynchroComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBeatEvent, int, BeatCount, bool, bIsDownBeat);
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PUNKROCK_API URhythmSynchroComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URhythmSynchroComponent();

	UPROPERTY(BlueprintAssignable, Category = "Rhythm Component")
	FOnBeatEvent OnBeatEvent;
protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnBeatTriggered(int BeatCount, bool bIsDownBeat) const;
	
	UPROPERTY(EditDefaultsOnly, Category="Rhythm System", meta=(AllowPrivateAccess=true))
	TArray<FRegisteredSyncEvent> RegisteredSyncEvents;
};
