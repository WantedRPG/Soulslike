// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/SLItemSlot.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SLItemData.h"

#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Styling/SlateBrush.h"

USLItemSlot::USLItemSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}
//Todo : 보유개수 때문에 USLItemData를 쓰는건 아닌거 같다
void USLItemSlot::SetItemData(USLItemData* NewItemData,int32 InStackCount)
{
	check(NewItemData);
	
	ItemID = NewItemData->ItemID;
	//Todo : 이미지 누락
	FGameplayTag StackableTag = FGameplayTag::RequestGameplayTag(FName("Item.Property.Stackable"));
	//같은 종류 아이템 누적 가능여부 tag로 구별 
	if (NewItemData->ItemTags.HasTag(StackableTag))
	{
		Txt_StackCount->SetVisibility(ESlateVisibility::Visible);
		Txt_StackCount->SetText(FText::AsNumber(InStackCount));
	}

	SetItemIconAsync(NewItemData->ItemIcon);
}

void USLItemSlot::SetEmpty()
{
	Image_ItemIcon->SetVisibility(ESlateVisibility::Hidden);
	Txt_StackCount->SetVisibility(ESlateVisibility::Hidden);
}

void USLItemSlot::UpdateStackCount(int32 InStackCount)
{
	Txt_StackCount->SetText(FText::AsNumber(InStackCount));
}

void USLItemSlot::SetItemIconAsync(TSoftObjectPtr<UTexture2D> SoftIcon)
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
			FStreamableDelegate::CreateUObject(this, &USLItemSlot::OnIconLoaded, SoftIcon.ToSoftObjectPath())
		);
	}
}

void USLItemSlot::OnIconLoaded(FSoftObjectPath SoftIconPath)
{
	// 로드가 완료된 후 텍스처 적용
	UTexture2D* LoadedTexture = Cast<UTexture2D>(SoftIconPath.ResolveObject());

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
