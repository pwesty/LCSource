#!/bin/bash

#######################################################################################################################
#1. To use this script, please input the build number at the first argument and the local name at the second argument.#
#2. If you do not input the local name at the second argument, it will be compiled as All Local.					  #
#######################################################################################################################

clear

if test -z "$1"
then
	echo "Format :: week.build.sh [ Week Number ]"
	echo " ====> ]# week.build.sh 20130318_0318"
	exit
fi

local_upper=( TLD BRZ MEX RUS GER FRC PLD SPN ITA USA )
local_lower=( tld brz mex rus ger frc pld spn ita usa )

local_gamigo_upper=( GER FRC PLD SPN ITA USA )
local_gamigo_lower=( ger frc pld spn ita USA )

local_bila_upper=( BRZ MEX )
local_bila_lower=( brz mex )

localAllCnt=10
localGamigoCnt=6
localBilaCnt=2

cnt=0
copy_local_upper=( 0 0 0 0 0 0 0 0 0 0 )
copy_local_lower=( 0 0 0 0 0 0 0 0 0 0 )
localname="ALL"

folderYear=$(date +%Y)
folderWeek=$1"BUILD"
rootDir="/RedMineLastChaosSystem/"

function make_()
{
	for((i=0;i<$1;i++))
	do
		echo "------------------------------"${copy_local_upper[i]}"_BUILD-----------------------------------"
		make cleanall && make ${copy_local_lower[i]} || exit 1;
		cp Connector/Connector GameServer/GameServer_d Helper/Helper SubHelper/SubHelper Messenger/Messenger LoginServer/LoginServer $rootDir${copy_local_upper[i]}"/Server/"$folderYear"/"$folderWeek"/"
		echo "Copy ${copy_local_upper[i]} OK"
	done
}

function makeFolder()
{
	for((i=0;i<localAllCnt;i++))
	do
		dirbuf=$rootDir${local_upper[i]}"/Server/"$folderYear"/"
		if [ -d $dirbuf ]
		then
			echo $dirbuf "is already exist"
		else
			mkdir -p $dirbuf
			svn add $dirbuf
			svn commit -m"Create Happy New Year" $dirbuf
		fi
		
		dirbuf+=$folderWeek"/"
		
		if [ -d $dirbuf ]
		then
			echo $dirbuf "is already exist"
		else
			mkdir -p $dirbuf
		fi
	done
}

function makeBinary()
{
	if [ -n "$2" ]
	then
		localname=$2

		#gamigo compile
		if [ $2 == "gamigo" ]
		then
			copy_local_upper=("${local_gamigo_upper[@]}")
			copy_local_lower=("${local_gamigo_lower[@]}")
			make_ $localGamigoCnt

		#bila compile
		elif [ $2 == "bila" ]
		then
			copy_local_upper=("${local_bila_upper[@]}")
			copy_local_lower=("${local_bila_lower[@]}")
			make_ $localBilaCnt
		
		#local compile (each)
		else
			copy_local_lower=$2
			for((i=0;i<localAllCnt;i++))
			do
				if [ "${local_lower[i]}" == $2 ]
				then
					copy_local_upper=$2
					make_ 1
					break
				else
					continue
				fi
			done
		fi
	else
		copy_local_upper=("${local_upper[@]}")
		copy_local_lower=("${local_lower[@]}")
		make_ $localAllCnt
	fi
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

svn up
svn up $rootDir
makeFolder
makeBinary $1 $2
svnAdd
currentDate=$date
svn commit -m "see #${localname} : $currentDate ( $folderWeek )" $rootDir

echo "**********************************************************************************************"
echo "**********************************FINISH THE COMPILE******************************************"
echo "**********************************************************************************************"
