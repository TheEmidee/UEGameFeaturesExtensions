// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GameFeaturesExtensions : ModuleRules
{
	public GameFeaturesExtensions(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
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
				"Core"
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"GameFeatures"
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{}
			);
	}
}
