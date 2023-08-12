// Copyright (C) 2023 owoDra

using UnrealBuildTool;

public class ProjectDuality : ModuleRules
{
	public ProjectDuality(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(
			new string[] 
			{ 
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"DualityCore"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[] 
			{
			}
		);

	}
}
