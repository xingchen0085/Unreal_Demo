// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/SCharacter.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/NavMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Public/SWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;//Can Crouch

	// Mesh Param  注意，如果使用原本的蹲起，则相机会跟随，效果不好，所以放弃使用
	//GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-88.0f),FRotator(0,-90.0f,0));// InPitch InYaw InRoll
	bIsCrouch = false;
	ZoomParam = 65.0f;//放大

	// Main Components
	MainCameraSpringArmCom = CreateDefaultSubobject<USpringArmComponent>(TEXT("MainCameraSpringArmCom"));
	MainCameraSpringArmCom->SetupAttachment(RootComponent);
	MainCameraSpringArmCom->bUsePawnControlRotation = true;
	MainCameraSpringArmCom->SetRelativeLocation(FVector(0,70.0f,70.0f));//防止第三人称影响瞄准
	
	MainCameraCom = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCameraCom"));
	MainCameraCom->SetupAttachment(MainCameraSpringArmCom);
	//MainCameraCom->SetRelativeRotation(FRotator(-10.0f, 0, 0));

}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));

	if (StaterWeponClass) {
		// Spawn Weapon Component,SetupAttach to Mesh
		// UClass* Class, FVector const& Location, FRotator const& Rotation, const FActorSpawnParameters& SpawnParameters = FActorSpawnParameters()
		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		CurrentWeapon = GetWorld()->SpawnActor<ASWeapon>(StaterWeponClass,FVector::ZeroVector,FRotator::ZeroRotator,ActorSpawnParameters);

		UE_LOG(LogTemp, Warning, TEXT("Spwan CurrentWeapon"));
		if (CurrentWeapon) {
			CurrentWeapon->SetOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,"WeaponSoket");
			UE_LOG(LogTemp, Warning, TEXT("Spwan CurrentWeapon And attach to Player"));
		}
	}

	DefaultZoom = MainCameraCom->FieldOfView;
	CurrentZoom = MainCameraCom->FieldOfView;
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 使用插值  float Current, float Target, float DeltaTime, float InterpSpeed
	float Result = FMath::FInterpTo(MainCameraCom->FieldOfView, CurrentZoom,DeltaTime,20.0f);

	MainCameraCom->SetFieldOfView(Result);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward",this,&ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&ASCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn",this,&ASCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp",this,&ASCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Crouch",IE_Pressed,this,&ASCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch",IE_Released,this,&ASCharacter::EndCrouch);
	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&ASCharacter::BeginJump);
	PlayerInputComponent->BindAction("Jump",IE_Released,this,&ASCharacter::EndJump);

	PlayerInputComponent->BindAction("Fire",IE_Pressed,this,&ASCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASCharacter::EndFire);

	PlayerInputComponent->BindAction("Zoom",IE_Pressed,this,&ASCharacter::ZoomIn);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ASCharacter::ResumeZoom);
}

void ASCharacter::StartFire()
{
	if (CurrentWeapon) {
		CurrentWeapon->StartFire();
	}
}

void ASCharacter::EndFire()
{
	if (CurrentWeapon) {
		CurrentWeapon->EndFire();
	}
}

FVector ASCharacter::GetPawnViewLocation() const
{
	if (MainCameraCom) {
		return MainCameraCom->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}


void ASCharacter::MoveForward(float Value){
	AddMovementInput(GetActorForwardVector(),Value);
}

void ASCharacter::MoveRight(float Value){
	AddMovementInput(GetActorRightVector(),Value);
}

void ASCharacter::BeginCrouch(){
	//Super::Crouch();
	bIsCrouch = true;
}

void ASCharacter::EndCrouch(){
	//Super::UnCrouch();
	bIsCrouch = false;
}

void ASCharacter::BeginJump(){
	Super::Jump();
}

void ASCharacter::EndJump(){
	Super::StopJumping();
}

void ASCharacter::ZoomIn()
{
	CurrentZoom = ZoomParam;
}

void ASCharacter::ResumeZoom()
{
	CurrentZoom = DefaultZoom;
}

