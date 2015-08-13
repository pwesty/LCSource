#!/bin/bash

if [ -z $1 ]; then
	echo ""
	echo "Usage : $0 [billa | tld | ger | usa | frc | uk | rus]"
	echo ""

	exit 1;
fi

########################################################################################

if [ $1 = "billa" ]; then

	for na in "brz mex"
	do
		make cleanall && make $na || exit 1;
		scp GameServer/GameServer_d root@10.1.90.80:/home/qa/LastChaos/new/GameServer_d_$na
		scp Messenger/Messenger root@10.1.90.81:/home/qa/LastChaos/new
		scp Connector/Connector root@10.1.90.81:/home/qa/LastChaos/new/Connector_$na
		scp Helper/Helper root@10.1.90.81:/home/qa/LastChaos/new/Helper_$na
		scp SubHelper/SubHelper root@10.1.90.81:/home/qa/LastChaos/new/SubHelper_$na
		scp LoginServer/LoginServer root@10.1.90.81:/home/qa/LastChaos/new/LoginServer_$na
	done
	
	exit 0
	
fi

########################################################################################
	
if [ $1 = "tld" ]; then
	remote_dir="root@10.1.90.82:/home/qa/LastChaos/new"
elif [ $1 = "ger" ]; then
	remote_dir="root@10.1.90.83:/home/qa/LastChaos/new"
elif [ $1 = "usa" ]; then
	remote_dir="root@10.1.90.84:/home/qa/LastChaos/new"
elif [ $1 = "frc" ]; then
	remote_dir="root@10.1.90.86:/home/qa/LastChaos/new"
elif [ $1 = "uk" ]; then
	remote_dir="root@10.1.90.87:/home/qa/LastChaos/new"
elif [ $1 = "rus" ]; then
	remote_dir="root@10.1.90.88:/home/qa/LastChaos/new"
else
	echo ""
	echo "Not found nation ($1)"
	echo ""

	exit 1
fi

# compile #
make cleanall && make $1 || exit 1;
scp GameServer/GameServer_d Messenger/Messenger Connector/Connector Helper/Helper SubHelper/SubHelper LoginServer/LoginServer $remote_dir

echo ""
echo "Done ($remote_dir).....................Good luck"
echo ""
