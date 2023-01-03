// Fill out your copyright notice in the Description page of Project Settings.


#include "TEST.h"

UTEST* UTEST::TestInstance = nullptr;

//UTEST::UTEST(const FObjectInitializer& ObjectInitializer)
//	: Super(ObjectInitializer)
//{
//
//}
UTEST* UTEST::GetInstancePtr() {
	if (TestInstance == nullptr)
	{
		TestInstance = NewObject<UTEST>();
		TestInstance->AddToRoot();
	}
	return TestInstance;
}
void UTEST::ApiTestDemo(FString userName, FString nickName, FString stateCode) {
	/**json ����**/
	FString serverData;
	FString tags[] = { TEXT("test1"), TEXT("test2") };
	TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create(&serverData);
	JsonWriter->WriteObjectStart();
	JsonWriter->WriteValue("name", userName);
	JsonWriter->WriteValue("id", 0);
	JsonWriter->WriteValue("age", 0);
	JsonWriter->WriteValue("isvip", true);
	JsonWriter->WriteObjectEnd();

	//JsonWriter->WriteArrayStart("tags");
	//JsonWriter->WriteValue("tags1");
	//JsonWriter->WriteValue("tags2");
	//JsonWriter->WriteArrayEnd();

	JsonWriter->Close();

	//http request
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();

	//Header
	HttpRequest->SetHeader("Content-Type", "application/json;charset=UTF-8");

	HttpRequest->SetVerb("POST");
	//url
	FString Url= FString::Printf(TEXT("http://101.200.63.67:4000/api/demo/%s?nickname=%s&status=%s"), *userName, *nickName, *stateCode);
	//FString testUrl = "http://101.200.63.67:4000/api/demo/1?nickname=2&�����÷��ص�HTTP״̬��=3";
	UE_LOG(LogTemp, Warning, TEXT("%s"),*Url);
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

			FString ResponseString = Response->GetContentAsString();
			GetInstancePtr()->TestDemoReceiveEvent.Broadcast(ResponseString);
			//callback(true, TCHAR_TO_UTF8(*ResponseString));
		});
	HttpRequest->ProcessRequest();


}

