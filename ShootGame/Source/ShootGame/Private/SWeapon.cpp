// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/SWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "ShootGame.h"

static int32 ShowDebugLineFlag = 0;//为1显示:正式环境不用
FAutoConsoleVariableRef CVARWeaponDebugLine(
	TEXT("ShowWeaponDebugLine"), ShowDebugLineFlag, TEXT("显示子弹轨迹帮助线，1 显示，0 不显示（默认）"), ECVF_Cheat
);

// Sets default values
ASWeapon::ASWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MuzzleSocketName = "MuzzleSocket";
	TranceEndBeanName = "BeamEnd";
	BaseDamage = 20.0f;
	AutoFireDuring = 0.15f;
	FireDuring = 0.15f;

	MainCom = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MainCom"));
}

// Called when the game starts or when spawned
void ASWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASWeapon::StartFire()
{
	GetWorldTimerManager().SetTimer(FireTimerHandler, this, &ASWeapon::Fire, AutoFireDuring, true, 0);
}

void ASWeapon::EndFire()
{
	GetWorldTimerManager().ClearTimer(FireTimerHandler);
}



void ASWeapon::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire.."));
	// Eyes location and vector,add 10000
	// FVector ViewLocation = GetPawnViewLocation();
	// FRotator ViewRotation = GetViewRotation();
	// FVector EndLocation = ViewRotation.Vector() * 10000;
	AActor * Owner = GetOwner();
	if (Owner == nullptr) {
		return;
	}
	FVector ViewLocation;
	FRotator ViewRotation;
	Owner->GetActorEyesViewPoint(ViewLocation, ViewRotation);

	FVector EndLocation = ViewRotation.Vector() * 10000;
	FVector TranceEndPoint = EndLocation;
	// struct FHitResult & OutHit,
	// const FVector & Start,
	// const FVector & End,
	// ECollisionChannel TraceChannel,
	// const FCollisionQueryParams & Params,
	// const FCollisionResponseParams & ResponseParam
	FHitResult OutHit;
	FCollisionQueryParams QueryParams;
	FCollisionResponseParams  ResponseParam;

	// ignore self
	QueryParams.AddIgnoredActor(GetOwner());
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;// 返回三角面
	QueryParams.bReturnPhysicalMaterial = true;//返回物理材质

	bool TranceResult = GetWorld()->LineTraceSingleByChannel(OutHit, ViewLocation, EndLocation,
		ECollisionChannel::ECC_Visibility, QueryParams, ResponseParam);

	if (ShowDebugLineFlag == 1) {
		// draw debugger line
		// const UWorld* InWorld, FVector const& LineStart, FVector const& LineEnd, FColor const& Color, bool bPersistentLines = false, float LifeTime = -1.f, uint8 DepthPriority = 0, float Thickness = 0.f
		DrawDebugLine(GetWorld(), ViewLocation, EndLocation, FColor::White, true, 60.0f, 0, 3.0f);
	}

	// 枪口特效
	if (MuzzleParticleSystem) {
		UGameplayStatics::SpawnEmitterAttached(MuzzleParticleSystem,MainCom, MuzzleSocketName);//	枪口生成粒子
	}

	// 枪声
	if (WeaponSound) {
	
		UGameplayStatics::SpawnSoundAttached(WeaponSound, MainCom, MuzzleSocketName);//	枪口位置
	}

	// 镜头抖动
	APawn * OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn) {
		APlayerController * PC = Cast<APlayerController>(OwnerPawn->GetController());
		//抖动
		if (PC) {
			// SubclassOf<class UCameraShake> Shake, float Scale = 1.f, ECameraAnimPlaySpace::Type PlaySpace = ECameraAnimPlaySpace::CameraLocal, FRotator UserPlaySpaceRot = FRotator::ZeroRotator
			if (CameraShake) {
				PC->ClientPlayCameraShake(CameraShake);
			}
		}
	}


	if (TranceResult) {
		AActor * HitActor = OutHit.GetActor();
		TranceEndPoint = OutHit.ImpactPoint;

		UE_LOG(LogTemp, Warning, TEXT("Trance Hit something,%s"), *HitActor->GetName());

		// 获得击中的材质
		EPhysicalSurface HitSurface = UPhysicalMaterial::DetermineSurfaceType(OutHit.PhysMaterial.Get());
		float ResultDamge = BaseDamage;
		UParticleSystem * ResultImpactParticleSystem = ImpactDefaultParticleSystem;
		switch (HitSurface)
		{
		case SURFACETYPE_FleshDefault:
			// 击中普通身体部位
			ResultImpactParticleSystem = ImpactParticleSystem;
			break;
		case SURFACETYPE_FleshVulnerable:
			ResultImpactParticleSystem = ImpactParticleSystem;
			ResultDamge = ResultDamge * 4.0f;// 增加4倍伤害
			break;
		default:
			break;
		}
		

		UE_LOG(LogTemp,Warning,TEXT("Hit Damage: %f"),ResultDamge);

		// 受伤害
		// AActor* DamagedActor, float BaseDamage, FVector const& HitFromDirection, FHitResult const& HitInfo, AController* EventInstigator,
		// AActor* DamageCauser, TSubclassOf<UDamageType> DamageTypeClass
		UGameplayStatics::ApplyPointDamage(HitActor, ResultDamge,ViewLocation,OutHit,
			GetOwner()->GetInstigatorController(),GetOwner(), DamageType);

		// 受伤特效 TODO
		if (ImpactParticleSystem) {
			// const UObject* WorldContextObject, UParticleSystem* EmitterTemplate, FVector SpawnLocation, FRotator SpawnRotation, FVector SpawnScale, bool bAutoDestroy, EPSCPoolMethod PoolingMethod
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ResultImpactParticleSystem,OutHit.ImpactPoint);
		}
	}

	// 轨迹
	if (TranceParticleSystem) {
		FVector MuzzleLocation = MainCom->GetSocketLocation(MuzzleSocketName);
		UParticleSystemComponent * TrancePar = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TranceParticleSystem, MuzzleLocation);

		// 设置粒子系统的终点
		if (TrancePar) {
			TrancePar->SetVectorParameter(TranceEndBeanName,TranceEndPoint);
		}
	}
}

