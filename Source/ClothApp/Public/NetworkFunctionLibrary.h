// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "JsonObjectConverter.h"
#include "Engine/DataTable.h"
#include "NetworkFunctionLibrary.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FLoginReturnData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "LoginReturnData")
		FString accessToken;
	UPROPERTY(BlueprintReadWrite, Category = "LoginReturnData")
		FString refreshToken;
	UPROPERTY(BlueprintReadWrite, Category = "LoginReturnData")
		FString type;
	UPROPERTY(BlueprintReadWrite, Category = "LoginReturnData")
		FString issueDate;
	UPROPERTY(BlueprintReadWrite, Category = "LoginReturnData")
		FString accessTokenExpirationDate;
	UPROPERTY(BlueprintReadWrite, Category = "LoginReturnData")
		FString refreshTokenExpirationDate;
	UPROPERTY(BlueprintReadWrite, Category = "LoginReturnData")
		FString issueProvider;
	UPROPERTY(BlueprintReadWrite, Category = "LoginReturnData")
		FString website;
	UPROPERTY(BlueprintReadWrite, Category = "LoginReturnData")
		FString version;
};

USTRUCT(BlueprintType)
struct FUserInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "LoginReturnData")
		int64 id;
	UPROPERTY(BlueprintReadWrite, Category = "LoginReturnData")
		FString nickName;
	UPROPERTY(BlueprintReadWrite, Category = "LoginReturnData")
		FString avatar;
	UPROPERTY(BlueprintReadWrite, Category = "LoginReturnData")
		FString account;
	UPROPERTY(BlueprintReadWrite, Category = "LoginReturnData")
		FString telephone;
	UPROPERTY(BlueprintReadWrite, Category = "LoginReturnData")
		FString userStatus;
	UPROPERTY(BlueprintReadWrite, Category = "LoginReturnData")
		FString gender;
};

UENUM(BlueprintType)
enum class EUploadFileType:uint8
{
	avatar         UMETA(DisplayName = "avatar"),
	image           UMETA(DisplayName = "image"),
	model           UMETA(DisplayName = "model"),
	file           UMETA(DisplayName = "file"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FUploadImgFinish, bool, bWasSuccessful,FString, ResponseMessage,FString,ImgUrl);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSendValidCodeFinish, bool, bWasSuccessful, FString, ResponseMessage, int32, StateCode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FLoginValidCodeFinish, bool, bWasSuccessful, FString, ResponseMessage, int32, StateCode, FLoginReturnData,Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FGetUserInfoFinish, bool, bWasSuccessful, FString, ResponseMessage, int32, StateCode, FUserInfo, Data);

UCLASS()
class CLOTHAPP_API UNetworkFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable, Category = "AppNetwork")
		static UNetworkFunctionLibrary* GetInstancePtr();
	UFUNCTION(BlueprintCallable, Category = "AppNetwork")
		static void UploadImgFile(FString filePath);
	UFUNCTION(BlueprintCallable, Category = "AppNetwork")
		static void SendValidCode(FString phoneNum);
	UFUNCTION(BlueprintCallable, Category = "AppNetwork")
		static void LoginValidCode(FString phoneNum,FString validCode);
	//UFUNCTION(BlueprintCallable, Category = "AppNetwork")
	//	static void LoginPassword(FString phoneNum, FString password);
	UFUNCTION(BlueprintCallable, Category = "AppNetwork")
		static void GetUserInfo(FString token);
	//UFUNCTION(BlueprintCallable, Category = "AppNetwork")
	//	static void UpdateUserInfo(FString token);
private:
	static UNetworkFunctionLibrary* AppNetInstance;

public:
	UPROPERTY(BlueprintAssignable)
		FUploadImgFinish UploadImgFinishEvent;
	UPROPERTY(BlueprintAssignable)
		FSendValidCodeFinish FSendValidCodeFinishEvent;
	UPROPERTY(BlueprintAssignable)
		FLoginValidCodeFinish FLoginValidCodeFinishEvent;
	UPROPERTY(BlueprintAssignable)
		FGetUserInfoFinish FGetUserInfoFinishEvent;

};
