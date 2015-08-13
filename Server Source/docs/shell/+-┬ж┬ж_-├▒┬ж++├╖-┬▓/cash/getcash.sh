#!/bin/bash

work_day=`date +%Y-%m-%d --date '1 days ago'`
search_key=BUY_CASH_ITEM_START
result_dir=./result

ssh root@202.183.192.50 "cat /home/LastChaos/Connector1/LogFiles/Connector_1.log.$work_day | grep $search_key" > $result_dir/1_$work_day.txt
ssh root@202.183.192.50 "cat /home/LastChaos/Connector2/LogFiles/Connector_2.log.$work_day | grep $search_key" > $result_dir/2_$work_day.txt

ssh root@202.183.192.51 "cat /home/LastChaos/Connector3/LogFiles/Connector_3.log.$work_day | grep $search_key" > $result_dir/3_$work_day.txt
ssh root@202.183.192.51 "cat /home/LastChaos/Connector4/LogFiles/Connector_4.log.$work_day | grep $search_key" > $result_dir/4_$work_day.txt

./calc.py $work_day

/usr/sbin/ssmtp icebug0@barunsongames.com < sum_one_month.txt
/usr/sbin/ssmtp travis.kim@barunsongames.com < sum_one_month.txt
