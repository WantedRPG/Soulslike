// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/SLItemInfo.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Styling/SlateBrush.h"

void USLItemInfo::OnIconLoaded(FSoftObjectPath SoftIconPath)
{
	// 로드가 완료된 후 텍스처 적용
	UTexture2D* LoadedTexture = Cast<UTexture2D>(SoftIconPath.ResolveObject());

	if (LoadedTexture && Image_ItemIcon)
	{
		Image_ItemIcon->SetVisibility(ESlateVisibility::Visible);
		Image_ItemIcon->SetBrushFromTexture(LoadedTexture);
	}
}

void USLItemInfo::SetItemName(FText InItemName)
{
	Txt_ItemName->SetText(InItemName);
}

void USLItemInfo::SetItemIconAsync(TSoftObjectPtr<UTexture2D> SoftIcon)
{
	//Todo : 이미지 로드 관련해서 조사하기, 위젯 닫을때 메모리해제 안되는거같음 -> 이미지 겹침 문제
	UTexture2D* LoadedTexture = SoftIcon.Get();
	//메모리에 로드되어 있는지 확인
	//Slot이 가리는거같음
	if (LoadedTexture && Image_ItemIcon)
	{
		Image_ItemIcon->SetBrushFromTexture(LoadedTexture);
		Image_ItemIcon->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		//로드되어 있지 않다면, 비동기 로드 요청
		FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
		StreamableManager.RequestAsyncLoad(
			SoftIcon.ToSoftObjectPath(),
			FStreamableDelegate::CreateUObject(this, &USLItemInfo::OnIconLoaded, SoftIcon.ToSoftObjectPath())
		);
	}
}