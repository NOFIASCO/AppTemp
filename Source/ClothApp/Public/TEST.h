// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

#include "TEST.generated.h"



/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTestDemoReceive, FString, ReceiveRawData);

UCLASS()
class CLOTHAPP_API UTEST : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
		//UFUNCTION(BlueprintCallable, Category = "AppNetwork")
		static UTEST* GetInstancePtr();
		//UFUNCTION(BlueprintCallable, Category = "AppNetwork")
		static void ApiTestDemo(FString userName, FString nickName, FString stateCode);

public:
	UPROPERTY(BlueprintAssignable)
		FTestDemoReceive TestDemoReceiveEvent;

private:
	static UTEST* TestInstance;
	
};
