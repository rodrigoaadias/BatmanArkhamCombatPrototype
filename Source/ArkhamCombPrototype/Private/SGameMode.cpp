#include "SGameMode.h"
#include "SCharacter.h"
#include "Kismet/GameplayStatics.h"

void ASGameMode::OnActorKilled(AActor* VictimActor, AActor* KillerActor)
{
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);
	if(Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RestartLevel");

		constexpr float RespawnDelay = 3.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}
	else
	{
		APawn* KillerPawn = Cast<APawn>(KillerActor);
		if(KillerPawn)
		{
			
		}
	}

	UE_LOG(LogTemp, Log, TEXT("OnActorKilled - Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(KillerActor));
}

void ASGameMode::RestartLevel()
{
	const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	UGameplayStatics::OpenLevel(GetWorld(), FName(CurrentLevelName));
}

void ASGameMode::NotifyLastEnemyRemaining()
{
	bLastEnemyRemaining = true;
}

void ASGameMode::ResetEnemyRemaining()
{	
	bLastEnemyRemaining = false;
}
