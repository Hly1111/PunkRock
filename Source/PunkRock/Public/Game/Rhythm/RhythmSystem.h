// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <AkGameplayTypes.h>
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RhythmTypes.h"
#include "GameplayTagContainer.h"
#include "RhythmSystem.generated.h"
struct FMusicInfo;
struct FGameplayTagContainer;
class UAkGameplayStatics;
class UAkRtpc;
class URhythmData;

/**
 * 
 */
// The song is divided into bars, each bar contains 8 notes, 8th is the shortest.
UCLASS()
class PUNKROCK_API URhythmSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	URhythmSystem();

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	const int AK_BAR_NOTES_COUNT = 8; // number of notes in a bar
	const int AK_INTERPOLATION_TIME = 100; // ms
	const int AK_MIN_BPM = 60; // minimum BPM
	const int AK_MAX_BPM = 240; // maximum BPM
	
	int _originalBPM;
	int _currentBPM;
	TObjectPtr<UAkAudioEvent> CurrentBgmEvent;
	int _currentSongId;
	int _noteCounter;

	UPROPERTY(EditDefaultsOnly, Category="Rhythm System")
	URhythmData* RhythmData;

	TMap<FGameplayTag, TObjectPtr<FMusicInfo>> AkEvent_Map;
	//TObjectPtr<UAkAudioEvent> AkEvent_PlayBgm;
	TObjectPtr<UAkRtpc>  Rtpc_PlaybackSpeed;

	UPROPERTY()
	TArray<FRegisteredSyncEvent> RegisteredEvents;

	UFUNCTION(BlueprintCallable, Category="Rhythm System")
	void SetNewBPM(int newBPM);
	
public:
	UPROPERTY()
	FOnAkPostEventCallback OnBeatCallback;
	UFUNCTION()
	void OnBeatCallbackFunc(EAkCallbackType type, UAkCallbackInfo* info);
	
	UFUNCTION(BlueprintCallable, Category = "Rhythm System")
	void PlayBGM(FGameplayTag MusicNameTag);

	UFUNCTION(BlueprintCallable, Category = "Rhythm System")
	int GetCurrentPositionMs();

	UFUNCTION(BlueprintCallable, Category = "Rhythm System")
	float GetCurrentPosition();

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Rhythm System")
	float GetBeatInterval();

	UFUNCTION(BlueprintCallable, Category = "Rhythm System", meta = (BlueprintThreadSafe))
	bool SetSyncEvent(
		const FRhythmSyncEvent& EventCallback,
		ERhythmSyncType SyncType,
		FGameplayTagContainer StartTags,
		FGameplayTagContainer CallTags,
		int32 LoopCount = 0);

	bool SetSyncEvent(const FRegisteredSyncEvent& EventCallback);
};
