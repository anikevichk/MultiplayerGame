
#include "MPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"


AMPlayerCharacter::AMPlayerCharacter(){
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->bUsePawnControlRotation = true;
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 720.0, 0);
}

void AMPlayerCharacter::PawnClientRestart(){
	Super::PawnClientRestart();
	APlayerController* PlayerController = GetController<APlayerController>();
	if (PlayerController){
		UEnhancedInputLocalPlayerSubsystem* InputSubsystem = PlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if (InputSubsystem){
			InputSubsystem->RemoveMappingContext(InputMappingContext);
			InputSubsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
	
}

void AMPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent){
		EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &AMPlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &AMPlayerCharacter::HandleLookInput);
		EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &AMPlayerCharacter::HandleMoveInput);
	}
}

void AMPlayerCharacter::HandleLookInput(const FInputActionValue& Input){
	FVector2D InputValue = Input.Get<FVector2D>();
	
	AddControllerPitchInput(InputValue.Y);
	AddControllerYawInput(InputValue.X);
}

void AMPlayerCharacter::HandleMoveInput(const FInputActionValue& Input){
	FVector2D InputValue = Input.Get<FVector2D>();
	InputValue.Normalize();

	AddMovementInput(GetMoveForwardDirection()*InputValue.Y + GetLookRightDirection()*InputValue.X);
}

FVector AMPlayerCharacter::GetLookRightDirection(){
	return Camera->GetRightVector();
}

FVector AMPlayerCharacter::GetLookForwardDirection(){
	return Camera->GetForwardVector();
}

FVector AMPlayerCharacter::GetMoveForwardDirection(){
	return FVector::CrossProduct(GetLookRightDirection(), GetLookForwardDirection());
}
