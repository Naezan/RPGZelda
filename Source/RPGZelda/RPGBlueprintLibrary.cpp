// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGBlueprintLibrary.h"
#include "RPGLoadingScreen/RPGLoadingScreen.h"

URPGBlueprintLibrary::URPGBlueprintLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void URPGBlueprintLibrary::PlayLoadingScreen(bool bPlayUntilStopped, float PlayTime)
{
	IRPGLoadingScreenModule& LoadingScreenModule = IRPGLoadingScreenModule::Get();
	LoadingScreenModule.StartInGameLoadingScreen(bPlayUntilStopped, PlayTime);
}

void URPGBlueprintLibrary::StopLoadingScreen()
{
	IRPGLoadingScreenModule& LoadingScreenModule = IRPGLoadingScreenModule::Get();
	LoadingScreenModule.StopInGameLoadingScreen();
}
