// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/SLItemTooltip.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

// 동기식으로 이미지 설정
void USLItemTooltip::SetupDragVisualIcon(UTexture2D* LoadedTexture)
{
	if (Image_ItemIcon && LoadedTexture)
	{
		Image_ItemIcon->SetBrushFromTexture(LoadedTexture);
		Image_ItemIcon->SetVisibility(ESlateVisibility::Visible);
	}
}