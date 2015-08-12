#ifndef __NTAM_MMCID_HEADER_FILE__
#define __NTAM_MMCID_HEADER_FILE__

#define MI_USER_LOGIN                   1401
#define MI_USER_LOGOUT                  1402
#define MI_ADD_USER                     1102
#define MI_DEL_USER                     1103
#define MI_CHG_USER_INFO                1104
#define MI_KILL_USER                    1105

/* MMCD New COMMAND */
#define MI_DIS_ALM_LOAD                 1500
#define MI_SET_ALM_LOAD                 1501
#define MI_DIS_LOG_LVL                  1502
#define MI_SET_LOG_LVL                  1503
#define MI_DIS_NET_USE					1525

#define MI_RELOAD_LOG_LVL               1602
#define MI_DIS_SYS_LOAD                 1603
#define MI_SET_SYS_LOAD                 1604

#define MI_DIS_PRC						1800
#define MI_GET_PRC                      1504
#define MI_ACT_PRC                      1505
#define MI_DACT_PRC                     1506
#define MI_SET_USER_SPD					1953
#define MI_DIS_USER_SPD					1954


/* GDMS SEND */
#define MI_CHG_SERV_INFO                1513
#define MI_GET_SERV_INFO                1514

/* GDAS SEND */
#define MI_DIS_EVENT_INFO               1515
#define MI_DEL_EVENT_INFO               1516
#define MI_DIS_GMARKET_EVT              1517

/* CSMS SEND */
#define MI_CHG_CHNL_INFO                1522
#define MI_GET_CHNL_INFO                1523
#define MI_CHG_CHNL_USER                1524
#define MI_CHG_ROOM_INFO                1525
#define MI_FIND_USER                    1526
#define MI_DIS_GAME_INFO                1527
#define MI_CHG_GAME_INFO                1528
#define MI_DIS_CHNL_STS                 1529
#define MI_DIS_ROOM_STS                 1530
#define MI_DIS_USER_STS                 1531
#define MI_DIS_WAIT_STS                 1532
#define MI_USER_BAN                     1533
#define MI_USER_BAN_ALL                 1534
#define MI_DIS_RCP_TBL                  1535
#define MI_DEL_RCP_TBL                  1536
#define MI_DIS_STG_LAP                  1537    /* MAP NO */
#define MI_ADD_STG_LAP                  1538
#define MI_SET_STG_LAP                  1539
#define MI_DEL_STG_LAP                  1540

#define MI_DIS_USER_INFO				1544
#define MI_DIS_USER_ITEM				1545
#define MI_DIS_USER_MISS				1546

/* BSMS SEND */
#define MI_NOTICE_MSG                   1541
#define MI_BROAD_MSG                    1542
#define MI_CHAT_DENY                    1543

/* ATTS SEND */
#define MI_SET_GAME_STS                 1544
#define MI_DIS_GAME_STS                 1545
#define MI_ADD_GAME_STS                 1546
#define MI_DEL_GAME_STS                 1547
#define MI_SET_EXT_STS                  1548

/* LOG SEND */
#define MI_DIS_GAME_LOG					1549
#define MI_SET_TOOL_LOG					1550
#define MI_DIS_TOOL_LOG					1551
#define MI_DIS_BLK_LOG					1552

/* BILLING SEND */
#define MI_DIS_USER_CASH				1553

/* GDBS SEND */
#define MI_DIS_UDB_INFO					1557
#define MI_DIS_UDB_ITEM					1558
#define MI_DIS_UDB_MISS					1559
#define MI_SET_UDB_INFO					1560
#define MI_SET_UDB_ITEM					1561
#define MI_GET_USER_ID					1562
#define MI_ADD_USER_BLK					1563
#define MI_DEL_USER_BLK					1564
#define MI_DIS_USER_BLK					1565
#define MI_DIS_USER_GIFT				1566
#define MI_SET_UDB_KERON				1572
#define MI_SET_USER_GIFT				1574

#define MI_DIS_UDB_INVMAX				1567

/* ONLY TEMS USE */
#define MI_ADD_IP_BLOK                  1600
#define MI_DEL_IP_BLOK                  1601
#define MI_DIS_IP_BLOK                  1602
#define MI_RELOAD_IP_BLOK               1603
#define MI_DIS_PKT_VER                  1604
#define MI_SET_PKT_VER                  1605
#define MI_DIS_NET_STS                  1606
#define MI_DIS_GAP_TIME                 1607
#define MI_DIS_CUR_USER                 1608
#define MI_DIS_PKT_DELAY                1609
#define MI_SET_PKT_DELAY                1610
#define MI_DIS_PING_TIME                1611
#define MI_CHG_PING_TIME                1612
#define MI_DIS_CUR_STAT                 1613

#define MI_ADD_EVENT_INFO				1700
#define MI_SET_EVENT_INFO				1701
#define MI_ADD_RCP_INFO					1702
#define MI_SET_RCP_INFO					1703

#define MI_DIS_USER_LOGIN				1106
#define MI_CHG_USER_PASS				1107

#define MI_DIS_HIS_CMD                  1201
#define MI_DIS_CMD_EXE                  1202
#define MI_DEL_CMD_EXE                  1203

#define MI_DIS_ADMIN					1580
#define MI_ADD_ADMIN					1581
#define MI_MOD_ADMIN					1582
#define MI_DEL_ADMIN					1583
#define MI_DIS_GDMS_USER                1584

#define MI_GET_COM						9900
#endif

