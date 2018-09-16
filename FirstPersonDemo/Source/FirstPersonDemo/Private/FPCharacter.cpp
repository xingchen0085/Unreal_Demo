// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/FPCharacter.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AFPCharacter::AFPCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetCapsuleComponent());
	SkeletalMesh->SetRelativeLocationAndRotation(FVector(-20.0f, 0, -151.0f),FRotator(0,-90.0f,0));

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(SkeletalMesh);
	FirstPersonCamera->SetRelativeLocation(FVector(20, 0, BaseEyeHeight));
	FirstPersonCamera->bUsePawnControlRotation = true;

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(SkeletalMesh, SkeletalSoketName);
	GunMesh->SetRelativeLocationAndRotation(FVector(0,0,0),FRotator(24.18f,-101.2f, 150.47644f));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = 350.0f;//臂长
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 10.0f;
	
	
	ThridPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThridPersonCamera"));
	ThridPersonCamera->SetupAttachment(SpringArm);
	ThridPersonCamera->SetRelativeLocation(FVector(0,0,90.0f));
	ThridPersonCamera->bUsePawnControlRotation = true;


	
	UCharacterMovementComponent * CharacterMovement = GetCharacterMovement();
	CharacterMovement->JumpZVelocity = 500.0f;//跳起来的高度
	
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
