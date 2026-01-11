// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PrettierMaterialEditor : ModuleRules
{
	public PrettierMaterialEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UnrealEd",      // 核心编辑器功能
				"EditorStyle",   // 编辑器样式
				"PrettierMaterial", // 依赖你的Runtime模块！
			}
		);
	}
}