// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPCharacter.generated.h"

UCLASS()
class FIRSTPERSONDEMO_API AFPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//控制移动
	void MoveForward(float Value);

	void MoveRight(float Value);

	UFUNCTION(BlueprintImplementableEvent,Category="Action")
	void ShouldJump();

	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
	void ShouldStopJumping();

	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
	void ShouldCrouch();

	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
	void ShouldStopCrouch();

	//第三人称相机
	UPROPERTY(VisibleDefaultsOnly,Category="Camera")
	class UCameraComponent * ThridPersonCamera;

	UPROPERTY(VisibleDefaultsOnly, Category = "Camera")
	class USpringArmComponent * SpringArm;

	//第一人称相机
	UPROPERTY(VisibleDefaultsOnly, Category = "Camera")
	class UCameraComponent * FristPersonCamera;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
