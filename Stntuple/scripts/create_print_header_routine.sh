#!/usr/bin/env bash

# echo $1  
# echo .............. $2

file=$1

 offline_version=v6_1_2
         release=`echo $MU2E_BASE_RELEASE`
 offline_git_tag=`cat $MU2E_BASE_RELEASE/.git/logs/HEAD | awk '{print $2}'` 
stntuple_git_tag=`cat $MU2E_BASE_RELEASE/Stntuple/.git/logs/HEAD | awk '{print $2}'`

#-----------------------------------------------------------------------
# first check if what is being built is different from dev_243
#-----------------------------------------------------------------------
# cvs rdiff -r $tag Stntuple 2>&1 | grep -v Diffing > /tmp/$$.$USER
# ndiff=`cat /tmp/$$.$USER | wc -c`
ndiff=0

if [ -f $file ] ; then rm $file ; fi

echo  "#include <cstdio>"                                                    >  $file
echo  "#include <cstdlib>"                                                   >> $file
echo  "#include <cstring>"                                                   >> $file
echo  ""
echo  "void stntuple_print_header() {"                                       >> $file
echo  "  printf(\"stnmaker.exe built on `date` by $USER@`hostname`\n\");"    >> $file
echo  "  printf(\"MU2E_BASE_RELEASE: $release, \");"                         >> $file
echo  "  printf(\"OFFLINE  GIT tag : $offline_git_tag\n\");"                 >> $file
echo  "  printf(\"STNTUPLE GIT tag : $stntuple_git_tag\n\");"                >> $file
echo  "}"                                                                    >> $file
echo  ""
echo  "void stntuple_get_version(char* Version, char* Text) {"               >> $file
echo  "  static char  txt[200];"                                             >> $file
echo  "  strcpy(Version,\"$offline_version\");"                              >> $file
echo  "  strcpy(txt,\"stnmaker.exe \");"                                     >> $file
echo  "  strcat(txt,\"built on `date` by $USER@`hostname`\");"               >> $file
echo  "  strcat(txt,\" MU2E_BASE_RELEASE $release, \");"                     >> $file
echo  "  strcat(txt,\" OFFLINE  GIT tag $offline_git_tag \");"               >> $file
echo  "  strcat(txt,\" STNTUPLE GIT tag $stntuple_git_tag \");"              >> $file
echo  "  strcpy(Text,txt);"                                                  >> $file
echo  "}"                                                                    >> $file

