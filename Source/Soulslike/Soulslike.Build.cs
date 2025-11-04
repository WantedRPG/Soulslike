// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Soulslike : ModuleRules
{
	public Soulslike(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks",
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

        PublicIncludePaths.AddRange(new string[] { ModuleDirectory });
    }
}
