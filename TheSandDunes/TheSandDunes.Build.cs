// Copyright Epic Games, Inc. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class TheSandDunes : ModuleRules
{
	public TheSandDunes(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "GameplayTasks", "PhysicsCore" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "MoviePlayer" });

		DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");

		AddDirectoriesRecursive(ModuleDirectory);
	}

	private void AddDirectoriesRecursive(string DirectoryPathToSearch)
	{
		foreach (string DirectoryPath in Directory.GetDirectories(DirectoryPathToSearch))
		{
			//System.Console.WriteLine("Agregando el directorio " + DirectoryPath);
			PrivateIncludePaths.Add(DirectoryPath);
			AddDirectoriesRecursive(DirectoryPath); 
		}
    }

}
