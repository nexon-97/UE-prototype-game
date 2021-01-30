// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class TestUEProjectEditorTarget : TargetRules
{
	public TestUEProjectEditorTarget(TargetInfo Target) : base(Target)
	{
		DefaultBuildSettings = BuildSettingsVersion.V2;
		bLegacyPublicIncludePaths = true;
		
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "TestUEProject" } );
	}
}
