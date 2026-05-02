#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "MPlayerCharacter.h"

AMPlayerCharacter::AMPlayerCharacter(){
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(GetRootComponent());
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
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
	}
}
