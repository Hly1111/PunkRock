// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Rhythm/RhythmSystem.h"
#include "Game/Rhythm/RhythmData.h"
#include "AkGameplayStatics.h"


URhythmSystem::URhythmSystem(): _currentBPM(0), _currentSongId(0), _noteCounter(1)
{
	OnBeatCallback.BindUFunction(this, FName("OnBeatCallbackFunc"));
}

void URhythmSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	RhythmData= Cast<URhythmData>(StaticLoadObject(URhythmData::StaticClass(), nullptr, TEXT("/Game/00PunkRock/Data/DA_RhythmData")));
	if (!RhythmData) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load RhythmData!"));
		return;
	}

	AkEvent_Map = TMap<FGameplayTag, TObjectPtr<FMusicInfo>>();
	for (FMusicInfo& MusicInfo : RhythmData->AkEvent_Info)
	{
		if (MusicInfo.AkEvent_PlayMusic)
		{
			AkEvent_Map.Add(MusicInfo.MusicNameTag, MusicInfo);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AkEvent_PlayMusic not set for %s!"), *MusicInfo.MusicNameTag.ToString());
		}
	}
	Rtpc_PlaybackSpeed = RhythmData->RTPC_PlaybackSpeed;
	if (!Rtpc_PlaybackSpeed)
	{
		UE_LOG(LogTemp, Warning, TEXT("RTPC_PlaybackSpeed not set in RhythmData!"));
	}
}

void URhythmSystem::SetNewBPM(int newBPM)
{
	newBPM = FMath::Clamp(newBPM, AK_MIN_BPM, AK_MAX_BPM);
	_currentBPM = newBPM;
	float newPlaybackSpeed = newBPM/_originalBPM;
	UAkGameplayStatics::SetRTPCValue(Rtpc_PlaybackSpeed, newPlaybackSpeed, AK_INTERPOLATION_TIME, nullptr);
}

void URhythmSystem::OnBeatCallbackFunc(EAkCallbackType type, UAkCallbackInfo* info)
{
	if (type == EAkCallbackType::MusicSyncBeat)
	{
		++_noteCounter;
		if (_noteCounter > AK_BAR_NOTES_COUNT)
		{
			_noteCounter = 1;
		}

		const FString TagName = FString::Printf(TEXT("RSS.NoteCount.%02d"), _noteCounter);
		const FGameplayTag CurrentTag = FGameplayTag::RequestGameplayTag(FName(*TagName));
		for (int32 i = RegisteredEvents.Num() - 1; i >= 0; --i)
		{
			FRegisteredSyncEvent& Event = RegisteredEvents[i];
			if (!Event.bActivated)
			{
				if (Event.StartTags.Contains(CurrentTag))
				{
					Event.bActivated = true;
				}
				else
				{
					continue;
				}
			}
			if (Event.CallTags.Contains(CurrentTag))
			{
				if (Event.Callback.ExecuteIfBound(_noteCounter, (_noteCounter == 1)))
				{
					if (Event.LoopCount > 0)
					{
						Event.TriggeredCount++;
						if (Event.TriggeredCount >= Event.LoopCount)
						{
							RegisteredEvents.RemoveAt(i);
						}
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Event callback failed for note count %d!"), _noteCounter);
				}
			}

			
		}
	}
}

void URhythmSystem::PlayBGM(FGameplayTag MusicNameTag)
{
	CurrentBgmEvent = AkEvent_Map[MusicNameTag]->AkEvent_PlayMusic;
	_originalBPM = AkEvent_Map[MusicNameTag]->OriginalBPM;
	if (!CurrentBgmEvent) return;
	constexpr int32 CallbackMusk = AK_MusicSyncBeat| AK_EnableGetSourcePlayPosition;
	_currentSongId = UAkGameplayStatics::PostEvent(CurrentBgmEvent, nullptr, CallbackMusk, OnBeatCallback);
	if (_currentSongId == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to post BGM event!"));
		return;
	};
	SetNewBPM(_originalBPM);
}

int URhythmSystem::GetCurrentPositionMs()
{
	return UAkGameplayStatics::GetSourcePlayPosition(CurrentBgmEvent, _currentSongId);
}

float URhythmSystem::GetCurrentPosition()
{
	int totalPositionMs = GetCurrentPositionMs();
	int positionInBar = totalPositionMs % static_cast<int>(GetBeatInterval() * 1000.f * 4.f); // Get position in the current bar of 4 beats
	return positionInBar/1000.f;
}

float URhythmSystem::GetBeatInterval()
{
	return 60.f/static_cast<float>(_currentBPM); // Return the current beat interval in milliseconds
}

bool URhythmSystem::SetSyncEvent(const FRhythmSyncEvent& EventCallback, ERhythmSyncType SyncType,
FGameplayTagContainer StartTags, FGameplayTagContainer CallTags, int32 LoopCount)
{
	
	if (CallTags.Num() == 0 || !EventCallback.IsBound()) return false;

	FRegisteredSyncEvent Event;
	Event.Callback = EventCallback;
	Event.SyncType = SyncType;
	Event.StartTags = StartTags.GetGameplayTagArray();
	Event.CallTags = CallTags.GetGameplayTagArray();
	Event.LoopCount = LoopCount;
	Event.bActivated = (StartTags.Num() == 0);
	RegisteredEvents.Add(Event);
	return true;
}

bool URhythmSystem::SetSyncEvent(const FRegisteredSyncEvent& EventCallback)
{
	RegisteredEvents.Add(EventCallback);
	return true;
}




