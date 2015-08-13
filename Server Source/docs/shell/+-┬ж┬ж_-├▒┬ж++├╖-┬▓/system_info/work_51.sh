#!/bin/bash

work_ip=`/sbin/ifconfig eth0 | grep "inet addr" | awk -F: '{print $2}' | awk '{print $1}'`
work_day=`date +%Y-%m-%d --date '1 days ago'`

echo "To: 라스트카오스 관리자"
echo "From: 라카컴파일서버<icebug0@barunsongames.com>"
echo "Subject: TLD_gather system informaion - $work_ip"

echo ""
echo "$work_ip - $work_day"

echo ""
echo "-------------------------System Info ------------------------------------"
top -b1 | head -n5

echo ""
echo "-------------------------Disk Info --------------------------------------"
df -h

echo ""
echo "-------------------------Core File Info ---------------------------------"
find /home/LastChaos -name "core*" -exec ls -al {} \;

echo ""
echo "-------------------------Log Usage --------------------------------------"
#du -sh /home/LastChaos/Login/LogFiles
#du -sh /home/LastChaos/Messenger/LogFiles
du -sh /home/LastChaos/Connector3/LogFiles
du -sh /home/LastChaos/Connector4/LogFiles
du -sh /home/LastChaos/Helper3/LogFiles
du -sh /home/LastChaos/Helper4/LogFiles
du -sh /home/LastChaos/SubHelper3/LogFiles
du -sh /home/LastChaos/SubHelper4/LogFiles
du -sh /home/LastChaos/GameServer/LogFiles

echo ""
echo "-------------------------Hacking Info -----------------------------------"
for i in 3 4
do
	for j in 1 2 3 4 5
	do
		work_file="/home/LastChaos/GameServer/LogFiles/Gameserver_"$i"_"$j".log."$work_day
		echo ""
		echo "---["$work_file"]"
		cat $work_file | grep 'HACKING'
	done
done

