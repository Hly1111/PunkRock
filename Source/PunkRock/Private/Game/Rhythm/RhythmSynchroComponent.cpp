
#include "Game/Rhythm/RhythmSynchroComponent.h"
#include "Game/Rhythm/RhythmSystem.h"

URhythmSynchroComponent::URhythmSynchroComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void URhythmSynchroComponent::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < RegisteredSyncEvents.Num(); i++)
	{
		auto event = &RegisteredSyncEvents[i];
		event->bActivated = (event->StartTags.Num() == 0);
		event->Callback.BindUFunction(this, FName("OnBeatTriggered"));
	}
	
	if (GetWorld() && GetWorld()->GetGameInstance())
	{
		if (URhythmSystem* RhythmSystem = GetWorld()->GetGameInstance()->GetSubsystem<URhythmSystem>())
		{
			for (int i = 0; i < RegisteredSyncEvents.Num(); i++)
			{
				auto event = &RegisteredSyncEvents[i];
				RhythmSystem->SetSyncEvent(*event);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to Get Rhythm System!"))
		}
	}

}

void URhythmSynchroComponent::OnBeatTriggered(int BeatCount, bool bIsDownBeat) const
{
	if (OnBeatEvent.IsBound())
	{
		OnBeatEvent.Broadcast(BeatCount, bIsDownBeat);
	}
}

