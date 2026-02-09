// Copyright Epic Games, Inc. All Rights Reserved.

#include "EurekaBlueprintHelperBPLibrary.h"
#include "EurekaBlueprintHelper.h"

UEurekaBlueprintHelperBPLibrary::UEurekaBlueprintHelperBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

#if WITH_EDITOR
void UEurekaBlueprintHelperBPLibrary::SetTextureMaxSize(UTexture2D* Texture, int32 NewMaxSize, bool bMarkDirty)
{
	// 安全检查，确保指针有效
	if (!Texture)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetTextureMaxSize: Texture is Null"));
		return;
	}
	// 设置最大尺寸。MaxTextureSize 如果设为0，意味着不限制（引用原图尺寸）
	Texture->MaxTextureSize = NewMaxSize;
	// 通知编辑器属性已变更，这会触发贴图的重新构建（Re-build），更新Mipmaps等
	Texture->PostEditChange();
	// 标记包为脏数据 (Dirty)，这样编辑器会提示你需要保存该资产
	if (bMarkDirty)
		Texture->MarkPackageDirty();

	UE_LOG(LogTemp, Warning, TEXT("Set Max Texture Size for %s to %d"), *Texture->GetName() , NewMaxSize);
}

void UEurekaBlueprintHelperBPLibrary::SetMaterialInstanceBoolByName(UMaterialInstanceConstant* Instance,
	FName PropertyName, bool Value)
{
	if (!Instance) return;;
	
	UScriptStruct* StructParams = FMaterialInstanceBasePropertyOverrides::StaticStruct(); // 获取 BasePropertyOverrides 的结构体类型信息
	void* StructData = &(Instance->BasePropertyOverrides); // 获取实际数据的内存地址
	
	FBoolProperty* TargetBoolProperty = CastField<FBoolProperty>(StructParams->FindPropertyByName(PropertyName)); // 尝试查找目标属性
	if (TargetBoolProperty)
	{
		TargetBoolProperty->SetPropertyValue_InContainer(StructData, Value); // 设置属性值
		FString OverridePropName = "bOverride_" + PropertyName.ToString(); // 处理 bOverride_xxx
		FBoolProperty* OverrideBoolProperty = CastField<FBoolProperty>(StructParams->FindPropertyByName(*OverridePropName));
		
		if (OverrideBoolProperty)
		{
			OverrideBoolProperty->SetPropertyValue_InContainer(StructData, true); // 勾选前面的小开关
		}
		
		FMaterialInstanceBasePropertyOverrides& Overrides = Instance->BasePropertyOverrides;
		FStaticParameterSet StaticParams;
		Instance->GetStaticParameterValues(StaticParams);
		Instance->UpdateStaticPermutation(StaticParams, Overrides);
		
		Instance->PostEditChange();
		Instance->MarkPackageDirty();
		
		
		UE_LOG(LogTemp, Log, TEXT("Successfully set %s to %d"), *PropertyName.ToString(), Value);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not find property named: %s"), *PropertyName.ToString());
	}
}

void UEurekaBlueprintHelperBPLibrary::SetMaterialInstanceIntByName(UMaterialInstanceConstant* Instance,
	FName PropertyName, int32 Value)
{
	if (!Instance) return;;
	
	UScriptStruct* StructParams = FMaterialInstanceBasePropertyOverrides::StaticStruct(); // 获取 BasePropertyOverrides 的结构体类型信息
	void* StructData = &(Instance->BasePropertyOverrides); // 获取实际数据的内存地址
	
	FIntProperty* TargetIntProperty = CastField<FIntProperty>(StructParams->FindPropertyByName(PropertyName)); // 尝试查找目标属性
	if (TargetIntProperty)
	{
		TargetIntProperty->SetPropertyValue_InContainer(StructData, Value); // 设置属性值
		FString OverridePropName = "bOverride_" + PropertyName.ToString(); // 处理 bOverride_xxx
		FBoolProperty* OverrideBoolProperty = CastField<FBoolProperty>(StructParams->FindPropertyByName(*OverridePropName));
		
		if (OverrideBoolProperty)
		{
			OverrideBoolProperty->SetPropertyValue_InContainer(StructData, true); // 勾选前面的小开关
		}
		
		FMaterialInstanceBasePropertyOverrides& Overrides = Instance->BasePropertyOverrides;
		FStaticParameterSet StaticParams;
		Instance->GetStaticParameterValues(StaticParams);
		Instance->UpdateStaticPermutation(StaticParams, Overrides);
		
		Instance->PostEditChange();
		Instance->MarkPackageDirty();
		
		
		UE_LOG(LogTemp, Log, TEXT("Successfully set %s to %d"), *PropertyName.ToString(), Value);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not find property named: %s"), *PropertyName.ToString());
	}
}

#endif
