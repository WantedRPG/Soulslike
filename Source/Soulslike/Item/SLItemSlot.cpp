// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/SLItemSlot.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SLItemData.h"

#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"


USLItemSlot::USLItemSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}
//Todo : 보유개수 때문에 USLItemData를 쓰는건 아닌거 같다
void USLItemSlot::SetItemData(USLItemData* NewItemData,int32 InStackCount)
{
	check(NewItemData);
	
	ItemID = NewItemData->ItemID;
	SetItemIconAsync(NewItemData->ItemIcon);
	Txt_StackCount->SetVisibility(ESlateVisibility::Visible);
	Txt_StackCount->SetText(FText::FromString(FString::FromInt(InStackCount)));
}

void USLItemSlot::SetItemIconAsync(TSoftObjectPtr<UTexture2D> SoftIcon)
{
	UTexture2D* LoadedTexture = SoftIcon.Get();
	//메모리에 로드되어 있는지 확인
	if (LoadedTexture)
	{
		if (Image_ItemIcon)
		{
			Image_ItemIcon->SetBrushFromTexture(LoadedTexture);
			Image_ItemIcon->SetVisibility(ESlateVisibility::Visible);
		}
		return;
	}
	
	if (SoftIcon.IsNull())
	{
		//아이콘이 비어있는 경우
		if (Image_ItemIcon)
		{
			Image_ItemIcon->SetBrushFromTexture(nullptr);
			Image_ItemIcon->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else 
	{
		//로드되어 있지 않다면, 비동기 로드 요청
		FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
		StreamableManager.RequestAsyncLoad(
			SoftIcon.ToSoftObjectPath(),
			FStreamableDelegate::CreateUObject(this, &USLItemSlot::OnIconLoaded, SoftIcon)
		);
	}
}

void USLItemSlot::OnIconLoaded(TSoftObjectPtr<UTexture2D> LoadedIcon)
{
	// 로드가 완료된 후 텍스처 적용
	UTexture2D* LoadedTexture = LoadedIcon.Get();

	if (LoadedTexture && Image_ItemIcon)
	{
		Image_ItemIcon->SetBrushFromTexture(LoadedTexture);
		Image_ItemIcon->SetVisibility(ESlateVisibility::Visible);
	}
}

void USLItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	Image_ItemIcon->SetVisibility(ESlateVisibility::Hidden);
	Txt_StackCount->SetVisibility(ESlateVisibility::Hidden);
}
