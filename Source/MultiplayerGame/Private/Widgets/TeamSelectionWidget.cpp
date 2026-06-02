// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TeamSelectionWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UTeamSelectionWidget::SetSlotID(uint8 NewSlotID) {
	SlotID = NewSlotID;
}
void UTeamSelectionWidget::UpdateSlotInfo(const FString& PlayerNickname) {
	if (InfoText)
		InfoText->SetText(FText::FromString(PlayerNickname));
}

void UTeamSelectionWidget::NativeConstruct() {
	Super::NativeConstruct();
	if (SelectButton)
		SelectButton->OnClicked.AddDynamic(this, &UTeamSelectionWidget::SelectButtonClicked);
}

void UTeamSelectionWidget::SelectButtonClicked() {
	OnSlotClicked.Broadcast(SlotID);
}
