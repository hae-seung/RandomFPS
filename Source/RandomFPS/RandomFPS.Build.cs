// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RandomFPS : ModuleRules
{
	public RandomFPS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"SlateCore",
			"NetCore",//FastArray
			
			"OnlineSubsystem",
			"OnlineSubsystemUtils",
			"OnlineSubsystemSteam",
			"Niagara",//나이아가라
			"PhysicsCore",//Physics Material
			"NavigationSystem", //Navigation
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"RandomFPS",
			"RandomFPS/Variant_Platforming",
			"RandomFPS/Variant_Platforming/Animation",
			"RandomFPS/Variant_Combat",
			"RandomFPS/Variant_Combat/AI",
			"RandomFPS/Variant_Combat/Animation",
			"RandomFPS/Variant_Combat/Gameplay",
			"RandomFPS/Variant_Combat/Interfaces",
			"RandomFPS/Variant_Combat/UI",
			"RandomFPS/Variant_SideScrolling",
			"RandomFPS/Variant_SideScrolling/AI",
			"RandomFPS/Variant_SideScrolling/Gameplay",
			"RandomFPS/Variant_SideScrolling/Interfaces",
			"RandomFPS/Variant_SideScrolling/UI",
			
			"RandomFPS/Public/",
			"RandomFPS/Private"
		});
		

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
