#pragma once

#include "CoreMinimal.h"
#include "GameplayTags.h"
#include "RhythmTypes.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FRhythmSyncEvent, int, BeatCount, bool, bIsDownBeat);

UENUM(BlueprintType)
enum class ERhythmSyncType : uint8
{
	NoteWhole	UMETA(DisplayName = "Note Whole"),
	NoteHalf UMETA(DisplayName = "Note Half"),
	NoteQuarter	UMETA(DisplayName = "Note Quarter"),
	Note8th		UMETA(DisplayName = "Note 8th"),
};

USTRUCT()
struct FRegisteredSyncEvent
{
	GENERATED_BODY()

	UPROPERTY()
	FRhythmSyncEvent Callback;

	UPROPERTY(EditDefaultsOnly, Category="Rhythm System")
	ERhythmSyncType SyncType = ERhythmSyncType::NoteWhole;
	
	UPROPERTY(EditDefaultsOnly, Category="Rhythm System")
	TArray<FGameplayTag> StartTags;

	UPROPERTY(EditDefaultsOnly, Category="Rhythm System")
	TArray<FGameplayTag> CallTags;

	UPROPERTY(EditDefaultsOnly, Category="Rhythm System")
	int32 LoopCount = 0;

	UPROPERTY()
	int32 TriggeredCount = 0;

	UPROPERTY()
	bool bActivated = false;
};
