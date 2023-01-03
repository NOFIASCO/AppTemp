// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkFunctionLibrary.h"


UNetworkFunctionLibrary* UNetworkFunctionLibrary::AppNetInstance = nullptr;
UNetworkFunctionLibrary* UNetworkFunctionLibrary::GetInstancePtr()
{
	if (AppNetInstance == nullptr)
	{
		AppNetInstance = NewObject<UNetworkFunctionLibrary>();
		AppNetInstance->AddToRoot();
	}
	return AppNetInstance;
}

void UNetworkFunctionLibrary::UploadImgFile(FString filePath)
{
	//FString TextPath = FPaths::ProjectDir() + TEXT("DataDrive/Dialogue.txt");
	//TextPath = FPaths::ConvertRelativePathToFull(TextPath);
	//filePath = TEXT("C:/Users/Administrator/Desktop/111.png");

	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
// Request header
	FString Boundary = "---------------------------" + FString::FromInt(FDateTime::Now().GetTicks());
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("multipart/form-data; boundary =" + Boundary));
	HttpRequest->SetVerb(TEXT("POST"));

	//The bit array format of the request's content
	TArray<uint8> RequestContent;

	// The bit array format of a single file
	TArray<uint8> FileContent;
	if (FFileHelper::LoadFileToArray(FileContent, *filePath))
	{
		// Begin Boundry
		FString BeginBoundry = "\r\n--" + Boundary + "\r\n";
		RequestContent.Append((uint8*)TCHAR_TO_ANSI(*BeginBoundry), BeginBoundry.Len());

		// File Header
		FString FileHeader = "Content-Disposition: form-data;name=\"file\";";
		FileHeader.Append("filename=\"" + FPaths::GetCleanFilename(filePath) + "\"\r\n");
		FileHeader.Append("Content-Type: \r\n\r\n");
		RequestContent.Append((uint8*)TCHAR_TO_ANSI(*FileHeader), FileHeader.Len());

		// File Content
		RequestContent.Append(FileContent);
	}

	// End Boundry
	FString EndBoundary = "\r\n--" + Boundary + "--\r\n";
	RequestContent.Append((uint8*)TCHAR_TO_ANSI(*EndBoundary), EndBoundary.Len());

	HttpRequest->SetVerb("POST");

	FString Url = FString::Printf(TEXT("http://101.200.63.67:4000/api/files/upload/image") );

	HttpRequest->SetURL(Url);
	HttpRequest->SetContent(RequestContent);

	HttpRequest->OnProcessRequestComplete().BindLambda([=](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			if (!Request.IsValid() || !Request.IsValid()) {
				UE_LOG(LogTemp, Warning, TEXT("Http Unknow error"));
				return;
			}
			int32 StatusCode = Response->GetResponseCode();
			bool IsOK = EHttpResponseCodes::IsOk(StatusCode);
			FString ResponseString = Response->GetContentAsString();
			FString message;
			FString imgUrl;
			//Deserializing Json
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);
			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				message=JsonObject->GetStringField(TEXT("message"));
				imgUrl = JsonObject->GetObjectField(TEXT("data"))->GetStringField(TEXT("url"));
			}

			UNetworkFunctionLibrary::GetInstancePtr()->UploadImgFinishEvent.Broadcast(bWasSuccessful&& IsOK, message, imgUrl);
		});
	HttpRequest->ProcessRequest();
}

void UNetworkFunctionLibrary::SendValidCode(FString phoneNum) {
	FString serverData;
	TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create(&serverData);
	JsonWriter->WriteObjectStart();
	JsonWriter->WriteValue("telephone", phoneNum);
	JsonWriter->WriteObjectEnd();
	JsonWriter->Close();
	//http request
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	//Header
	HttpRequest->SetHeader("Content-Type", "application/json;charset=UTF-8");
	HttpRequest->SetVerb("POST");
	//url
	FString Url = FString::Printf(TEXT("http://101.200.63.67:4000/api/login/send-valid-code"));
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *Url);
	HttpRequest->SetURL(Url);
	//content
	HttpRequest->SetContentAsString(serverData);
	HttpRequest->OnProcessRequestComplete().BindLambda([=](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			if (!Request.IsValid() || !Request.IsValid()) {
				UE_LOG(LogTemp, Warning, TEXT("Http Unknow error"));
				return;
			}
			int32 StatusCode = Response->GetResponseCode();
			bool IsOK = EHttpResponseCodes::IsOk(StatusCode);
			
			FString ResponseString = Response->GetContentAsString();
			FString message;

			//Deserializing Json
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);
			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				message = JsonObject->GetStringField(TEXT("message"));
			}

			UNetworkFunctionLibrary::GetInstancePtr()->FSendValidCodeFinishEvent.Broadcast(bWasSuccessful&&IsOK,message, StatusCode);
		});
	HttpRequest->ProcessRequest();
}

void UNetworkFunctionLibrary::LoginValidCode(FString phoneNum, FString validCode) {
	FString serverData;
	TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create(&serverData);
	JsonWriter->WriteObjectStart();
	JsonWriter->WriteValue("telephone", phoneNum);
	JsonWriter->WriteValue("code", validCode);
	JsonWriter->WriteObjectEnd();
	JsonWriter->Close();
	//http request
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	//Header
	HttpRequest->SetHeader("Content-Type", "application/json;charset=UTF-8");
	HttpRequest->SetVerb("POST");
	//url
	FString Url = FString::Printf(TEXT("http://101.200.63.67:4000/api/login/phone-code"));
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *Url);
	HttpRequest->SetURL(Url);
	//content
	HttpRequest->SetContentAsString(serverData);
	HttpRequest->OnProcessRequestComplete().BindLambda([=](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			if (!Request.IsValid() || !Request.IsValid()) {
				UE_LOG(LogTemp, Warning, TEXT("Http Unknow error"));
				return;
			}
			int32 StatusCode = Response->GetResponseCode();
			bool IsOK = EHttpResponseCodes::IsOk(StatusCode);
			FString ResponseString = Response->GetContentAsString();

			FString message;
			TSharedPtr<FJsonObject> data;
			FLoginReturnData loginReturnData;

			//Deserializing Json
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);
			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				if (bWasSuccessful && IsOK) {
					data = JsonObject->GetObjectField(TEXT("data"));					
					loginReturnData.accessToken= data->GetStringField(TEXT("access_token"));
					loginReturnData.refreshToken = data->GetStringField(TEXT("refresh_token"));
					loginReturnData.type = data->GetStringField(TEXT("type"));
					loginReturnData.issueDate = data->GetStringField(TEXT("issue_date"));
					loginReturnData.accessTokenExpirationDate = data->GetStringField(TEXT("access_token_expiration_date"));
					loginReturnData.refreshTokenExpirationDate = data->GetStringField(TEXT("refresh_token_expiration_date"));
					loginReturnData.issueProvider = data->GetStringField(TEXT("issue_provider"));
					loginReturnData.website = data->GetStringField(TEXT("website"));
					loginReturnData.version = data->GetStringField(TEXT("version"));
					//FJsonObjectConverter::JsonObjectStringToUStruct(data, &loginReturnData);
				}
				
				message = JsonObject->GetStringField(TEXT("message"));
			}

			UNetworkFunctionLibrary::GetInstancePtr()->FLoginValidCodeFinishEvent.Broadcast(bWasSuccessful && IsOK, message, StatusCode, loginReturnData);
		});
	HttpRequest->ProcessRequest();
}

void UNetworkFunctionLibrary::GetUserInfo(FString token) {
	//http request
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	//Header
	FString tokenString = FString::Printf(TEXT("Bearer %s"),*token);
	HttpRequest->SetHeader("Content-Type", "application/json;charset=UTF-8");
	HttpRequest->SetHeader("Authorization", tokenString);
	HttpRequest->SetVerb("GET");
	//url
	FString Url = FString::Printf(TEXT("http://101.200.63.67:4000/api/users/me"));
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *Url);
	HttpRequest->SetURL(Url);
	//content
	//HttpRequest->SetContentAsString(serverData);
	HttpRequest->OnProcessRequestComplete().BindLambda([=](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			if (!Request.IsValid() || !Request.IsValid()) {
				UE_LOG(LogTemp, Warning, TEXT("Http Unknow error"));
				return;
			}
			int32 StatusCode = Response->GetResponseCode();
			bool IsOK = EHttpResponseCodes::IsOk(StatusCode);

			FString ResponseString = Response->GetContentAsString();
			FString message;
			TSharedPtr<FJsonObject> data;
			//suck server data struct
			TSharedPtr<FJsonObject> baseInfo;
			FUserInfo userInfo;
			//Deserializing Json
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);
			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				if (bWasSuccessful && IsOK) {
					data = JsonObject->GetObjectField(TEXT("data"));
					baseInfo=data->GetObjectField(TEXT("baseInfo"));
					userInfo.id=data->GetIntegerField(TEXT("id"));
					userInfo.nickName= data->GetStringField(TEXT("nickname"));
					userInfo.account= data->GetStringField(TEXT("account"));
					userInfo.telephone= data->GetStringField(TEXT("telephone"));
					userInfo.userStatus= data->GetStringField(TEXT("status"));
					userInfo.gender= baseInfo->GetStringField(TEXT("gender"));

					//FJsonObjectConverter::JsonObjectStringToUStruct(data, &loginReturnData);
				}

				message = JsonObject->GetStringField(TEXT("message"));
			}

			UNetworkFunctionLibrary::GetInstancePtr()->FGetUserInfoFinishEvent.Broadcast(bWasSuccessful && IsOK, message, StatusCode, userInfo);
		});
	HttpRequest->ProcessRequest();
}
