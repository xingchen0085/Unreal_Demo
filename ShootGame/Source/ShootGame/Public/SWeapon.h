// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"


UCLASS()
class SHOOTGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadonly,Category="Components")
	class USkeletalMeshComponent * MainCom;// Weapon Main Mesh

	UPROPERTY(EditAnywhere, Category = "ParticleSystem")
	class USoundBase* WeaponSound;

	UPROPERTY(EditAnywhere, Category = "ParticleSystem")
	FName MuzzleSocketName;

	UPROPERTY(EditAnywhere, Category = "ParticleSystem")
	class UParticleSystem * MuzzleParticleSystem;

	UPROPERTY(EditAnywhere, Category = "ParticleSystem")
	class UParticleSystem * ImpactParticleSystem;

	UPROPERTY(EditAnywhere, Category = "ParticleSystem")
	class UParticleSystem * TranceParticleSystem;

	UPROPERTY(EditAnywhere, Category = "ParticleSystem")
	class UParticleSystem * ImpactDefaultParticleSystem;

	UPROPERTY(EditAnywhere, Category = "ParticleSystem")
	FName TranceEndBeanName;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float BaseDamage;

	UPROPERTY(EditAnywhere, Category = "Damage")
	TSubclassOf<UDamageType> DamageType;

	void Fire();

	FTimerHandle FireTimerHandler;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "FireParam")
	float AutoFireDuring;// 自动开火时间间隔

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "FireParam")
	float FireDuring;// 开火时间间隔

	UPROPERTY(EditAnywhere, Category = "Components")
	TSubclassOf<class UCameraShake> CameraShake;

	//UPROPERTY(EditAnywhere, Category = "Core")
	//EPhysicalSurface  DefaultSurface;

	//UPROPERTY(EditAnywhere, Category = "Core")
	//EPhysicalSurface VulnerableSurface;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartFire();

	void EndFire();
	
};
