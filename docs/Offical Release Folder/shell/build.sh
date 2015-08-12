#!/bin/bash

if [ -z $1 ] || [ -z $2 ]; then
	echo "./a.sh [ALL | TLD | BILA | RUS | GER | FRC | PLD | SPN | ITA | USA | UK | GAMIGO] code"
exit
fi

arg=$(echo $1 | tr 'a-z' 'A-Z')

if [ $arg = "ALL" ]; then
    compile="tld brz mex rus ger frc pld spn ita uk usa"
elif [ $arg = "GAMIGO" ]; then
    compile="brz mex ger frc pld spn ita uk usa"
elif [ $arg = "TLD" ]; then
    compile=tld
elif [ $arg = "BILA" ]; then
    compile="brz mex"
elif [ $arg = "RUS" ]; then
    compile=rus 
elif [ $arg = "GER" ]; then
    compile=ger
elif [ $arg = "FRC" ]; then
    compile=frc 
elif [ $arg = "pld" ]; then
    compile=pld 
elif [ $arg = "SPN" ]; then
    compile=spn 
elif [ $arg = "ITA" ]; then
    compile=ita 
elif [ $arg = "USA" ]; then
    compile=usa
elif [ $arg = "UK" ]; then
    compile=uk
else
	echo "-------------------------------------"
	echo "Error : not found nation code"
	echo "-------------------------------------"
	exit
fi

#echo $compile

############################################################################
root_dir="/RedMineLastChaosSystem/"
dir_name=$2_${2:4}"BUILD"

svn update
svn update $root_dir

for na in $compile
do
	na_upper=$(echo $na | tr 'a-z' 'A-Z')
	work_dir=${root_dir}${na_upper}"/Server/"$(date +%Y)"/"$dir_name
	if [ -e $work_dir ]; then
		echo "Exist dir - "$work_dir
	else
		mkdir $work_dir
	fi
	
	make cleanall && make $na || exit 1;
	
	cp Connector/Connector $work_dir
	cp GameServer/GameServer_d $work_dir
	cp Helper/Helper $work_dir
	cp LoginServer/LoginServer $work_dir
	cp Messenger/Messenger $work_dir
	cp SubHelper/SubHelper $work_dir
	
	if [ -e $work_dir ]; then
		echo "------------------------------------------------------------"
	else
		svn add $work_dir
	fi
done
############################################################################
svn_revision=`export LANG=en_US && svn info | grep Rev:`
log="source revision : "${svn_revision}
svn commit -m "${log}" $root_dir
