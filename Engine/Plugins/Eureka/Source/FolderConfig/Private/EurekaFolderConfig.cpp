#include "EurekaFolderConfig.h"
#include "Editor.h"
#include "Subsystems/ImportSubsystem.h"
#include "Engine/Texture.h"
#include "AssetRegistryModule.h"

// 资产设置宏
#define UPDATE_ASSET_SETTING(Property, NewValue) \
	if (Property != NewValue) { \
	Property = NewValue; \
	bIsDirty = true; \
	UE_LOG(LogTemp, Warning, TEXT("Eureka Config: <%s> Changed %s for %s."), *Asset->GetClass()->GetName() , *Asset->GetName()); \
	}

void UEurekaFolderConfig::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	// 监听导入
	if (GEditor)
	{
		if (UImportSubsystem* ImportSubsystem = GEditor->GetEditorSubsystem<UImportSubsystem>())
		{
			ImportSubsystem->OnAssetPostImport.AddUObject(this, &UEurekaFolderConfig::OnAssetPostImport);
		}
	}
	
	// 监听重命名/移动
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	AssetRegistryModule.Get().OnAssetRenamed().AddUObject(this, &UEurekaFolderConfig::OnAssetRenamed);
	
	UE_LOG(LogTemp, Log, TEXT("Eureka Auto Asset Config: Initialized and Listening..."))
}

void UEurekaFolderConfig::Deinitialize()
{
	
	// 清理绑定
	if (GEditor)
	{
		if (UImportSubsystem* ImportSubsystem = GEditor->GetEditorSubsystem<UImportSubsystem>())
		{
			ImportSubsystem->OnAssetPostImport.RemoveAll(this);
		}
	}
	
	Super::Deinitialize();
}

void UEurekaFolderConfig::ProcessAsset(UObject* Asset)
{
	if (!Asset) return;
	
	// --- 配置区域 ---
	FString TargetFolder = TEXT("/Game/TestFolderConfig");
	// ---------------
	
	// 检查路径
	FString AssetPath = Asset->GetPathName();
	if (AssetPath.IsEmpty() || TargetFolder.IsEmpty() || !AssetPath.StartsWith(TargetFolder))
	{
		UE_LOG(LogTemp, Warning, TEXT("Eureka Folder Config: Path loading failed."));
		return;
	}
	
	bool bIsDirty = false;
	
	// 贴图设置
	SetTextures(Asset, bIsDirty);
	// 模型设置
	
}

void UEurekaFolderConfig::OnAssetPostImport(UFactory* InFactory, UObject* InCreatedObject)
{
	ProcessAsset(InCreatedObject);
}

void UEurekaFolderConfig::OnAssetRenamed(const FAssetData& InAssetData, const FString& InOldObjectPath)
{
	// 当资产被移动后，InAssetData 指向的是“新位置”的资产
	// 我们需要获取这个资产的 UObject 指针
	UObject* Asset = InAssetData.GetAsset();

	// 如果资产没有加载（极其罕见，因为移动操作通常发生在内容浏览器，资产会被加载），则尝试加载它
	if (!Asset)
	{
		// 只有当路径符合我们的目标时才去加载，优化性能
		if (InAssetData.ObjectPath.ToString().StartsWith(TEXT("/Game/Art/UI/")))
		{
			Asset = InAssetData.GetAsset(); 
		}
	}
    
	// 调用统一处理逻辑
	ProcessAsset(Asset);
}

void UEurekaFolderConfig::SetTextures(UObject* Asset, bool& bIsDirty)
{
	if (UTexture2D* Texture = Cast<UTexture2D>(Asset))
	{
		// 贴图设置
		UPDATE_ASSET_SETTING(Texture->SRGB, true);
		UPDATE_ASSET_SETTING(Texture->PowerOfTwoMode, ETexturePowerOfTwoSetting::PadToPowerOfTwo);
		
		// 更新资源、Post Edit
		Texture->UpdateResource();
		Texture->MarkPackageDirty();
		Texture->PostEditChange();
	}
}
