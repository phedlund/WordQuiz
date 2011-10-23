#!/bin/bash

VFILE="/../app/src/version.h"
PFILE="/mac/info.plist"
AFILE="/mac/appcast.xml"
GFILE="/LICENSE.GPL2"

show_usage() {
echo
echo "Usage:"
echo "Writes a version.h file for WordQuiz"
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
WQ_PUB_DATE=$(date -u "+%a, %d %b %Y %X %z")

MYDIR=`dirname $0`
#echo $MYDIR
VERSIONFILE=$MYDIR$VFILE
#echo $VERSIONFILE
PLISTFILE=$MYDIR$PFILE
GPLFILE=$MYDIR$GFILE
APPCASTFILE=$MYDIR$AFILE

write_version_h
sed -i "" -e "3s/.*/WordQuiz is Copyright (C) 1997-$WQ_VERSION_YEAR Peter B Hedlund./" $GPLFILE

#Update info.plist
cp $PLISTFILE.txt $PLISTFILE
sed -i "" -e "s/\@WQ_COPYRIGHT@/Copyright (C) 1997-$WQ_VERSION_YEAR Peter B Hedlund/" $PLISTFILE
sed -i "" -e "s/\@WQ_VERSION@/$WQ_VERSION_YEAR.$WQ_VERSION_MONTH.$WQ_VERSION_DAY/" $PLISTFILE

#Update appcast.xml
cp $APPCASTFILE.txt $APPCASTFILE
sed -i "" -e "s/\@WQ_VERSION@/$WQ_VERSION_YEAR.$WQ_VERSION_MONTH.$WQ_VERSION_DAY/" $APPCASTFILE
sed -i "" -e "s/\@WQ_PUB_DATE@/$WQ_PUB_DATE/" $APPCASTFILE
