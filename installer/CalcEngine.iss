#ifndef MyAppVersion
  #define MyAppVersion "1.0.0"
#endif

#ifndef SourceDir
  #define SourceDir "."
#endif

[Setup]
AppId={{8F55E438-6170-45C2-A2D4-B02BEA4D9629}
AppName=CalcEngine
AppVersion={#MyAppVersion}
AppPublisher=Mubashir
DefaultDirName={autopf}\CalcEngine
DefaultGroupName=CalcEngine
DisableProgramGroupPage=yes
OutputBaseFilename=CalcEngine-Setup
Compression=lzma
SolidCompression=yes
WizardStyle=modern
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "{#SourceDir}\CalcEngine.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SourceDir}\*.dll"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{autoprograms}\CalcEngine"; Filename: "{app}\CalcEngine.exe"
Name: "{autodesktop}\CalcEngine"; Filename: "{app}\CalcEngine.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\CalcEngine.exe"; Description: "{cm:LaunchProgram,CalcEngine}"; Flags: nowait postinstall skipifsilent
