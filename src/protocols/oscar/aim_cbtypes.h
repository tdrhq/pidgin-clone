/*
 * AIM Callback Types
 *
 */
#ifndef __AIM_CBTYPES_H__
#define __AIM_CBTYPES_H__

/*
 * SNAC Families.
 */
#define AIM_CB_FAM_ACK 0x0000
#define AIM_CB_FAM_GEN 0x0001
#define AIM_CB_FAM_LOC 0x0002
#define AIM_CB_FAM_BUD 0x0003
#define AIM_CB_FAM_MSG 0x0004
#define AIM_CB_FAM_ADS 0x0005
#define AIM_CB_FAM_INV 0x0006
#define AIM_CB_FAM_ADM 0x0007
#define AIM_CB_FAM_POP 0x0008
#define AIM_CB_FAM_BOS 0x0009
#define AIM_CB_FAM_LOK 0x000a
#define AIM_CB_FAM_STS 0x000b
#define AIM_CB_FAM_TRN 0x000c
#define AIM_CB_FAM_CTN 0x000d /* ChatNav */
#define AIM_CB_FAM_CHT 0x000e /* Chat */
#define AIM_CB_FAM_SCH 0x000f /* "New" search */
#define AIM_CB_FAM_ICO 0x0010 /* Used for uploading buddy icons */
#define AIM_CB_FAM_SSI 0x0013 /* Server stored information */
#define AIM_CB_FAM_ICQ 0x0015
#define AIM_CB_FAM_ATH 0x0017
#define AIM_CB_FAM_EML 0x0018
#define AIM_CB_FAM_OFT 0xfffe /* OFT/Rvous */
#define AIM_CB_FAM_SPECIAL 0xffff /* Internal libfaim use */

/*
 * SNAC Family: Ack.
 * 
 * Not really a family, but treating it as one really
 * helps it fit into the libfaim callback structure better.
 *
 */
#define AIM_CB_ACK_ACK 0x0001

/*
 * SNAC Family: General.
 */ 
#define AIM_CB_GEN_ERROR 0x0001
#define AIM_CB_GEN_CLIENTREADY 0x0002
#define AIM_CB_GEN_SERVERREADY 0x0003
#define AIM_CB_GEN_SERVICEREQ 0x0004
#define AIM_CB_GEN_REDIRECT 0x0005
#define AIM_CB_GEN_RATEINFOREQ 0x0006
#define AIM_CB_GEN_RATEINFO 0x0007
#define AIM_CB_GEN_RATEINFOACK 0x0008
#define AIM_CB_GEN_RATECHANGE 0x000a
#define AIM_CB_GEN_SERVERPAUSE 0x000b
#define AIM_CB_GEN_SERVERRESUME 0x000d
#define AIM_CB_GEN_REQSELFINFO 0x000e
#define AIM_CB_GEN_SELFINFO 0x000f
#define AIM_CB_GEN_EVIL 0x0010
#define AIM_CB_GEN_SETIDLE 0x0011
#define AIM_CB_GEN_MIGRATIONREQ 0x0012
#define AIM_CB_GEN_MOTD 0x0013
#define AIM_CB_GEN_SETPRIVFLAGS 0x0014
#define AIM_CB_GEN_WELLKNOWNURL 0x0015
#define AIM_CB_GEN_NOP 0x0016
#define AIM_CB_GEN_DEFAULT 0xffff

/*
 * SNAC Family: Location Services.
 */ 
#define AIM_CB_LOC_ERROR 0x0001
#define AIM_CB_LOC_REQRIGHTS 0x0002
#define AIM_CB_LOC_RIGHTSINFO 0x0003
#define AIM_CB_LOC_SETUSERINFO 0x0004
#define AIM_CB_LOC_REQUSERINFO 0x0005
#define AIM_CB_LOC_USERINFO 0x0006
#define AIM_CB_LOC_WATCHERSUBREQ 0x0007
#define AIM_CB_LOC_WATCHERNOT 0x0008
#define AIM_CB_LOC_DEFAULT 0xffff

/*
 * SNAC Family: Buddy List Management Services.
 */ 
#define AIM_CB_BUD_ERROR 0x0001
#define AIM_CB_BUD_REQRIGHTS 0x0002
#define AIM_CB_BUD_RIGHTSINFO 0x0003
#define AIM_CB_BUD_ADDBUDDY 0x0004
#define AIM_CB_BUD_REMBUDDY 0x0005
#define AIM_CB_BUD_REJECT 0x000a
#define AIM_CB_BUD_ONCOMING 0x000b
#define AIM_CB_BUD_OFFGOING 0x000c
#define AIM_CB_BUD_DEFAULT 0xffff

/*
 * SNAC Family: Messeging Services.
 */ 
#define AIM_CB_MSG_ERROR 0x0001
#define AIM_CB_MSG_PARAMINFO 0x0005
#define AIM_CB_MSG_INCOMING 0x0007
#define AIM_CB_MSG_EVIL 0x0009
#define AIM_CB_MSG_MISSEDCALL 0x000a
#define AIM_CB_MSG_CLIENTAUTORESP 0x000b
#define AIM_CB_MSG_ACK 0x000c
#define AIM_CB_MSG_MTN 0x0014
#define AIM_CB_MSG_DEFAULT 0xffff

/*
 * SNAC Family: Advertisement Services
 */ 
#define AIM_CB_ADS_ERROR 0x0001
#define AIM_CB_ADS_DEFAULT 0xffff

/*
 * SNAC Family: Invitation Services.
 */ 
#define AIM_CB_INV_ERROR 0x0001
#define AIM_CB_INV_DEFAULT 0xffff

/*
 * SNAC Family: Administrative Services.
 */ 
#define AIM_CB_ADM_ERROR 0x0001
#define AIM_CB_ADM_INFOCHANGE_REPLY 0x0005
#define AIM_CB_ADM_DEFAULT 0xffff

/*
 * SNAC Family: Popup Messages
 */ 
#define AIM_CB_POP_ERROR 0x0001
#define AIM_CB_POP_DEFAULT 0xffff

/*
 * SNAC Family: Misc BOS Services.
 */ 
#define AIM_CB_BOS_ERROR 0x0001
#define AIM_CB_BOS_RIGHTSQUERY 0x0002
#define AIM_CB_BOS_RIGHTS 0x0003
#define AIM_CB_BOS_DEFAULT 0xffff

/*
 * SNAC Family: User Lookup Services
 */ 
#define AIM_CB_LOK_ERROR 0x0001
#define AIM_CB_LOK_DEFAULT 0xffff

/*
 * SNAC Family: User Status Services
 */ 
#define AIM_CB_STS_ERROR 0x0001
#define AIM_CB_STS_SETREPORTINTERVAL 0x0002
#define AIM_CB_STS_REPORTACK 0x0004
#define AIM_CB_STS_DEFAULT 0xffff

/*
 * SNAC Family: Translation Services
 */ 
#define AIM_CB_TRN_ERROR 0x0001
#define AIM_CB_TRN_DEFAULT 0xffff

/*
 * SNAC Family: Chat Navigation Services
 */ 
#define AIM_CB_CTN_ERROR 0x0001
#define AIM_CB_CTN_CREATE 0x0008
#define AIM_CB_CTN_INFO 0x0009
#define AIM_CB_CTN_DEFAULT 0xffff

/*
 * SNAC Family: Chat Services
 */ 
#define AIM_CB_CHT_ERROR 0x0001
#define AIM_CB_CHT_ROOMINFOUPDATE 0x0002
#define AIM_CB_CHT_USERJOIN 0x0003
#define AIM_CB_CHT_USERLEAVE 0x0004
#define AIM_CB_CHT_OUTGOINGMSG 0x0005
#define AIM_CB_CHT_INCOMINGMSG 0x0006
#define AIM_CB_CHT_DEFAULT 0xffff

/*
 * SNAC Family: "New" Search
 */ 
#define AIM_CB_SCH_ERROR 0x0001
#define AIM_CB_SCH_SEARCH 0x0002
#define AIM_CB_SCH_RESULTS 0x0003

/*
 * SNAC Family: Buddy icons
 */ 
#define AIM_CB_ICO_ERROR 0x0001
#define AIM_CB_ICO_REQUEST 0x0004
#define AIM_CB_ICO_RESPONSE 0x0005

/*
 * SNAC Family: ICQ
 *
 * Most of these are actually special.
 */ 
#define AIM_CB_ICQ_ERROR 0x0001
#define AIM_CB_ICQ_OFFLINEMSG 0x00f0
#define AIM_CB_ICQ_OFFLINEMSGCOMPLETE 0x00f1
#define AIM_CB_ICQ_INFO 0x00f2
#define AIM_CB_ICQ_ALIAS 0x00f3
#define AIM_CB_ICQ_DEFAULT 0xffff

/*
 * SNAC Family: Server-Stored Buddy Lists
 */
#define AIM_CB_SSI_ERROR 0x0001
#define AIM_CB_SSI_REQRIGHTS 0x0002
#define AIM_CB_SSI_RIGHTSINFO 0x0003
#define AIM_CB_SSI_REQLIST 0x0005
#define AIM_CB_SSI_LIST 0x0006
#define AIM_CB_SSI_ACTIVATE 0x0007
#define AIM_CB_SSI_ADD 0x0008
#define AIM_CB_SSI_MOD 0x0009
#define AIM_CB_SSI_DEL 0x000A
#define AIM_CB_SSI_SRVACK 0x000E
#define AIM_CB_SSI_NOLIST 0x000F
#define AIM_CB_SSI_EDITSTART 0x0011
#define AIM_CB_SSI_EDITSTOP 0x0012
#define AIM_CB_SSI_SENDAUTH 0x0014
#define AIM_CB_SSI_RECVAUTH 0x0015
#define AIM_CB_SSI_SENDAUTHREQ 0x0018
#define AIM_CB_SSI_RECVAUTHREQ 0x0019
#define AIM_CB_SSI_SENDAUTHREP 0x001a
#define AIM_CB_SSI_RECVAUTHREP 0x001b
#define AIM_CB_SSI_ADDED 0x001c

/*
 * SNAC Family: Authorizer
 *
 * Used only in protocol versions three and above.
 *
 */
#define AIM_CB_ATH_ERROR 0x0001
#define AIM_CB_ATH_LOGINREQEST 0x0002
#define AIM_CB_ATH_LOGINRESPONSE 0x0003
#define AIM_CB_ATH_AUTHREQ 0x0006
#define AIM_CB_ATH_AUTHRESPONSE 0x0007

/*
 * SNAC Family: Email
 *
 * Used for getting information on the email address
 * associated with your screen name.
 *
 */
#define AIM_CB_EML_ERROR 0x0001
#define AIM_CB_EML_SENDCOOKIES 0x0006
#define AIM_CB_EML_MAILSTATUS 0x0007
#define AIM_CB_EML_INIT 0x0016

/*
 * OFT Services
 *
 * See non-SNAC note below.
 */
#define AIM_CB_OFT_DIRECTIMCONNECTREQ 0x0001/* connect request -- actually an OSCAR CAP */
#define AIM_CB_OFT_DIRECTIMINCOMING 0x0002
#define AIM_CB_OFT_DIRECTIMDISCONNECT 0x0003
#define AIM_CB_OFT_DIRECTIMTYPING 0x0004
#define AIM_CB_OFT_DIRECTIM_ESTABLISHED 0x0005

#if 0
/* Are this arbitrary?! */
#define AIM_CB_OFT_GETFILECONNECTREQ 0x0006	/* connect request -- actually an OSCAR CAP */
#define AIM_CB_OFT_GETFILELISTINGREQ 0x0007	/* OFT listing.txt request */
#define AIM_CB_OFT_GETFILEFILEREQ 0x0008	/* received file request */
#define AIM_CB_OFT_GETFILEFILESEND 0x0009	/* received file request confirm -- send data */
#define AIM_CB_OFT_GETFILECOMPLETE 0x000a	/* received file send complete*/
#define AIM_CB_OFT_GETFILE_ESTABLISHED 0x000b	/* request for file get acknowledge */
#define AIM_CB_OFT_GETFILEDISCONNECT 0x000c	/* OFT connection disconnected.*/
#define AIM_CB_OFT_GETFILELISTING 0x000d	/* OFT listing.txt received.*/
#define AIM_CB_OFT_GETFILERECEIVE 0x000e	/* OFT file incoming.*/
#define AIM_CB_OFT_GETFILELISTINGRXCONFIRM 0x000f
#define AIM_CB_OFT_GETFILESTATE4 0x0010
#endif

#define AIM_CB_OFT_SENDFILE_REQUEST 0x0011	/* started receiving file */
#define AIM_CB_OFT_SENDFILE_SEND 0x0012		/* buddy ready to for us to send */
#define AIM_CB_OFT_SENDFILE_COMPLETE 0x0013	/* send to buddy complete */
#define AIM_CB_OFT_SENDFILE_RESUME 0x0015	/* somethin' */

#define AIM_CB_OFT_PROMPT 0x0101		/* Just connected to another buddy, receiver sends this */
#define AIM_CB_OFT_RESUMESOMETHING 0x0106	/* I really don't know */
#define AIM_CB_OFT_ACK 0x0202			/* Just after receiving the prompt, sender sends this */
#define AIM_CB_OFT_DONE 0x0204			/* After a transfer is complete, sender sends this */
#define AIM_CB_OFT_RESUME 0x0205		/* Resume transferring, send by whoever paused? */
#define AIM_CB_OFT_RESUMEACK 0x0207		/* Not really sure */

#define AIM_CB_OFT_GETFILE_REQUESTLISTING 0x1108 /* request a listing.txt file from the remote user */
#define AIM_CB_OFT_GETFILE_SENDLISTING 0x1208	/* we're going to send a listing.txt file */
#define AIM_CB_OFT_GETFILE_ACKLISTING 0x120b	/* we received the listing.txt file, yay */
#define AIM_CB_OFT_GETFILE_REQUESTFILE 0x120c	/* request a specific file from the remote user */

#define AIM_CB_OFT_ESTABLISHED 0xFFFF		/* connection to buddy initiated */

/*
 * SNAC Family: Internal Messages
 *
 * This isn't truely a SNAC family either, but using
 * these, we can integrated non-SNAC services into
 * the SNAC-centered libfaim callback structure.
 *
 */ 
#define AIM_CB_SPECIAL_AUTHSUCCESS 0x0001
#define AIM_CB_SPECIAL_AUTHOTHER 0x0002
#define AIM_CB_SPECIAL_CONNERR 0x0003
#define AIM_CB_SPECIAL_CONNCOMPLETE 0x0004
#define AIM_CB_SPECIAL_FLAPVER 0x0005
#define AIM_CB_SPECIAL_CONNINITDONE 0x0006
#define AIM_CB_SPECIAL_IMAGETRANSFER 0x007
#define AIM_CB_SPECIAL_MSGTIMEOUT 0x008
#define AIM_CB_SPECIAL_UNKNOWN 0xffff
#define AIM_CB_SPECIAL_DEFAULT AIM_CB_SPECIAL_UNKNOWN

/* SNAC flags */
#define AIM_SNACFLAGS_DESTRUCTOR 0x0001

#endif/*__AIM_CBTYPES_H__ */
