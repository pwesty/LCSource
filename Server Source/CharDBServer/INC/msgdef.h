#ifndef _MESSAGE_DEFINE_
#define _MESSAGE_DEFINE_

/* Message Code Definition */
#define DEF_SVC         1000
#define DEF_SYS         5000


/* Service ID Definition */
/* Service ID for Generic */
#define SID_SYS				0x01
#define SID_LOG				0x02
#define SID_MML				0x03
#define SID_RELOAD			0x04
#define SID_START			0x05
#define SID_KILL			0x06
#define SID_STATUS			0x07
#define	SID_MMCD			0x08
#define SID_MMLBIN          0x400

/* TEMS SYSTEM */
#define SID_C2TEMS			0x10

/* GDMS SYSTEMS */
#define SID_GDMS_CONN		0x20
#define SID_GDMS_CHL		0x21
#define SID_GDMS_CHAR		0x22
#define SID_GDMS_INFO		0x23

/* GDMS, GDBS COMMON */
#define SID_GDMS_GDBS		0x2B

/* GDMS, TEMS, CSMS COMMON */
#define SID_GDMS_TEMS_CSMS	0x2C

/* CSMS SYSTEM */
#define SID_C2CSMS			0x30
#define SID_BLOCK_CSMS		0x3A
#define SID_CSMS_CHAR		0x40
#define SID_CSMS_FIELD		0x50
#define SID_CSMS_MOB		0x51
#define SID_CSMS_BATTLE		0x52
#define SID_CSMS_ITEM		0x60
#define SID_CSMS_FIELDITEM	0x61

/* CSMS, GDBS COMMON */
#define SID_CSMS_GDBS_CHAR	0x4B
#define SID_CSMS_GDBS_ITEM	0x6B

/* CSMS to CSMS COMMON */
#define SID_CSMS_SYNC		0x3C

/* GSMS SYSTEM */
#define SID_MSGR_GROUP		0x70
#define SID_MSGR_BUDDY		0x71
#define SID_MSGR_CHAT		0x72

/* GSMS, SDBS COMMON */
#define SID_GSMS_SDBS_GROUP		0x80
#define SID_GSMS_SDBS_BUDDY		0x81
#define SID_GSMS_SDBS_CONN		0x82


/* Message ID Definition */
/* Message ID for MMCD */
#define MID_MML_REQ					101
#define MID_MML_RST					102
#define MID_MML_CHK					103
#define MID_MML_ACK					104

#define MID_LOG_WRITE       		301

/* Message ID for SID_SYS */
#define MID_SOCKCHK_REQ				0x11
#define MID_SOCKCHK_RESP			0x12
#define MID_SYSTEM_INFO				0x13

/* Message ID for SID_STATUS */
#define MID_CONSOL					0x11

/* Message Queue type define */
#define INSERT_DATA_REQ 	0x11
#define INSERT_DATA_RES 	0x12

/*message type*/

#define RELOCATE_STSOC_SERVICETYPE 	0x01
#define STSOC_RELOCATE_SERVICETYPE 	0x02

#define SID_RELOCATE 0x10

#endif
