#include "Framework/MGameState.h"
#include "Net/UnrealNetwork.h"

void AMGameState::RequestPlayerSelectionChange(const APlayerState* RequestingPlayer, uint8 DesiredSlot) {
	if (!HasAuthority() || IsSlotTaken(DesiredSlot)) return;
	
	FPlayerSelection* PlayerSelectionPtr = PlayerSelectionArray.FindByPredicate([&](const FPlayerSelection& PlayerSelection) {
		return PlayerSelection.IsForPlayer(RequestingPlayer);
	});

	if (PlayerSelectionPtr) PlayerSelectionPtr->SetSlot(DesiredSlot);
	else PlayerSelectionArray.Add(FPlayerSelection(DesiredSlot, RequestingPlayer));	

	OnPlayerSelectionUpdated.Broadcast(PlayerSelectionArray);
}

bool AMGameState::IsSlotTaken(uint8 SlotID) const {
	for (const FPlayerSelection& PlayerSelection: PlayerSelectionArray) {
		if (PlayerSelection.GetSlot() == SlotID) {
			return true;
		}
	}
	return false;
}

void AMGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(
		AMGameState,
		PlayerSelectionArray,
		COND_None,
		REPNOTIFY_Always
	);
}

const TArray<FPlayerSelection>& AMGameState::GetPlayerSelectionChange() const {
	return PlayerSelectionArray;
}

void AMGameState::OnRep_PlayerSelectionArray(){
	OnPlayerSelectionUpdated.Broadcast(PlayerSelectionArray);
}
