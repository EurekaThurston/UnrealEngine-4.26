// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "EurekaFolderConfig.generated.h"

/**
 * 
 */
UCLASS()
class FOLDERCONFIG_API UEurekaFolderConfig : public UEditorSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
private:
	// 核心处理函数
	void ProcessAsset(UObject* Asset);
	// 事件回调: 导入
	void OnAssetPostImport(UFactory* InFactory, UObject* InCreatedObject);
	// 事件回调: 重命名/移动
	void OnAssetRenamed(const FAssetData& InAssetData, const FString& InOldObjectPath);
	// 贴图
	void SetTextures(UObject* Asset, bool& bIsDirty);
	
};
