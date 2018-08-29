// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Demo01GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DEMO01_API ADemo01GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
		void StartPlay() override;
	
	
};
