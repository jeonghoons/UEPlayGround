// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Test : ModuleRules
{
	public Test(ReadOnlyTargetRules Target) : base(Target)
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
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"Test",
			"Test/Variant_Platforming",
			"Test/Variant_Platforming/Animation",
			"Test/Variant_Combat",
			"Test/Variant_Combat/AI",
			"Test/Variant_Combat/Animation",
			"Test/Variant_Combat/Gameplay",
			"Test/Variant_Combat/Interfaces",
			"Test/Variant_Combat/UI",
			"Test/Variant_SideScrolling",
			"Test/Variant_SideScrolling/AI",
			"Test/Variant_SideScrolling/Gameplay",
			"Test/Variant_SideScrolling/Interfaces",
			"Test/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
