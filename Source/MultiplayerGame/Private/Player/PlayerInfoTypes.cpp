// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerInfoTypes.h"
#include "GameFramework/PlayerState.h"
#include "Network/MNetStatics.h"

FPlayerSelection::FPlayerSelection()
	:Slot{InvalidSlot}, PlayerUniqueID{FUniqueNetIdRepl::Invalid()}, PlayerName {} {}

FPlayerSelection::FPlayerSelection(uint8 ValidSlot, const APlayerState* PlayerState):Slot{ValidSlot} {
	if (PlayerState) {
		PlayerUniqueID = PlayerState->GetUniqueId();
		PlayerName = PlayerState->GetPlayerName();
	}
}

bool FPlayerSelection::IsForPlayer(const APlayerState* PlayerState) const {
	if (!PlayerState) return false;

	#if WITH_EDITOR
		return PlayerState->GetPlayerName() == GetPlayerNickname();
	#else
		return PlayerState->GetUniqueId() == GetPlayerUniqueID();
	#endif
}

bool FPlayerSelection::IsValid() const {
	#if WITH_EDITOR
		return true;
	#else
		if (!PlayerUniqueID.IsValid() || Slot == UMNetStatics::GetPlayerCountPerTeam()*2)
			return false;
		return true;
	#endif
}
