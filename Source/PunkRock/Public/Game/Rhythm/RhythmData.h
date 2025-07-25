// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTags.h"
#include "RhythmData.generated.h"

class UAkAudioEvent;
class UAkRtpc;

USTRUCT(BlueprintType)
struct FMusicInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MusicNameTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAkAudioEvent> AkEvent_PlayMusic;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int OriginalBPM;
};
/**
 * 
 */
UCLASS(BlueprintType)
class PUNKROCK_API URhythmData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FMusicInfo> AkEvent_Info;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAkRtpc> RTPC_PlaybackSpeed;
};
