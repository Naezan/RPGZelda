// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

// This module must be loaded "PreLoadingScreen" in the .uproject file, otherwise it will not hook in time!

public class RPGLoadingScreen : ModuleRules
{
	public RPGLoadingScreen(ReadOnlyTargetRules Target)
		: base(Target)
	{
		PrivatePCHHeaderFile = "RPGLoadingScreen.h";

		PCHUsage = PCHUsageMode.UseSharedPCHs;

		PrivateIncludePaths.Add("RPGLoadingScreen");

		PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"MoviePlayer",
				"Slate",
				"SlateCore",
				"InputCore"
			}
		);
	}
}
