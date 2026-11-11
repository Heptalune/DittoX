#define MyAppName               "DittoX"
#define MyAppVersion            GetFileVersion("..\Release64\DittoX.exe")
#define MyAppVerName            MyAppName + " " + MyAppVersion
#define MyAppPublisher          "DittoX (based on Ditto by Scott Brogden)"
#define MyAppSupportURL         "https://github.com/Heptalune/DittoX"
#define MyAppCopyrighEndYear    GetDateTimeString('yyyy','','')
#define MyOutputBaseFilename    "DittoXSetup_" + GetEnv('VERSION_FILENAME')

[Setup]
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppVerName}

AppPublisher={# MyAppPublisher}

AppPublisherURL={#MyAppSupportURL}
AppSupportURL={#MyAppSupportURL}
AppUpdatesURL={#MyAppSupportURL}

WizardStyle=modern

UninstallDisplayIcon={app}\DittoX.exe
UninstallDisplayName={#MyAppName}

VersionInfoDescription={#MyAppName} installer
VersionInfoVersion={#MyAppVersion}
VersionInfoProductName={#MyAppName}
VersionInfoProductVersion={#MyAppVersion}

AppCopyright={#MyAppPublisher} {#MyAppCopyrighEndYear}

OutputBaseFilename={#MyOutputBaseFilename}
ArchitecturesInstallIn64BitMode=x64compatible
ArchitecturesAllowed=x64compatible
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
;UsePreviousTasks=no
;DisableDirPage=yes
DisableProgramGroupPage=yes
DisableReadyPage=yes
DirExistsWarning=no
UninstallLogMode=overwrite
ChangesAssociations=yes
CloseApplications=yes
MinVersion=6.1
SetupLogging=yes

[Languages]
Name: English; MessagesFile: compiler:Default.isl
Name: ChineseSimplified; MessagesFile: ChineseSimplified.isl
Name: ChineseTraditional; MessagesFile: ChineseTraditional.isl

[CustomMessages]
English.RunDittoXOnStartup=Run DittoX on Windows startup
English.AddFirewallException=Add Windows Firewall exception for DittoX on port 23443
English.LaunchDittoX=Launch DittoX
English.ViewHelp=View Help
English.ViewChangeHistory=View Change History
English.UninstallDittoX=Uninstall DittoX
English.VCRuntimeInstallFailed=VCRuntime prerequisite install failed.

[Tasks]
Name: RunAtStartup; Description: {cm:RunDittoXOnStartup}
Name: AddFireWallException; Description: {cm:AddFirewallException};  Flags: unchecked

[Files]
Source: ..\Release64\DittoX.exe; DestDir: {app}; DestName: DittoX.exe; Flags: ignoreversion; AfterInstall: AddProgramToFirewall(ExpandConstant('{app}\DittoX.exe'), 'DittoX_FromInstaller_64');
Source: ..\Release64\ICU_Loader.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\Release64\Addins\DittoXUtil.dll; DestDir: {app}\Addins; Flags: ignoreversion

; System32 stores a 64-bit DLL on x64 system
; (using System32 directly - the 64-bit ISCC compiler cannot access the sysnative alias)
Source: C:\Windows\System32\vcruntime140.dll;  DestDir: {app}; Flags: ignoreversion
Source: C:\Windows\System32\vcruntime140_1.dll;  DestDir: {app}; Flags: ignoreversion
Source: C:\Windows\System32\msvcp140.dll;  DestDir: {app}; Flags: ignoreversion
Source: C:\Windows\System32\mfc140u.dll;  DestDir: {app}; Flags: ignoreversion


Source: ..\Debug\Language\*; DestDir: {app}\Language; BeforeInstall: BeforeLanguageInstall()
Source: ..\Debug\Themes\*; DestDir: {app}\Themes

[Icons]
Name: {group}\DittoX; Filename: {app}\DittoX.exe
Name: {group}\{cm:UninstallDittoX}; Filename: {uninstallexe}

[Run]
Filename: {app}\DittoX.exe; Description: {cm:LaunchDittoX}; Flags: nowait postinstall
Filename: https://github.com/Heptalune/DittoX; Description: {cm:ViewHelp}; Flags: nowait postinstall skipifsilent shellexec unchecked
Filename: https://github.com/Heptalune/DittoX/releases; Description: {cm:ViewChangeHistory}; Flags: nowait postinstall skipifsilent shellexec unchecked

[Registry]
; keep "Software\Ditto" - the app reads/writes its settings there (hardcoded)
Root: HKCU; Subkey: Software\Ditto; Flags: uninsdeletekey
Root: HKCU; Subkey: SOFTWARE\Microsoft\Windows\CurrentVersion\Run; ValueType: string; ValueName: DittoX; flags: uninsdeletevalue; ValueData: {app}\DittoX.exe; Tasks: RunAtStartup

Root: HKCU; Subkey: Software\Ditto; ValueType: dword; ValueName: SetFocus_iexplore.exe; ValueData: 00000001

Root: HKCU; Subkey: Software\Ditto; ValueType: string; ValueName: LanguageFile; ValueData: "{code:AdjustedLanguage|{language}}"

Root: HKCU; Subkey: Software\Ditto\PasteStrings; ValueType: string; ValueName: gvim.exe; ValueData: """{{PLUS}gP"
Root: HKCU; Subkey: Software\Ditto\CopyStrings; ValueType: string; ValueName: gvim.exe; ValueData: """{{PLUS}y"
Root: HKCU; Subkey: Software\Ditto\CutStrings; ValueType: string; ValueName: gvim.exe; ValueData: """{{PLUS}x"

Root: HKCU; Subkey: Software\Ditto\PasteStrings; ValueType: string; ValueName: cmd.exe; OnlyBelowVersion: 10; ValueData: % {{Delay100}ep
Root: HKCU; Subkey: Software\Ditto\CopyStrings; ValueType: string; ValueName: cmd.exe; OnlyBelowVersion: 10; ValueData: % {{Delay100}ey

Root: HKCU; Subkey: Software\Ditto\PasteStrings; ValueName: cmd.exe; MinVersion: 10; Flags: deletevalue
Root: HKCU; Subkey: Software\Ditto\CopyStrings; ValueName: cmd.exe; MinVersion: 10; Flags: deletevalue

;associate .dto with DittoX
Root: HKCR; Subkey: .dto; ValueType: string; ValueName: ; ValueData: DittoX; Flags: uninsdeletevalue
Root: HKCR; Subkey: DittoX; ValueType: string; ValueName: ; ValueData: DittoX; Flags: uninsdeletekey
Root: HKCR; Subkey: DittoX\DefaultIcon; ValueType: string; ValueName: ; ValueData: {app}\DittoX.exe,0
Root: HKCR; Subkey: DittoX\shell\open\command; ValueType: string; ValueName: ; ValueData: """{app}\DittoX.exe"" ""%1"""


[Code]
procedure BeforeLanguageInstall();
var
  sDir: String;
begin
end;

function AdjustedLanguage(Param: String): String;
begin
  Case Param of
    'ChineseSimplified' : Result := 'Chinese Simplified';
    'ChineseTraditional' : Result := 'Chinese Traditional';
  else
    Result := Param;
  end;
end;

procedure CleanupOldFiles();
var
  sDir: String;
  begin
    sDir := ExpandConstant('{app}');

    DeleteFile(sDir+'\mfc100u.dll')
    DeleteFile(sDir+'\mfcm100u.dll')
    DeleteFile(sDir+'\msvcp100.dll')
    DeleteFile(sDir+'\msvcr100.dll')

    DeleteFile(sDir+'\iculx55.dll')
    DeleteFile(sDir+'\icule55.dll')
    DeleteFile(sDir+'\icuuc55.dll')
    DeleteFile(sDir+'\icutu55.dll')
    DeleteFile(sDir+'\icuio55.dll')
    DeleteFile(sDir+'\icuin55.dll')
    DeleteFile(sDir+'\icudt55.dll')


    //moved to use the windows included dll
    DeleteFile(sDir+'\icuuc58.dll')
    DeleteFile(sDir+'\icuin58.dll')
    DeleteFile(sDir+'\icutu58.dll')
    DeleteFile(sDir+'\icuio58.dll')
    DeleteFile(sDir+'\icudt58.dll')

    DelTree(sDir+'\Help', TRUE, TRUE, TRUE)
end;


procedure RegisterForCrashDump(theApp : String);
var
   theExe : String;
begin
  theExe := theApp + '.exe';
  if IsWin64() then
    begin
      if RegValueExists(HKLM64, 'Software\Microsoft\Windows\Windows Error Reporting\LocalDumps\' + theExe, 'DumpFolder') = false then
        RegWriteStringValue(HKLM64, 'Software\Microsoft\Windows\Windows Error Reporting\LocalDumps\' + theExe, 'DumpFolder', ExpandConstant('{userappdata}') + '\Ditto\Dumps');
      if RegValueExists(HKLM64, 'Software\Microsoft\Windows\Windows Error Reporting\LocalDumps\' + theExe, 'DumpType') = false then
        RegWriteDWordValue(HKLM64, 'Software\Microsoft\Windows\Windows Error Reporting\LocalDumps\' + theExe, 'DumpType', 2);
      if RegValueExists(HKLM64, 'Software\Microsoft\Windows\Windows Error Reporting\LocalDumps\' + theExe, 'DumpCount') = false then
        RegWriteDWordValue(HKLM64, 'Software\Microsoft\Windows\Windows Error Reporting\LocalDumps\' + theExe, 'DumpCount', 3);
    end
  else
    begin
      if RegValueExists(HKEY_LOCAL_MACHINE, 'Software\Microsoft\Windows\Windows Error Reporting\LocalDumps\' + theExe, 'DumpFolder') = false then
        RegWriteStringValue(HKEY_LOCAL_MACHINE, 'Software\Microsoft\Windows\Windows Error Reporting\LocalDumps\' + theExe, 'DumpFolder', ExpandConstant('{userappdata}') + '\Ditto\Dumps');
      if RegValueExists(HKEY_LOCAL_MACHINE, 'Software\Microsoft\Windows\Windows Error Reporting\LocalDumps\' + theExe, 'DumpType') = false then
        RegWriteDWordValue(HKEY_LOCAL_MACHINE, 'Software\Microsoft\Windows\Windows Error Reporting\LocalDumps\' + theExe, 'DumpType', 2);
      if RegValueExists(HKEY_LOCAL_MACHINE, 'Software\Microsoft\Windows\Windows Error Reporting\LocalDumps\' + theExe, 'DumpCount') = false then
        RegWriteDWordValue(HKEY_LOCAL_MACHINE, 'Software\Microsoft\Windows\Windows Error Reporting\LocalDumps\' + theExe, 'DumpCount', 3);
    end;
end;

function IsVC2017CRuntimeInstalled(): Boolean;
var
  Installed: Boolean;
  IsInstalled: Cardinal;
begin
  Installed := false
  IsInstalled := 0;

  if RegQueryDWordValue(HKLM, 'SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x64', 'Installed', IsInstalled) then
  begin
    if (IsInstalled = 1) then
    begin
      Installed := true;
    end;
  end;

  //double check the HKLM64 key
  if (IsInstalled <> 1) and IsWin64() then
  begin
    if RegQueryDWordValue(HKLM64, 'SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x64', 'Installed', IsInstalled) then
    begin
      if (IsInstalled = 1) then
      begin
        Installed := true;
      end;
    end;
  end;

  Result := Installed;
end;

procedure CheckForPreReqs();
var
  nReturnCode: Integer;
begin


end;

procedure CurStepChanged(CurStep: TSetupStep);
var
  ProgressPage: TOutputProgressWizardPage;
  ErrorCode : Integer;
  AbortNeeded: Boolean;
begin
  AbortNeeded := false;
  case CurStep of
    ssInstall:
    begin
      CheckForPreReqs();
    end;

	  ssPostInstall:
  	begin
    end;

    ssDone:
    begin
      RegisterForCrashDump('Ditto')
      CleanupOldFiles()
    end;

  end;
end;


function RuleExistsInFirewall(RuleName : String) : Boolean;
var
  ErrorCode : Integer;
begin
  Exec('>', 'netsh advfirewall firewall show rule name="' + RuleName + '"', '', SW_HIDE, ewWaitUntilTerminated, ErrorCode);
  if ErrorCode = 0 then
    Result := True
  else
    Result := False;
end;

procedure AddProgramToFirewall(ProgramName : String; RuleName : String);
var
  ErrorCode : Integer;
  Success : Boolean;
  WindowsVersion : TWindowsVersion;
begin
  if IsTaskSelected('AddFireWallException') then
    begin
    GetWindowsVersionEx(WindowsVersion);
    if (WindowsVersion.Major < 6) then
      begin
        Success := Exec('>', 'netsh firewall add allowedprogram "' + ProgramName + '" "' + RuleName + '" ENABLE ALL', '', SW_HIDE, ewWaitUntilTerminated, ErrorCode);
      end
    else
      begin
        if (not RuleExistsInFirewall(RuleName)) then
          begin
            Success := Exec('>', 'netsh advfirewall firewall add rule name="' + RuleName + '" dir=in action=allow protocol=TCP localport=23443 program="' + ProgramName + '" enable=yes', '', SW_HIDE, ewWaitUntilTerminated, ErrorCode);
            Success := Exec('>', 'netsh advfirewall firewall add rule name="' + RuleName + '" dir=out action=allow protocol=TCP localport=23443 program="' + ProgramName + '" enable=yes', '', SW_HIDE, ewWaitUntilTerminated, ErrorCode);
          end
      end;

    if not Success then
       Log('Error - Unable to add ' + RuleName + ' to List of Windows firewall exceptions. ErrorCode: ' + IntToStr(ErrorCode))
    else
       Log(RuleName + ' successfully added to list of Windows firewall exceptions. ErrorCode: ' + IntToStr(ErrorCode))
  end
end;
