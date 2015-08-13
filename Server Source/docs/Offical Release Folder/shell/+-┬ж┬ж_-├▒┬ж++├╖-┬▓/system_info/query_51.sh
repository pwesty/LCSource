#!/bin/bash

work_ip=`/sbin/ifconfig eth0 | grep "inet addr" | awk -F: '{print $2}' | awk '{print $1}'`
work_day=`date +%Y-%m-%d --date '1 days ago'`

echo "To: 라스트카오스 관리자"
echo "From: 라카컴파일서버<icebug0@barunsongames.com>"
echo "Subject: TLD_gather query informaion - $work_ip"

echo ""
echo "$work_ip - $work_day"

echo ""
echo "-------------------------Query Error Info -------------------------------"
for i in 3 4
do
	for j in 1 2 3 4 5
	do
		work_file="/home/LastChaos/GameServer/LogFiles/Gameserver_"$i"_"$j".log."$work_day
		echo ""
		echo "---["$work_file"]"
		cat $work_file | grep 'ERROR' | grep 'qeury'
	done
done

