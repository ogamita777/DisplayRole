#include "GameMode/TestGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

ATestGameModeBase::ATestGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> playerPawnClass(TEXT("/Game/Blueprints/Player/BP_TestPlayer"));
	if (playerPawnClass.Class != NULL)
	{
		DefaultPawnClass = playerPawnClass.Class;
	}
}

AActor* ATestGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	if (PlayerStartTagList.IsValidIndex(m_spawnIndex))
	{
		TArray<AActor*> playerStartActorList;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), playerStartActorList);

		for (AActor* playerStartActor : playerStartActorList)
		{
			APlayerStart* playerStart = Cast<APlayerStart>(playerStartActor);
			if (playerStart->PlayerStartTag == PlayerStartTagList[m_spawnIndex])
			{
				m_spawnIndex++;

				return playerStartActor;
			}
		}
	}

	return Super::ChoosePlayerStart_Implementation(Player);
}