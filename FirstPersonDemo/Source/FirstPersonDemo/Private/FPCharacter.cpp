// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/FPCharacter.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AFPCharacter::AFPCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 350.0f;//臂长
	SpringArm->bEnableCameraLag = true;
	//SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = 10.0f;
	//SpringArm->CameraRotationLagSpeed = 10.0f;
	
	ThridPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThridPersonCamera"));
	ThridPersonCamera->SetupAttachment(SpringArm);
	ThridPersonCamera->AddLocalOffset(FVector(0,0,90.0f));
	ThridPersonCamera->bUsePawnControlRotation = true;

}

// Called when the game starts or when spawned
void AFPCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward",this,&AFPCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AFPCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp",this,&AFPCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&AFPCharacter::ShouldJump);
	PlayerInputComponent->BindAction("Jump",IE_Released,this,&AFPCharacter::ShouldStopJumping);

	PlayerInputComponent->BindAction("Crouch",IE_Pressed,this,&AFPCharacter::ShouldCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AFPCharacter::ShouldStopCrouch);
}


void AFPCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(),Value);
}

void AFPCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}
