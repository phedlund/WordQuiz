@echo off
set WQ_VERSION_YEAR=%date:~10,4%
set WQ_VERSION_MONTH=%date:~4,2%
set WQ_VERSION_DAY=%date:~7,2%

set MYDIR=%~dp0
set MYDIR=%MYDIR:~0,-1%
for %%d in (%MYDIR%) do set MYDIR2=%%~dpd
          echo ParentDirectory=%MYDIR2%

set MYDIR2=%MYDIR2:~0,-1%
for %%d in (%MYDIR2%) do set MYDIR3=%%~dpd
          echo ParentDirectory=%MYDIR3%

set VERSIONFILE=%MYDIR3%app\src\version.h
set GPLFILE=%MYDIR3%dist\LICENSE.GPL2
set ISSFILE=%MYDIR3%\dist\win\iss_include.txt
set APPCASTFILE=%MYDIR3%\dist\win\appcastwin.xml

::Generate version.h
echo #ifndef WQ_VERSION > %VERSIONFILE%
echo:>> %VERSIONFILE%
echo #define WQ_VERSION          "%WQ_VERSION_YEAR%.%WQ_VERSION_MONTH%.%WQ_VERSION_DAY%" >> %VERSIONFILE%
echo #define WQ_FILEVERSION      %WQ_VERSION_YEAR%,%WQ_VERSION_MONTH%,%WQ_VERSION_DAY%,0 >> %VERSIONFILE%
echo #define WQ_FILEVERSION_STR  "%WQ_VERSION_YEAR%.%WQ_VERSION_MONTH%.%WQ_VERSION_DAY%.0\0" >> %VERSIONFILE%
echo #define WQ_COPYRIGHT_STR    "Copyright (C) 1997-%WQ_VERSION_YEAR% Peter B Hedlund\0" >> %VERSIONFILE%
echo:>> %VERSIONFILE%
echo #endif >> %VERSIONFILE%

::Update license file
sed -i "3s/.*/WordQuiz is Copyright (C) 1997-%WQ_VERSION_YEAR% Peter B Hedlund./" %GPLFILE%

::Generate include file for Inno Setup
echo AppName=WordQuiz > %ISSFILE%
echo AppVerName=WordQuiz %WQ_VERSION_YEAR%.%WQ_VERSION_MONTH%.%WQ_VERSION_DAY% >> %ISSFILE%
echo DefaultDirName={pf}\WordQuiz >> %ISSFILE%
echo AllowNoIcons=true >> %ISSFILE%
echo ChangesAssociations=true >> %ISSFILE%
echo WindowShowCaption=false >> %ISSFILE%
echo WindowResizable=false >> %ISSFILE%
echo AppCopyright=Copyright (C) 1997-%WQ_VERSION_YEAR% Peter B Hedlund >> %ISSFILE%
echo BackColor=$ff0000 >> %ISSFILE%
echo DefaultGroupName=WordQuiz >> %ISSFILE%
echo LicenseFile=%GPLFILE% >> %ISSFILE%
echo MinVersion=0,5.0.2195 >> %ISSFILE%
echo OutputBaseFilename=wqsetup >> %ISSFILE%
echo OutputDir=%MYDIR% >> %ISSFILE%
echo SourceDir=C:\WINDOWS\TEMP\smtemp >> %ISSFILE%
echo WizardImageBackColor=$808000 >> %ISSFILE%
echo WizardImageFile=%MYDIR%\wizimglarge.bmp >> %ISSFILE%
echo AppPublisher=Peter Hedlund >> %ISSFILE%
echo AppPublisherURL=http://peterandlinda.com/wordquiz >> %ISSFILE%
echo AppSupportURL=http://peterandlinda.com/wordquiz >> %ISSFILE%
echo AppUpdatesURL=http://peterandlinda.com/wordquiz >> %ISSFILE%
echo Compression=lzma/ultra >> %ISSFILE%
echo UninstallDisplayIcon={app}\WordQuiz.exe >> %ISSFILE%
echo WizardSmallImageFile=%MYDIR%\wizimgsmall.bmp >> %ISSFILE%
echo UsePreviousSetupType=true >> %ISSFILE%
echo DisableReadyMemo=false >> %ISSFILE%
echo AlwaysShowComponentsList=false >> %ISSFILE%
echo WindowStartMaximized=false >> %ISSFILE%
echo AppMutex=phWORDQUIZ_APPLICATION_MUTEX >> %ISSFILE%
echo AppID=phWordQuiz >> %ISSFILE%
echo InternalCompressLevel=normal >> %ISSFILE%
echo SolidCompression=true >> %ISSFILE%
echo ShowLanguageDialog=auto >> %ISSFILE%
echo AppVersion=%WQ_VERSION_YEAR%,%WQ_VERSION_MONTH%,%WQ_VERSION_DAY%,0 >> %ISSFILE%
echo VersionInfoVersion=%WQ_VERSION_YEAR%.%WQ_VERSION_MONTH%.%WQ_VERSION_DAY% >> %ISSFILE%
echo VersionInfoCompany=Peter Hedlund >> %ISSFILE%
echo VersionInfoDescription=WordQuiz Installer >> %ISSFILE%
echo VersionInfoTextVersion=%WQ_VERSION_YEAR%.%WQ_VERSION_MONTH%.%WQ_VERSION_DAY% >> %ISSFILE%
echo PrivilegesRequired=admin >> %ISSFILE%

::Update appcastwin.xml
copy %APPCASTFILE%.txt %APPCASTFILE% /Y
sed -i "s/@WQ_VERSION@/%WQ_VERSION_YEAR%.%WQ_VERSION_MONTH%.%WQ_VERSION_DAY%.0/" %APPCASTFILE%
