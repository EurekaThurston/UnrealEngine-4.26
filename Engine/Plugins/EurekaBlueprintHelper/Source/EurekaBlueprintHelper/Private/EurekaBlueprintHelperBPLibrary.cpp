// Copyright Epic Games, Inc. All Rights Reserved.

#include "EurekaBlueprintHelperBPLibrary.h"
#include "EurekaBlueprintHelper.h"

UEurekaBlueprintHelperBPLibrary::UEurekaBlueprintHelperBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}


void UEurekaBlueprintHelperBPLibrary::SetTextureMaxSize(UTexture2D* Texture, int32 NewMaxSize, bool bMarkDirty)
{
	// 安全检查，确保指针有效
	if (!Texture)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetTextureMaxSize: Texture is Null"));
		return;
	}

#if WITH_EDITOR
	// 设置最大尺寸。MaxTextureSize 如果设为0，意味着不限制（引用原图尺寸）
	Texture->MaxTextureSize = NewMaxSize;
	// 通知编辑器属性已变更，这会触发贴图的重新构建（Re-build），更新Mipmaps等
	Texture->PostEditChange();
	// 标记包为脏数据 (Dirty)，这样编辑器会提示你需要保存该资产
	if (bMarkDirty)
		Texture->MarkPackageDirty();

	UE_LOG(LogTemp, Warning, TEXT("Set Max Texture Size for %s to %d"), *Texture->GetName() , NewMaxSize);
	
#endif
}
