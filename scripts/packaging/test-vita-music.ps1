param(
    [string]$SdlRoot = '<test dir>\sdl3-x86',
    [string]$Track = '<test dir>\game\MUSIC\Track04.ogg',
    [string]$OutputDirectory = '<local notes>\vita_music_test'
)
$ErrorActionPreference = 'Stop'
$repo = (Resolve-Path "$PSScriptRoot\..\..").Path
New-Item -ItemType Directory -Force -Path $OutputDirectory | Out-Null
$out = (Resolve-Path $OutputDirectory).Path
$commands = @"
@echo off
set "PATH=C:\Program Files (x86)\Microsoft Visual Studio\Installer;%PATH%"
call "<test dir>\env.bat"
cd /d "$out"
cl /nologo /O2 /MT /std:c11 /D_CRT_SECURE_NO_WARNINGS /I"$SdlRoot\include" "$repo\scripts\packaging\tests\vita_music_main.c" /Fe"$out\vita_music_test.exe" /link "$SdlRoot\lib\SDL3.lib"
exit /b %errorlevel%
"@
Set-Content -LiteralPath "$out\build.bat" -Value $commands -Encoding ASCII
$ErrorActionPreference = 'Continue'
& cmd /d /c "$out\build.bat" 2>&1 | ForEach-Object { Write-Output $_ }
$buildResult = $LASTEXITCODE
$ErrorActionPreference = 'Stop'
if ($buildResult -ne 0) { throw 'Music fixture compilation failed' }
Copy-Item -LiteralPath "$SdlRoot\bin\SDL3.dll" -Destination "$out\SDL3.dll"
& "$out\vita_music_test.exe" $Track
if ($LASTEXITCODE -ne 0) { throw 'Music fixture failed' }
