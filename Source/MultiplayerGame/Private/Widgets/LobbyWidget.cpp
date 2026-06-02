// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/LobbyWidget.h"
#include "Components/UniformGridSlot.h"
#include "Components/UniformGridPanel.h"
#include "Widgets/TeamSelectionWidget.h"
#include "Network/MNetStatics.h"
#include "Player/LobbyPlayerController.h"
#include "Framework/MGameState.h"

void ULobbyWidget::NativeConstruct() {
	Super::NativeConstruct();
	
	ClearAndLoadTeamSelectionSlots();
	LobbyPlayerController = GetOwningPlayer<ALobbyPlayerController>();
	ConfigGameState();
}

void ULobbyWidget::ClearAndLoadTeamSelectionSlots() {
	TeamSelectionPanel->ClearChildren();
	const int32 PlayersPerTeam = UMNetStatics::GetPlayerCountPerTeam();

	for (int32 i = 0; i < PlayersPerTeam * 2; i++){
		UTeamSelectionWidget* SelectionSlot = CreateWidget<UTeamSelectionWidget>(this, TeamSelectionWidgetClass);

		if (SelectionSlot) {
			SelectionSlot->SetSlotID(i);
			UUniformGridSlot* GridSlot = TeamSelectionPanel->AddChildToUniformGrid(SelectionSlot);
			if (GridSlot) {
				const int32 Row = i % PlayersPerTeam;
				const int32 Column = i / PlayersPerTeam;
				
				GridSlot->SetRow(Row);
				GridSlot->SetColumn(Column);
			}

			SelectionSlot->OnSlotClicked.AddUObject(this, &ULobbyWidget::SlotSelected);
			TeamSelectionSlots.Add(SelectionSlot);
		}
			
	}
}
void ULobbyWidget::SlotSelected(uint8 NewSlotID) {
	if (LobbyPlayerController) {
		LobbyPlayerController->Server_RequestSlotSelectionChange(NewSlotID);
	}
}

void ULobbyWidget::ConfigGameState() {
	UWorld* World = GetWorld();
	if (!World) return;

	GameState = World->GetGameState<AMGameState>();
	if (!GameState) World->GetTimerManager().SetTimer(GonfigTimer, this, &ULobbyWidget::ConfigGameState, 1.f);
	else {
		GameState->OnPlayerSelectionUpdated.AddUObject(this, &ULobbyWidget::UpdatePlayerSelectionDisplay);
		UpdatePlayerSelectionDisplay(GameState->GetPlayerSelectionChange());
	}
}

void ULobbyWidget::UpdatePlayerSelectionDisplay(const TArray<FPlayerSelection>& PlayerSelections) {
	for (UTeamSelectionWidget* SelectionSlot:TeamSelectionSlots) {
		SelectionSlot->UpdateSlotInfo("Empty");
	}

	for ( const FPlayerSelection& PlayerSelection : PlayerSelections) {
		if (!PlayerSelection.IsValid()) continue;
		TeamSelectionSlots[PlayerSelection.GetSlot()]->UpdateSlotInfo(PlayerSelection.GetPlayerNickname());
	}
}
