// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/MAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMAnimInstance::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if (OwnerCharacter) OwnerMovementComponent = OwnerCharacter->GetCharacterMovement();
}

void UMAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (OwnerCharacter) {
		MovementSpeed = OwnerCharacter->GetVelocity().Length();
		FRotator BodyRotatation = OwnerCharacter->GetActorRotation();
		FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(BodyRotatation, PreviousRotationSpeed);
		PreviousRotationSpeed = BodyRotatation;

		YawSpeed = Delta.Yaw/DeltaSeconds;
		SmoothedYawSpeed = UKismetMathLibrary::FInterpTo(SmoothedYawSpeed, YawSpeed, DeltaSeconds, SmoothedYawLerpSpeed);

		FRotator ControlRotation = OwnerCharacter->GetBaseAimRotation();
		LookRotationOffset = UKismetMathLibrary::NormalizedDeltaRotator(ControlRotation, BodyRotatation);
	}

	if (OwnerMovementComponent) bIsJumping = OwnerMovementComponent->IsFalling();
}

void UMAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds) {
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
}
