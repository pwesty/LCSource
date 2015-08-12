#!/usr/bin/python2.6
#-*- coding: euc-kr -*-

import os
import sys
import time
import datetime

log_dir = './LogFiles'
yesterday = datetime.date.fromtimestamp(time.time() - (86400)).strftime('%Y-%m-%d')

# 국가코드, 국가명, [] - 유효한 IP
nation_info = {
    '0' : ['한국',
           ['10.1.40.2', '10.1.40.82', '10.1.90.30', '10.1.90.8']],
    '4' : ['태국',
           ['10.1.90.27', '10.1.90.82', '202.183.192.50', '202.183.192.51', '103.251.70.30']],
    '9' : ['미국',
           ['10.1.90.29', '10.1.90.84']],
    '10' : ['브라질',
            ['10.1.90.25', '10.1.90.26', '10.1.90.80', '10.1.90.81']],
    '13' : ['독일',
            ['10.1.90.28', '10.1.90.83']],
    '14' : ['스페인',
            []],
    '15' : ['프랑스',
            ['10.1.90.31', '10.1.90.86']],
    '16' : ['폴란드',
            []],
    '17' : ['러시아',
            ['10.1.90.33', '10.1.90.88']],
    '18' : ['터키',
            []],
    '19' : ['이탈리아',
            []],
    '20' : ['멕시코',
            ['10.1.90.25', '10.1.90.26', '10.1.90.80', '10.1.90.81']],
    '24' : ['영국',
            []]
}

server = {} # 모든 서버 정보를 가지는 변수
client = {} # 모든 클라이언트 정보를 가지는 변수

server_check_keyword = 'SERVER_INFO -'
client_check_keyword = 'CLIENT_INFO -'

def line_analysis(store, keyword, line):
    line = line[:-1]
    t = line.split(keyword)
    info = t[1].split(' : ')
    del(info[::2])  # 데이터만 추출

    nation = info[0]
    ip = info[1]
    port = info[2]
    client_ip = info[3]

    # 새로운 국가라면 사전 데이터를 추가
    if nation not in store:
            store[nation] = []

    # 관리 대상 국가라면, 유효 IP는 제외
    if nation in nation_info:
            if ip in nation_info[nation][1]:
                    return

    # 기록된 IP가 아니라면
    if ip not in store[nation]:
            store[nation].append(ip)
   
def print_result(f, store):
    for key in store.keys():
        # 국가명 출력하기
        if key in nation_info:
            f.write('[%s] -----\n' % nation_info[key][0])
        else:
            f.write('[Unknown] -----\n')

        # IP 출력하기
        for ip in store[key]:
            f.write('%s\n' % ip)

        f.write('\n')

#########################################################
# 로그 파일 가져오기
logfile = log_dir + '/authentication.log.' + yesterday

with open(logfile) as f:
    lines = f.readlines();
    for line in lines:
        # 서버 정보를 분석
        if server_check_keyword in line:
            line_analysis(server, server_check_keyword, line)
            
        # 클라이언트 정보를 분석
        elif client_check_keyword in line:
            line_analysis(client, client_check_keyword, line)

#########################################################
# 생성된 정보를 바탕으로 파일 생성
result_filename = 'result.txt'
with open('result.txt', 'wt') as f:
    f.write('To: 라스트카오스관리자\n')
    f.write('From: 라카서버추적<icebug0@barunsonena.com>\n')
    f.write('Subject: 라스트카오스 - 서버 기동 정보\n\n')
    
    f.write('-------------------- 실행된 서버의 IP --------------------\n\n')
    print_result(f, server)

    f.write('\n\n\n')

    f.write('-------------------- 클라이언트가 접속한 게임서버의 IP --------------------\n\n')
    print_result(f, client)
