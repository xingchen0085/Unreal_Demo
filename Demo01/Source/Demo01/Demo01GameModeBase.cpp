// Fill out your copyright notice in the Description page of Project Settings.

#include "Demo01GameModeBase.h"
#include "Engine/Engine.h"

void ADemo01GameModeBase::StartPlay() {
	
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Yellow,TEXT("ADemo01GameModeBase.StartPlay"));
	}
}


