// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ArkhamCombPrototype : ModuleRules
{
	public ArkhamCombPrototype(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"GameplayTags"
		});
	}
}
