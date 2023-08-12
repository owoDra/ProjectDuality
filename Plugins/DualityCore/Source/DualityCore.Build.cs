// Copyright (C) 2023 owoDra

using UnrealBuildTool;

public class DualityCore : ModuleRules
{
	public DualityCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] 
			{
			}
		);
				
		
		PrivateIncludePaths.AddRange(
			new string[] 
			{
			}
		);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "CoreUObject",
                "ApplicationCore",
                "Engine",
                "GameplayTags",
                "GameplayTasks",
                "GameplayAbilities",
            }
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "InputCore",
                "EnhancedInput",
                "Slate",
				"SlateCore",
                "UMG",
                "AudioModulation",
                "AudioMixer",
                "EngineSettings",
                "DeveloperSettings",
            }
		);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
		);

	}
}
