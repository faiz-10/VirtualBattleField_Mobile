



// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
APlayerCharacterBase::APlayerCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TPPCam = CreateDefaultSubobject<UCameraComponent>(TEXT("TPPCamera"));
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));

	CameraBoom->SetupAttachment(RootComponent);
	TPPCam->AttachToComponent(CameraBoom, FAttachmentTransformRules::KeepRelativeTransform);

	CameraBoom->TargetArmLength = 400.f;
	TPPCam->SetRelativeLocation(FVector(0, 0, 100.f));

	TPPCam->bUsePawnControlRotation = false;
	this->bUseControllerRotationPitch = false;
	this->bUseControllerRotationRoll = false;
	this->bUseControllerRotationYaw = false;

}

// Called when the game starts or when spawned
void APlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerCharacterBase::PlayerMovement(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), Input.Y);
	AddMovementInput(GetActorRightVector(), Input.X);
}

void APlayerCharacterBase::PlayerLookAround(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();
	AddControllerPitchInput(Input.X);
	AddControllerYawInput(Input.Y);
}

// Called every frame
void APlayerCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* PlayerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (PlayerInput) {
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController)) {
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->AddMappingContext(IMC_Main, 1);
		}
	}

	PlayerInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerCharacterBase::PlayerMovement);
	PlayerInput->BindAction(IA_Look, ETriggerEvent::Triggered, this, &APlayerCharacterBase::PlayerLookAround);

}

