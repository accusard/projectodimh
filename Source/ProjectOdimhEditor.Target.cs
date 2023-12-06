// Copyright 2017-2018 Vanny Sou. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectOdimhEditorTarget : TargetRules
{
	public ProjectOdimhEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
		ExtraModuleNames.AddRange( new string[] { "ProjectOdimh" } );
	}
}
