// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FPInteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFPInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TURRETMASTER_API IFPInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// BlueprintNativeEvent: makes this usable in either C++ or BP
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	void Interact(); 
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	bool CanInteract(); 
	
};
