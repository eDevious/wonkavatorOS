
# Wonkavator OS - VS2022 Project Generator
param([string]$Name = "WonkavatorOS")

Write-Host "Building VS2022 solution for $Name..."

# 1. Create Solution
$sln = @"
Microsoft Visual Studio Solution File, Format Version 12.00
# Visual Studio Version 17
VisualStudioVersion = 17.0.31903.59
MinimumVisualStudioVersion = 10.0.40219.1
Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "$Name", "$Name.vcxproj", "{B1A2C3D4-E5F6-4A5B-8C9D-0E1F2A3B4C5D}"
EndProject
Global
	GlobalSection(SolutionConfigurationPlatforms) = preSolution
		Debug|x64 = Debug|x64
		Release|x64 = Release|x64
	EndGlobalSection
	GlobalSection(ProjectConfigurationPlatforms) = postSolution
		{B1A2C3D4-E5F6-4A5B-8C9D-0E1F2A3B4C5D}.Debug|x64.ActiveCfg = Debug|x64
		{B1A2C3D4-E5F6-4A5B-8C9D-0E1F2A3B4C5D}.Debug|x64.Build.0 = Debug|x64
		{B1A2C3D4-E5F6-4A5B-8C9D-0E1F2A3B4C5D}.Release|x64.ActiveCfg = Release|x64
		{B1A2C3D4-E5F6-4A5B-8C9D-0E1F2A3B4C5D}.Release|x64.Build.0 = Release|x64
	EndGlobalSection
EndGlobal
"@
$sln | Out-File -Encoding ASCII "$Name.sln"

# 2. Create VCXProj
$vcxproj = @"
<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets="Build" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ItemGroup>
    <ClCompile Include="stage0\boot.c" />
    <ClCompile Include="stage0\cli.c" />
    <ClCompile Include="stage0\orchestrator.c" />
    <ClCompile Include="stage0\dma_driver.c" />
    <ClCompile Include="stage0\wonk_sync.c" />
    <ClCompile Include="stage0\ai_placer.c" />
  </ItemGroup>
  <ItemGroup>
    <ClInclude Include="stage0\boot.h" />
    <ClInclude Include="stage0\ai_placer.h" />
  </ItemGroup>
  <ItemGroup>
    <None Include="kernels\monolithic.c" />
    <None Include="kernels\hybrid.c" />
    <None Include="kernels\unikernel.c" />
    <None Include="link.ld" />
    <None Include="README.md" />
  </ItemGroup>
  <PropertyGroup>
    <ConfigurationType>Application</ConfigurationType>
    <PlatformToolset>v143</PlatformToolset>
    <LanguageStandard>stdcpp17</LanguageStandard>
    <AdditionalIncludeDirectories>stage0;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
  </PropertyGroup>
</Project>
"@
$vcxproj | Out-File -Encoding UTF8 "$Name.vcxproj"

# 3. Create Filters for nice folders
$filters = @"
<?xml version="1.0" encoding="utf-8"?>
<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ItemGroup>
    <Filter Include="stage0" />
    <Filter Include="kernels" />
    <Filter Include="sim" />
  </ItemGroup>
  <ItemGroup>
    <ClCompile Include="stage0\boot.c"><Filter>stage0</Filter></ClCompile>
    <ClCompile Include="stage0\cli.c"><Filter>stage0</Filter></ClCompile>
    <ClCompile Include="stage0\orchestrator.c"><Filter>stage0</Filter></ClCompile>
    <ClCompile Include="stage0\dma_driver.c"><Filter>stage0</Filter></ClCompile>
    <ClCompile Include="stage0\wonk_sync.c"><Filter>stage0</Filter></ClCompile>
    <ClCompile Include="stage0\ai_placer.c"><Filter>stage0</Filter></ClCompile>
    <ClInclude Include="stage0\boot.h"><Filter>stage0</Filter></ClInclude>
    <ClInclude Include="stage0\ai_placer.h"><Filter>stage0</Filter></ClInclude>
  </ItemGroup>
</Project>
"@
$filters | Out-File -Encoding UTF8 "$Name.vcxproj.filters"

Write-Host "Done! Opening VS2022..."
Start-Process "devenv.exe" "$Name.sln"