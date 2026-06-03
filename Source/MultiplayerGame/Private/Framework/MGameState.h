#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Player/PlayerInfoTypes.h"
#include "MGameState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(
	FOnPlayerSelectionUpdated,
	const TArray<FPlayerSelection>&
);

UCLASS()
class MULTIPLAYERGAME_API AMGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	void RequestPlayerSelectionChange(const APlayerState* RequestingPlayer, uint8 DesiredSlot);
	bool IsSlotTaken(uint8 SlotID) const;
	
	FOnPlayerSelectionUpdated OnPlayerSelectionUpdated;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	const TArray<FPlayerSelection>& GetPlayerSelectionChange() const;

	bool CanStart() const;
private:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerSelectionArray)
	TArray<FPlayerSelection> PlayerSelectionArray;

	UFUNCTION()
	void OnRep_PlayerSelectionArray();
};