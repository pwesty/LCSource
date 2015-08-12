#!/bin/bash

work_day=`date +%Y-%m-%d --date '1 days ago'`

ssh root@202.183.192.50 < work_50.sh > ./result/50_$work_day.txt
ssh root@202.183.192.51 < work_51.sh > ./result/51_$work_day.txt

ssh root@202.183.192.50 < query_50.sh > ./result/50_query_$work_day.txt
ssh root@202.183.192.51 < query_51.sh > ./result/51_query_$work_day.txt

ssmtp icebug0@barunsongames.com < ./result/51_$work_day.txt
ssmtp icebug0@barunsongames.com < ./result/50_$work_day.txt

ssmtp jungsoo@barunsongames.com < ./result/51_$work_day.txt
ssmtp jungsoo@barunsongames.com < ./result/50_$work_day.txt

ssmtp travis.kim@barunsongames.com < ./result/51_$work_day.txt
ssmtp travis.kim@barunsongames.com < ./result/50_$work_day.txt



ssmtp icebug0@barunsongames.com < ./result/51_query_$work_day.txt
ssmtp icebug0@barunsongames.com < ./result/50_query_$work_day.txt

ssmtp jungsoo@barunsongames.com < ./result/51_query_$work_day.txt
ssmtp jungsoo@barunsongames.com < ./result/50_query_$work_day.txt
