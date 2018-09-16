// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

UCLASS()
class SHOOTGAME_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// Charater move.
	void MoveForward(float Value);

	void MoveRight(float Value);

	void BeginCrouch();

	void EndCrouch();

	void BeginJump();

	void EndJump();

	void ZoomIn();

	void ResumeZoom();

	UPROPERTY(EditAnywhere, Category = "Param", meta = (UIMin = "5.0", UIMax = "170", ClampMin = "0.001", ClampMax = "360.0", Units = deg))
	float ZoomParam;

	float CurrentZoom;// 当前值

	float DefaultZoom;// 默认值

	// ThirdPersopn Camera
	UPROPERTY(VisibleAnywhere,BlueprintReadonly,Category="Compnents")
	class UCameraComponent * MainCameraCom;

	UPROPERTY(VisibleAnywhere,BlueprintReadonly,Category="Components")
	class USpringArmComponent * MainCameraSpringArmCom;

	UPROPERTY(BlueprintReadOnly, Category = "Player")
	class ASWeapon * CurrentWeapon;

	// starter weapon
	UPROPERTY(EditAnywhere, Category = "Player")
	TSubclassOf<ASWeapon> StaterWeponClass;

	UPROPERTY(EditAnywhere, Category = "Param")
	bool bIsCrouch;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// override view location,if camera is alived,use camera's location
	virtual FVector GetPawnViewLocation() const override;

	void StartFire(); 

	void EndFire();
};
