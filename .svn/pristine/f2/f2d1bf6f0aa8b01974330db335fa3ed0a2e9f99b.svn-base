#!/bin/bash

clear

if test -z "$1"
then
	echo "Format :: week.build.sh [ Week Number ]"
	echo " ====> ]# week.build.sh 20130318_0318"
	exit
fi

localAll=( BRZ MEX RUS TLD GER FRC PLD SPN ITA USA )
localAllcnt=10
local=( usa brz mex rus tld )
LOCAL=( USA BRZ MEX RUS TLD )
localcnt=5
xtraplocal=( ger frc pld spn ita )
XTRAPLOCAL=( GER FRC PLD SPN ITA )
xtrapcnt=5
folderYear=$(date +%Y)
folderWeek=$1"BUILD"
#folderSource=$(date +%Y%m%d)"-"$1"BUILD"
bSVNAdd="no"
#svnSourceFoldername=" /sourcedir/svn_source"
#svnServer=" svn://10.1.90.3/LastChaos_server"
rootDir="/RedMineLastChaosSystem/"
#Function
makeWeekFolder()
{
	for((i=0;i<localAllcnt;i++))
	do
		dirbuf=$rootDir${localAll[i]}"/Server/"$folderYear"/"
		if [ -d $dirbuf ]
		then
			echo $dirbuf
		else
			mkdir $dirbuf
			svn add $dirbuf
			svn commit -m"Create Happy New Year" $dirbuf
		fi
		dirbuf+=$folderWeek"/"
		if [ -d $dirbuf ]
		then
			echo $dirbuf
		else
			mkdir $dirbuf
		fi
	done
}

delxtrap()
{
	cp -R GameServer/ GameServer_noxtrap/
	cd GameServer_noxtrap/
	sed 's/#define XTRAP$//g' Config_Localize_GER.h > Config_Localize_GER.h.new
	mv Config_Localize_GER.h.new Config_Localize_GER.h
	sed 's/#define XTRAP$//g' Config_Localize_EUR.h > Config_Localize_EUR.h.new
	mv Config_Localize_EUR.h.new Config_Localize_EUR.h
	sed 's/#define XTRAP$//g' Config_Localize_ITA.h > Config_Localize_ITA.h.new
	mv Config_Localize_ITA.h.new Config_Localize_ITA.h
	sed 's/#define XTRAP$//g' Config_Localize_gamigo.h > Config_Localize_gamigo.h.new
	mv Config_Localize_gamigo.h.new Config_Localize_gamigo.h
	cd ../
}

makeBinaries()
{
	for((i=0;i<xtrapcnt;i++))
	do
		echo "**********************************************************"
		echo "                  ${XTRAPLOCAL[i]} BUILD                  "
		echo "**********************************************************"
		make cleanall
		make ${xtraplocal[i]}
		dirbuf=$rootDir${XTRAPLOCAL[i]}"/Server/"$folderYear"/"$folderWeek"/"
		mv Connector/Connector GameServer/GameServer_d Helper/Helper SubHelper/SubHelper Messenger/Messenger LoginServer/LoginServer GameServer_noxtrap/GameServer_noxtrap $dirbuf
		make cleanxtrap
        done
}

svnAdd()
{
	for((i=0;i<localAllcnt;i++))
	do
		dirbuf=$rootDir${localAll[i]}"/Server/"$folderYear"/"$folderWeek"/"
		svn add $dirbuf
		svn add $dirbuf*
	done
}

svn update
svn update $rootDir

makeWeekFolder
makeBinaries
#alld2u.sh
#cp.dump.new $1
svnAdd
currentDate=$date
svn commit -m "see #ALL : $currentDate ( $folderWeek )" $rootDir

