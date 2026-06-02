// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LobbyPlayerController.h"
#include "Framework/MGameState.h"
#include "GameFramework/PlayerState.h"

void ALobbyPlayerController::Server_RequestSlotSelectionChange_Implementation(uint8 SlotID) {
	if (!GetWorld()) return;

	AMGameState* GameState = GetWorld()->GetGameState<AMGameState>();
	if (!GameState) return;

	GameState->RequestPlayerSelectionChange(GetPlayerState<APlayerState>(), SlotID);
}

bool ALobbyPlayerController::Server_RequestSlotSelectionChange_Validate(uint8 SlotID) {
	return true;
}
