#!/bin/bash

#TODO
#generate /win/WordQuiz.rc?

VFILE="/../app/src/version.h"
IFILE="/win/iss_include.txt"
PADFILE="/win/wordquiz.xml"
PFILE="/mac/info.plist"
GPLFILE="LICENSE.GPL2"

show_usage() {
echo
echo "Usage:"
echo "Writes a version.h file for WordQuiz"
echo "Writes an include file for /win/wordquiz.iss (Inno Setup)"
echo "Writes an updated info.plist in /mac"
echo "Writes an updated copyright line in LICENSE.GPL2"
echo
exit
}

write_version_h () {
echo "#ifndef WQ_VERSION" > $VERSIONFILE
echo "" >> $VERSIONFILE
echo "#define WQ_VERSION          \"$WQ_VERSION_YEAR.$WQ_VERSION_MONTH.$WQ_VERSION_DAY\"" >> $VERSIONFILE
echo "#define WQ_FILEVERSION      $WQ_VERSION_YEAR,$WQ_VERSION_MONTH,$WQ_VERSION_DAY,0" >> $VERSIONFILE
echo "#define WQ_FILEVERSION_STR  \"$WQ_VERSION_YEAR.$WQ_VERSION_MONTH.$WQ_VERSION_DAY.0\0\"" >> $VERSIONFILE
echo "#define WQ_COPYRIGHT_STR    \"Copyright (C) 1997-$WQ_VERSION_YEAR Peter B Hedlund\0\"" >> $VERSIONFILE
echo "" >> $VERSIONFILE
echo "#endif" >> $VERSIONFILE
}

write_iss_include () {
echo "AppName=WordQuiz" > $ISSFILE
echo "AppVerName=WordQuiz $WQ_VERSION_YEAR.$WQ_VERSION_MONTH.$WQ_VERSION_DAY" >> $ISSFILE
echo "DefaultDirName={pf}\WordQuiz" >> $ISSFILE
echo "AllowNoIcons=true" >> $ISSFILE
echo "ChangesAssociations=true" >> $ISSFILE
echo "WindowShowCaption=false" >> $ISSFILE
echo "WindowResizable=false" >> $ISSFILE
echo "AppCopyright=Copyright (C) 1997-$WQ_VERSION_YEAR Peter B Hedlund" >> $ISSFILE
echo "BackColor=\$ff0000" >> $ISSFILE
echo "DefaultGroupName=WordQuiz" >> $ISSFILE
echo "LicenseFile=X:\Projects\WordQuiz\dist\LICENSE.GPL2" >> $ISSFILE
echo "MinVersion=0,5.0.2195" >> $ISSFILE
echo "OutputBaseFilename=wqsetup" >> $ISSFILE
echo "OutputDir=X:\Projects\WordQuiz\dist\win" >> $ISSFILE
echo "SourceDir=C:\WINDOWS\TEMP\smtemp" >> $ISSFILE
echo "WizardImageBackColor=\$808000" >> $ISSFILE
echo "WizardImageFile=X:\Projects\WordQuiz\dist\win\wizimglarge.bmp" >> $ISSFILE
echo "AppPublisher=Peter Hedlund" >> $ISSFILE
echo "AppPublisherURL=http://peterandlinda.com/wordquiz" >> $ISSFILE
echo "AppSupportURL=http://peterandlinda.com/wordquiz" >> $ISSFILE
echo "AppUpdatesURL=http://peterandlinda.com/wordquiz" >> $ISSFILE
echo "Compression=lzma/ultra" >> $ISSFILE
echo "UninstallDisplayIcon={app}\WordQuiz.exe" >> $ISSFILE
echo "WizardSmallImageFile=X:\Projects\WordQuiz\dist\win\wizimgsmall.bmp" >> $ISSFILE
echo "UsePreviousSetupType=true" >> $ISSFILE
echo "DisableReadyMemo=false" >> $ISSFILE
echo "AlwaysShowComponentsList=false" >> $ISSFILE
echo "WindowStartMaximized=false" >> $ISSFILE
echo "AppMutex=phWORDQUIZ_APPLICATION_MUTEX" >> $ISSFILE
echo "AppID=phWordQuiz" >> $ISSFILE

echo "InternalCompressLevel=normal" >> $ISSFILE
echo "SolidCompression=true" >> $ISSFILE
echo "ShowLanguageDialog=auto" >> $ISSFILE
echo "AppVersion=$WQ_VERSION_YEAR,$WQ_VERSION_MONTH,$WQ_VERSION_DAY,0" >> $ISSFILE
echo "VersionInfoVersion=$WQ_VERSION_YEAR.$WQ_VERSION_MONTH.$WQ_VERSION_DAY" >> $ISSFILE
echo "VersionInfoCompany=Peter Hedlund" >> $ISSFILE
echo "VersionInfoDescription=WordQuiz Installer" >> $ISSFILE
echo "VersionInfoTextVersion=$WQ_VERSION_YEAR.$WQ_VERSION_MONTH.$WQ_VERSION_DAY" >> $ISSFILE
echo "PrivilegesRequired=admin" >> $ISSFILE

}

# Minimum number of arguments needed by this program
MINARGS=0

# show usage if '-h' or  '--help' is the first argument or no argument is given
case $1 in
  "-h"|"--help") show_usage ;;
esac

# get the number of command-line arguments given
ARGC=$#

# check to make sure enough arguments were given or exit
if [[ $ARGC -lt $MINARGS ]] ; then
 echo "Too few arguments given (Minimum:$MINARGS)"
 echo
 show_usage
fi

WQ_VERSION_YEAR=$(date +"%Y")
WQ_VERSION_MONTH=$(date +"%m")
WQ_VERSION_DAY=$(date +"%d")

MYDIR=`dirname $0`
#echo $MYDIR
VERSIONFILE=$MYDIR$VFILE
#echo $VERSIONFILE
ISSFILE=$MYDIR$IFILE
PLISTFILE=$MYDIR$PFILE

write_version_h
write_iss_include
sed -i '3s/.*/WordQuiz is Copyright (C) 1997-'$WQ_VERSION_YEAR' Peter B Hedlund./' $GPLFILE

#xmlstar.sf.net
xmlstarlet edit -u "/plist/dict/key[.='CFBundleShortVersionString']/following-sibling::node()[1]" -v $WQ_VERSION_YEAR.$WQ_VERSION_MONTH.$WQ_VERSION_DAY $PLISTFILE.template > $PLISTFILE

echo "Don't forget to:"
echo "  Recompile WordQuiz"
echo "  Recompile the Windows installer"
echo "  Update the PAD File"
echo ""
