; Script created by ISTool
; http://www.lerstad.com/istool.htm

#define QTDIR "C:\QtSDK\Desktop\Qt\4.7.3\mingw\bin"
#define WQDIR "X:\Projects\WordQuiz"

[Setup]
#include "iss_include.txt"

[Dirs]

[Files]
;Common
Source: X:\Projects\WordQuiz-build-win\app\bin\WordQuiz.exe; DestDir: {app}\; DestName: WordQuiz.exe;
Source: {#WQDIR}\app\src\sounds\correct.wav; DestDir: {app}\; DestName: correct.wav; 
Source: {#WQDIR}\app\src\sounds\error.wav; DestDir: {app}\; DestName: error.wav;
Source: {#WQDIR}\app\src\sounds\syntax_error.wav; DestDir: {app}\; DestName: syntax_error.wav;
Source: {#QTDIR}\QtCore4.dll; DestDir: {app}
Source: {#QTDIR}\QtGui4.dll; DestDir: {app}
Source: {#QTDIR}\QtXml4.dll; DestDir: {app}
Source: {#QTDIR}\QtOpenGL4.dll; DestDir: {app}
Source: {#QTDIR}\phonon4.dll; DestDir: {app}
Source: {#QTDIR}\libgcc_s_dw2-1.dll; DestDir: {app}
Source: {#QTDIR}\mingwm10.dll; DestDir: {app}

;English
Source: {#WQDIR}\app\kwordquiz\src\examples\fill_in_the_blank.kvtml; DestDir: {userappdata}\WordQuiz; Languages: English; Tasks: 
Source: {#WQDIR}\app\kwordquiz\src\examples\example.kvtml; DestDir: {userappdata}\WordQuiz; Languages: English
Source: {#WQDIR}\app\kwordquiz\src\examples\french_verbs.kvtml; DestDir: {userappdata}\WordQuiz; Languages: English
Source: {#WQDIR}\app\kwordquiz\src\examples\us_states_and_capitals.kvtml; DestDir: {userappdata}\WordQuiz; Languages: English

;Swedish

[Icons]
;Common
Name: {group}\WordQuiz; Filename: {app}\WordQuiz.exe; WorkingDir: {app}; IconFilename: {app}\WordQuiz.exe

;English
Name: {group}\Uninstall WordQuiz; FileName: {uninstallexe}; WorkingDir: {app}; Languages: English
Name: {group}\WordQuiz Home Page; FileName: http://peterandlinda.com/wordquiz; Languages: English

;Swedish

[INI]

[InstallDelete]

[Registry]
;wql
Root: HKCR; Subkey: .wql; ValueType: string; ValueData: WordQuiz.Document; Flags: uninsdeletekeyifempty uninsdeletevalue; Tasks: WqlAssocTask
Root: HKCR; Subkey: WordQuiz.Document; ValueType: string; ValueData: WordQuiz Document; Flags: uninsdeletekeyifempty uninsdeletevalue; Tasks: WqlAssocTask
Root: HKCR; Subkey: WordQuiz.Document\DefaultIcon; ValueType: string; ValueData: {app}\wordquiz.exe,1; Flags: uninsdeletekeyifempty uninsdeletevalue; Tasks: WqlAssocTask
Root: HKCR; Subkey: WordQuiz.Document\Shell\Open\Command; ValueType: string; ValueData: "{app}\wordquiz.exe ""%1"""; Flags: uninsdeletekeyifempty uninsdeletevalue; Tasks: WqlAssocTask

;kvtml
Root: HKCR; SubKey: .kvtml; ValueType: string; ValueData: KEduVocDocument; Flags: uninsdeletekey; Tasks: KvtmlAssocTask
Root: HKCR; SubKey: KEduVocDocument; ValueType: string; ValueData: KDE Vocabulary Document; Flags: uninsdeletekey; Tasks: KvtmlAssocTask
Root: HKCR; SubKey: KEduVocDocument\Shell\Open\Command; ValueType: string; ValueData: """{app}\WordQuiz.exe"" ""%1"""; Flags: uninsdeletevalue; Tasks: KvtmlAssocTask
Root: HKCR; Subkey: KEduVocDocument\DefaultIcon; ValueType: string; ValueData: {app}\WordQuiz.exe,1; Flags: uninsdeletevalue; Tasks: KvtmlAssocTask

;Settings
Root: HKCU; Subkey: Software\PHSoftware\WordQuiz; Flags: uninsdeletekey
Root: HKLM; Subkey: Software\Microsoft\Windows\CurrentVersion\App Paths\WordQuiz.exe; ValueType: string; ValueData: {app}\WordQuiz.exe; Flags: uninsdeletekeyifempty uninsdeletevalue

[Run]
Filename: {app}\WordQuiz.exe; WorkingDir: {app}; Flags: unchecked postinstall nowait

[UninstallDelete]

[UninstallRun]

[Messages]

[Tasks]
Name: KvtmlAssocTask; Description: Associate WordQuiz with KDE Vocabulary files (.kvtml); GroupDescription: Create File Associations:
Name: WqlAssocTask; Description: Associate WordQuiz with WordQuiz files (.wql); GroupDescription: Create File Associations:


[_ISToolPreCompile]

[_ISTool]

[Languages]
Name: English; MessagesFile: compiler:Default.isl; LicenseFile: {#WQDIR}\dist\LICENSE.GPL2; 

[InnoIDE_Settings]
LogFileOverwrite=false

