/*
 *  aim_im.c
 *
 *  The routines for sending/receiving Instant Messages.
 *
 *  Note the term ICBM (Inter-Client Basic Message) which blankets
 *  all types of genericly routed through-server messages.  Within
 *  the ICBM types (family 4), a channel is defined.  Each channel
 *  represents a different type of message.  Channel 1 is used for
 *  what would commonly be called an "instant message".  Channel 2
 *  is used for negotiating "rendezvous".  These transactions end in
 *  something more complex happening, such as a chat invitation, or
 *  a file transfer.
 *
 *  In addition to the channel, every ICBM contains a cookie.  For
 *  standard IMs, these are only used for error messages.  However,
 *  the more complex rendezvous messages make suitably more complex
 *  use of this field.
 *
 */

#define FAIM_INTERNAL
#include <aim.h>

/*
 * Takes a msghdr (and a length) and returns a client type
 * code.  Note that this is *only a guess* and has a low likelihood
 * of actually being accurate.
 *
 * Its based on experimental data, with the help of Eric Warmenhoven
 * who seems to have collected a wide variety of different AIM clients.
 *
 *
 * Heres the current collection:
 *  0501 0003 0101 0101 01       AOL Mobile Communicator, WinAIM 1.0.414
 *  0501 0003 0101 0201 01       WinAIM 2.0.847, 2.1.1187, 3.0.1464, 
 *                                      4.3.2229, 4.4.2286
 *  0501 0004 0101 0102 0101     WinAIM 4.1.2010, libfaim (right here)
 *  0501 0001 0101 01            AOL v6.0, CompuServe 2000 v6.0, any
 *                                      TOC client
 *
 * Note that in this function, only the feature bytes are tested, since
 * the rest will always be the same.
 *
 */
faim_export fu16_t aim_fingerprintclient(fu8_t *msghdr, int len)
{
	static const struct {
		fu16_t clientid;
		int len;
		fu8_t data[10];
	} fingerprints[] = {
		/* AOL Mobile Communicator, WinAIM 1.0.414 */
		{ AIM_CLIENTTYPE_MC, 
		  3, {0x01, 0x01, 0x01}},

		/* WinAIM 2.0.847, 2.1.1187, 3.0.1464, 4.3.2229, 4.4.2286 */
		{ AIM_CLIENTTYPE_WINAIM, 
		  3, {0x01, 0x01, 0x02}},

		/* WinAIM 4.1.2010, libfaim */
		{ AIM_CLIENTTYPE_WINAIM41,
		  4, {0x01, 0x01, 0x01, 0x02}},

		/* AOL v6.0, CompuServe 2000 v6.0, any TOC client */
		{ AIM_CLIENTTYPE_AOL_TOC,
		  1, {0x01}},

		{ 0, 0}
	};
	int i;

	if (!msghdr || (len <= 0))
		return AIM_CLIENTTYPE_UNKNOWN;

	for (i = 0; fingerprints[i].len; i++) {
		if (fingerprints[i].len != len)
			continue;
		if (memcmp(fingerprints[i].data, msghdr, fingerprints[i].len) == 0)
			return fingerprints[i].clientid;
	}

	return AIM_CLIENTTYPE_UNKNOWN;
}

/* This should be endian-safe now... but who knows... */
faim_export fu16_t aim_iconsum(const fu8_t *buf, int buflen)
{
	fu32_t sum;
	int i;

	for (i = 0, sum = 0; i + 1 < buflen; i += 2)
		sum += (buf[i+1] << 8) + buf[i];
	if (i < buflen)
		sum += buf[i];

	sum = ((sum & 0xffff0000) >> 16) + (sum & 0x0000ffff);

	return (fu16_t)sum;
}

/*
 * Send an ICBM (instant message).  
 *
 *
 * Possible flags:
 *   AIM_IMFLAGS_AWAY  -- Marks the message as an autoresponse
 *   AIM_IMFLAGS_ACK   -- Requests that the server send an ack
 *                        when the message is received (of type 0x0004/0x000c)
 *   AIM_IMFLAGS_OFFLINE--If destination is offline, store it until they are
 *                        online (probably ICQ only).
 *   AIM_IMFLAGS_UNICODE--Instead of ASCII7, the passed message is
 *                        made up of UNICODE duples.  If you set
 *                        this, you'd better be damn sure you know
 *                        what you're doing.
 *   AIM_IMFLAGS_ISO_8859_1 -- The message contains the ASCII8 subset
 *                        known as ISO-8859-1.  
 *
 * Generally, you should use the lowest encoding possible to send
 * your message.  If you only use basic punctuation and the generic
 * Latin alphabet, use ASCII7 (no flags).  If you happen to use non-ASCII7
 * characters, but they are all clearly defined in ISO-8859-1, then 
 * use that.  Keep in mind that not all characters in the PC ASCII8
 * character set are defined in the ISO standard. For those cases (most
 * notably when the (r) symbol is used), you must use the full UNICODE
 * encoding for your message.  In UNICODE mode, _all_ characters must
 * occupy 16bits, including ones that are not special.  (Remember that
 * the first 128 UNICODE symbols are equivelent to ASCII7, however they
 * must be prefixed with a zero high order byte.)
 *
 * I strongly discourage the use of UNICODE mode, mainly because none
 * of the clients I use can parse those messages (and besides that,
 * wchars are difficult and non-portable to handle in most UNIX environments).
 * If you really need to include special characters, use the HTML UNICODE 
 * entities.  These are of the form &#2026; where 2026 is the hex 
 * representation of the UNICODE index (in this case, UNICODE 
 * "Horizontal Ellipsis", or 133 in in ASCII8).
 *
 * Implementation note:  Since this is one of the most-used functions
 * in all of libfaim, it is written with performance in mind.  As such,
 * it is not as clear as it could be in respect to how this message is
 * supposed to be layed out. Most obviously, tlvlists should be used 
 * instead of writing out the bytes manually. 
 *
 * XXX more precise verification that we never send SNACs larger than 8192
 * XXX check SNAC size for multipart
 *
 */
faim_export int aim_send_im_ext(aim_session_t *sess, struct aim_sendimext_args *args)
{
	static const fu8_t deffeatures[] = {
		0x01, 0x01, 0x01, 0x02
	};
	aim_conn_t *conn;
	int i, msgtlvlen;
	aim_frame_t *fr;
	aim_snacid_t snacid;

	if (!sess || !(conn = aim_conn_findbygroup(sess, 0x0004)))
		return -EINVAL;

	if (!args)
		return -EINVAL;

	if (args->flags & AIM_IMFLAGS_MULTIPART) {
		if (args->mpmsg->numparts <= 0)
			return -EINVAL;
	} else {
		if (!args->msg || (args->msglen <= 0))
			return -EINVAL;

		if (args->msglen >= MAXMSGLEN)
			return -E2BIG;
	}

	/* Painfully calculate the size of the message TLV */
	msgtlvlen = 1 + 1; /* 0501 */

	if (args->flags & AIM_IMFLAGS_CUSTOMFEATURES)
		msgtlvlen += 2 + args->featureslen;
	else
		msgtlvlen += 2 + sizeof(deffeatures);

	if (args->flags & AIM_IMFLAGS_MULTIPART) {
		aim_mpmsg_section_t *sec;

		for (sec = args->mpmsg->parts; sec; sec = sec->next) {
			msgtlvlen += 2 /* 0101 */ + 2 /* block len */;
			msgtlvlen += 4 /* charset */ + sec->datalen;
		}

	} else {
		msgtlvlen += 2 /* 0101 */ + 2 /* block len */;
		msgtlvlen += 4 /* charset */ + args->msglen;
	}


	if (!(fr = aim_tx_new(sess, conn, AIM_FRAMETYPE_FLAP, 0x02, msgtlvlen+128)))
		return -ENOMEM;

	/* XXX should be optional */	
	snacid = aim_cachesnac(sess, 0x0004, 0x0006, 0x0000, args->destsn, strlen(args->destsn)+1);
	aim_putsnac(&fr->data, 0x0004, 0x0006, 0x0000, snacid);

	/* 
	 * Generate a random message cookie 
	 *
	 * We could cache these like we do SNAC IDs.  (In fact, it 
	 * might be a good idea.)  In the message error functions, 
	 * the 8byte message cookie is returned as well as the 
	 * SNAC ID.
	 *
	 */
	for (i = 0; i < 8; i++)
		aimbs_put8(&fr->data, (fu8_t) rand());

	/*
	 * Channel ID
	 */
	aimbs_put16(&fr->data, 0x0001);

	/*
	 * Destination SN (prepended with byte length)
	 */
	aimbs_put8(&fr->data, strlen(args->destsn));
	aimbs_putraw(&fr->data, args->destsn, strlen(args->destsn));

	/*
	 * Message TLV (type 2).
	 */
	aimbs_put16(&fr->data, 0x0002);
	aimbs_put16(&fr->data, msgtlvlen);

	/*
	 * Features 
	 *
	 */
	aimbs_put8(&fr->data, 0x05);
	aimbs_put8(&fr->data, 0x01);

	if (args->flags & AIM_IMFLAGS_CUSTOMFEATURES) {
		aimbs_put16(&fr->data, args->featureslen);
		aimbs_putraw(&fr->data, args->features, args->featureslen);
	} else {
		aimbs_put16(&fr->data, sizeof(deffeatures));
		aimbs_putraw(&fr->data, deffeatures, sizeof(deffeatures));
	}

	if (args->flags & AIM_IMFLAGS_MULTIPART) {
		aim_mpmsg_section_t *sec;

		for (sec = args->mpmsg->parts; sec; sec = sec->next) {
			aimbs_put16(&fr->data, 0x0101);
			aimbs_put16(&fr->data, sec->datalen + 4);
			aimbs_put16(&fr->data, sec->charset);
			aimbs_put16(&fr->data, sec->charsubset);
			aimbs_putraw(&fr->data, sec->data, sec->datalen);
		}

	} else {

		aimbs_put16(&fr->data, 0x0101);

		/* 
		 * Message block length.
		 */
		aimbs_put16(&fr->data, args->msglen + 0x04);

		/*
		 * Character set.
		 */
		if (args->flags & AIM_IMFLAGS_CUSTOMCHARSET) {

			aimbs_put16(&fr->data, args->charset);
			aimbs_put16(&fr->data, args->charsubset);

		} else {
			if (args->flags & AIM_IMFLAGS_UNICODE)
				aimbs_put16(&fr->data, 0x0002);
			else if (args->flags & AIM_IMFLAGS_ISO_8859_1)
				aimbs_put16(&fr->data, 0x0003);
			else
				aimbs_put16(&fr->data, 0x0000);

			aimbs_put16(&fr->data, 0x0000);
		}

		/*
		 * Message.  Not terminated.
		 */
		aimbs_putraw(&fr->data, args->msg, args->msglen);
	}

	/*
	 * Set the Request Acknowledge flag.  
	 */
	if (args->flags & AIM_IMFLAGS_ACK) {
		aimbs_put16(&fr->data, 0x0003);
		aimbs_put16(&fr->data, 0x0000);
	}

	/*
	 * Set the Autoresponse flag.
	 */
	if (args->flags & AIM_IMFLAGS_AWAY) {
		aimbs_put16(&fr->data, 0x0004);
		aimbs_put16(&fr->data, 0x0000);
	}

	if (args->flags & AIM_IMFLAGS_OFFLINE) {
		aimbs_put16(&fr->data, 0x0006);
		aimbs_put16(&fr->data, 0x0000);
	}

	/*
	 * Set the I HAVE A REALLY PURTY ICON flag.
	 */
	if (args->flags & AIM_IMFLAGS_HASICON) {
		aimbs_put16(&fr->data, 0x0008);
		aimbs_put16(&fr->data, 0x000c);
		aimbs_put32(&fr->data, args->iconlen);
		aimbs_put16(&fr->data, 0x0001);
		aimbs_put16(&fr->data, args->iconsum);
		aimbs_put32(&fr->data, args->iconstamp);
	}

	/*
	 * Set the Buddy Icon Requested flag.
	 */
	if (args->flags & AIM_IMFLAGS_BUDDYREQ) {
		aimbs_put16(&fr->data, 0x0009);
		aimbs_put16(&fr->data, 0x0000);
	}

	aim_tx_enqueue(sess, fr);

	if (!(sess->flags & AIM_SESS_FLAGS_DONTTIMEOUTONICBM))
		aim_cleansnacs(sess, 60); /* clean out SNACs over 60sec old */

	return 0;
}

/*
 * Simple wrapper for aim_send_im_ext() 
 *
 * You cannot use aim_send_im if you need the HASICON flag.  You must
 * use aim_send_im_ext directly for that.
 *
 * aim_send_im also cannot be used if you require UNICODE messages, because
 * that requires an explicit message length.  Use aim_send_im_ext().
 *
 */
faim_export int aim_send_im(aim_session_t *sess, const char *destsn, fu16_t flags, const char *msg)
{
	struct aim_sendimext_args args;

	args.destsn = destsn;
	args.flags = flags;
	args.msg = msg;
	args.msglen = strlen(msg);

	/* Make these don't get set by accident -- they need aim_send_im_ext */
	args.flags &= ~(AIM_IMFLAGS_CUSTOMFEATURES | AIM_IMFLAGS_HASICON | AIM_IMFLAGS_MULTIPART);

	return aim_send_im_ext(sess, &args);
}

/*
 * This is also performance sensitive. (If you can believe it...)
 *
 */
faim_export int aim_send_icon(aim_session_t *sess, const char *sn, const fu8_t *icon, int iconlen, time_t stamp, fu16_t iconsum)
{
	aim_conn_t *conn;
	int i;
	fu8_t ck[8];
	aim_frame_t *fr;
	aim_snacid_t snacid;

	if (!sess || !(conn = aim_conn_findbygroup(sess, 0x0004)))
		return -EINVAL;

	if (!sn || !icon || (iconlen <= 0) || (iconlen >= MAXICONLEN))
		return -EINVAL;

	for (i = 0; i < 8; i++)
		aimutil_put8(ck+i, (fu8_t) rand());

	if (!(fr = aim_tx_new(sess, conn, AIM_FRAMETYPE_FLAP, 0x02, 10+8+2+1+strlen(sn)+2+2+2+8+16+2+2+2+2+2+2+2+4+4+4+iconlen+strlen(AIM_ICONIDENT)+2+2)))
		return -ENOMEM;

	snacid = aim_cachesnac(sess, 0x0004, 0x0006, 0x0000, NULL, 0);
	aim_putsnac(&fr->data, 0x0004, 0x0006, 0x0000, snacid);

	/*
	 * Cookie
	 */
	aimbs_putraw(&fr->data, ck, 8);

	/*
	 * Channel (2)
	 */
	aimbs_put16(&fr->data, 0x0002);

	/*
	 * Dest sn
	 */
	aimbs_put8(&fr->data, strlen(sn));
	aimbs_putraw(&fr->data, sn, strlen(sn));

	/*
	 * TLV t(0005)
	 *
	 * Encompasses everything below.
	 */
	aimbs_put16(&fr->data, 0x0005);
	aimbs_put16(&fr->data, 2+8+16+6+4+4+iconlen+4+4+4+strlen(AIM_ICONIDENT));

	aimbs_put16(&fr->data, 0x0000);
	aimbs_putraw(&fr->data, ck, 8);
	aim_putcap(&fr->data, AIM_CAPS_BUDDYICON);

	/* TLV t(000a) */
	aimbs_put16(&fr->data, 0x000a);
	aimbs_put16(&fr->data, 0x0002);
	aimbs_put16(&fr->data, 0x0001);

	/* TLV t(000f) */
	aimbs_put16(&fr->data, 0x000f);
	aimbs_put16(&fr->data, 0x0000);

	/* TLV t(2711) */
	aimbs_put16(&fr->data, 0x2711);
	aimbs_put16(&fr->data, 4+4+4+iconlen+strlen(AIM_ICONIDENT));
	aimbs_put16(&fr->data, 0x0000);
	aimbs_put16(&fr->data, iconsum);
	aimbs_put32(&fr->data, iconlen);
	aimbs_put32(&fr->data, stamp);
	aimbs_putraw(&fr->data, icon, iconlen);
	aimbs_putraw(&fr->data, AIM_ICONIDENT, strlen(AIM_ICONIDENT));

	/* TLV t(0003) */
	aimbs_put16(&fr->data, 0x0003);
	aimbs_put16(&fr->data, 0x0000);

	aim_tx_enqueue(sess, fr);

	return 0;
}

faim_internal int aim_request_directim(aim_session_t *sess, const char *destsn, fu8_t *ip, fu16_t port, fu8_t *ckret)
{
	aim_conn_t *conn;
	fu8_t ck[8];
	aim_frame_t *fr;
	aim_snacid_t snacid;
	aim_tlvlist_t *tl = NULL, *itl = NULL;
	int hdrlen, i;
	fu8_t *hdr;
	aim_bstream_t hdrbs;

	if (!sess || !(conn = aim_conn_findbygroup(sess, 0x0004)))
		return -EINVAL;

	if (!(fr = aim_tx_new(sess, conn, AIM_FRAMETYPE_FLAP, 0x02, 256+strlen(destsn))))
		return -ENOMEM;

	snacid = aim_cachesnac(sess, 0x0004, 0x0006, 0x0000, NULL, 0);
	aim_putsnac(&fr->data, 0x0004, 0x0006, 0x0000, snacid);

	/* 
	 * Generate a random message cookie 
	 *
	 * This cookie needs to be alphanumeric and NULL-terminated to be 
	 * TOC-compatible.
	 *
	 * XXX have I mentioned these should be generated in msgcookie.c?
	 *
	 */
	for (i = 0; i < 7; i++)
	       	ck[i] = 0x30 + ((fu8_t) rand() % 10);
	ck[7] = '\0';

	if (ckret)
		memcpy(ckret, ck, 8);

	/* Cookie */
	aimbs_putraw(&fr->data, ck, 8);

	/* Channel */
	aimbs_put16(&fr->data, 0x0002);

	/* Destination SN */
	aimbs_put8(&fr->data, strlen(destsn));
	aimbs_putraw(&fr->data, destsn, strlen(destsn));

	aim_addtlvtochain_noval(&tl, 0x0003);

	hdrlen = 2+8+16+6+8+6+4;
	hdr = malloc(hdrlen);
	aim_bstream_init(&hdrbs, hdr, hdrlen);

	aimbs_put16(&hdrbs, 0x0000);
	aimbs_putraw(&hdrbs, ck, 8);
	aim_putcap(&hdrbs, AIM_CAPS_IMIMAGE);

	aim_addtlvtochain16(&itl, 0x000a, 0x0001);
	aim_addtlvtochain_raw(&itl, 0x0003, 4, ip);
	aim_addtlvtochain16(&itl, 0x0005, port);
	aim_addtlvtochain_noval(&itl, 0x000f);
	
	aim_writetlvchain(&hdrbs, &itl);

	aim_addtlvtochain_raw(&tl, 0x0005, aim_bstream_curpos(&hdrbs), hdr);

	aim_writetlvchain(&fr->data, &tl);

	free(hdr);
	aim_freetlvchain(&itl);
	aim_freetlvchain(&tl);

	aim_tx_enqueue(sess, fr);

	return 0;
}

faim_internal int aim_request_sendfile(aim_session_t *sess, const char *sn, const char *filename, fu16_t numfiles, fu32_t totsize, fu8_t *ip, fu16_t port, fu8_t *ckret)
{
	aim_conn_t *conn;
	int i;
	fu8_t ck[8];
	aim_frame_t *fr;
	aim_snacid_t snacid;

	if (!sess || !(conn = aim_conn_findbygroup(sess, 0x0004)))
		return -EINVAL;

	if (!sn || !filename)
		return -EINVAL;

	if (!(fr = aim_tx_new(sess, conn, AIM_FRAMETYPE_FLAP, 0x02, 10+8+2+1+strlen(sn)+2+2+2+8+16+6+8+6+4+2+2+2+2+4+strlen(filename)+4)))
		return -ENOMEM;

	snacid = aim_cachesnac(sess, 0x0004, 0x0006, 0x0000, NULL, 0);
	aim_putsnac(&fr->data, 0x0004, 0x0006, 0x0000, snacid);

	for (i = 0; i < 7; i++)
		aimutil_put8(ck+i, 0x30 + ((fu8_t) rand() % 10));
	ck[7] = '\0';

	if (ckret)
		memcpy(ckret, ck, 8);

	/*
	 * Cookie
	 */
	aimbs_putraw(&fr->data, ck, 8);

	/*
	 * Channel (2)
	 */
	aimbs_put16(&fr->data, 0x0002);

	/*
	 * Dest sn
	 */
	aimbs_put8(&fr->data, strlen(sn));
	aimbs_putraw(&fr->data, sn, strlen(sn));

	/*
	 * TLV t(0005)
	 *
	 * Encompasses everything below. Gee.
	 */
	aimbs_put16(&fr->data, 0x0005);
	aimbs_put16(&fr->data, 2+8+16+6+8+6+4+2+2+2+2+4+strlen(filename)+4);

	aimbs_put16(&fr->data, 0x0000);
	aimbs_putraw(&fr->data, ck, 8);
	aim_putcap(&fr->data, AIM_CAPS_SENDFILE);

	/* TLV t(000a) */
	aimbs_put16(&fr->data, 0x000a);
	aimbs_put16(&fr->data, 0x0002);
	aimbs_put16(&fr->data, 0x0001);

	/* TLV t(0003) (IP) */
	aimbs_put16(&fr->data, 0x0003);
	aimbs_put16(&fr->data, 0x0004);
	aimbs_putraw(&fr->data, ip, 4);

	/* TLV t(0005) (port) */
	aimbs_put16(&fr->data, 0x0005);
	aimbs_put16(&fr->data, 0x0002);
	aimbs_put16(&fr->data, port);

	/* TLV t(000f) */
	aimbs_put16(&fr->data, 0x000f);
	aimbs_put16(&fr->data, 0x0000);

	/* TLV t(2711) */
	aimbs_put16(&fr->data, 0x2711);
	aimbs_put16(&fr->data, 2+2+4+strlen(filename)+4);

	/* ? */
	aimbs_put16(&fr->data, 0x0001);
	aimbs_put16(&fr->data, numfiles);
	aimbs_put32(&fr->data, totsize);
	aimbs_putraw(&fr->data, filename, strlen(filename));

	/* ? */
	aimbs_put32(&fr->data, 0x00000000);

	aim_tx_enqueue(sess, fr);

	return 0;
}

static int outgoingim(aim_session_t *sess, aim_module_t *mod, aim_frame_t *rx, aim_modsnac_t *snac, aim_bstream_t *bs)
{
	int i, ret = 0;
	aim_rxcallback_t userfunc;
	fu8_t cookie[8];
	fu16_t channel;
	aim_tlvlist_t *tlvlist;
	char *sn;
	int snlen;
	fu16_t icbmflags = 0;
	fu8_t flag1 = 0, flag2 = 0;
	fu8_t *msg = NULL;
	aim_tlv_t *msgblock;

	/* ICBM Cookie. */
	for (i = 0; i < 8; i++)
		cookie[i] = aimbs_get8(bs);

	/* Channel ID */
	channel = aimbs_get16(bs);

	if (channel != 0x01) {
		faimdprintf(sess, 0, "icbm: ICBM recieved on unsupported channel.  Ignoring. (chan = %04x)\n", channel);
		return 0;
	}

	snlen = aimbs_get8(bs);
	sn = aimbs_getstr(bs, snlen);

	tlvlist = aim_readtlvchain(bs);

	if (aim_gettlv(tlvlist, 0x0003, 1))
		icbmflags |= AIM_IMFLAGS_ACK;
	if (aim_gettlv(tlvlist, 0x0004, 1))
		icbmflags |= AIM_IMFLAGS_AWAY;

	if ((msgblock = aim_gettlv(tlvlist, 0x0002, 1))) {
		aim_bstream_t mbs;
		int featurelen, msglen;

		aim_bstream_init(&mbs, msgblock->value, msgblock->length);

		aimbs_get8(&mbs);
		aimbs_get8(&mbs);
		for (featurelen = aimbs_get16(&mbs); featurelen; featurelen--)
			aimbs_get8(&mbs);
		aimbs_get8(&mbs);
		aimbs_get8(&mbs);

		msglen = aimbs_get16(&mbs) - 4; /* final block length */

		flag1 = aimbs_get16(&mbs);
		flag2 = aimbs_get16(&mbs);

		msg = aimbs_getstr(&mbs, msglen);
	}

	if ((userfunc = aim_callhandler(sess, rx->conn, snac->family, snac->subtype)))
		ret = userfunc(sess, rx, channel, sn, msg, icbmflags, flag1, flag2);

	free(sn);
	aim_freetlvchain(&tlvlist);

	return ret;
}

/*
 * Ahh, the joys of nearly ridiculous over-engineering.
 *
 * Not only do AIM ICBM's support multiple channels.  Not only do they
 * support multiple character sets.  But they support multiple character 
 * sets / encodings within the same ICBM.
 *
 * These multipart messages allow for complex space savings techniques, which
 * seem utterly unnecessary by today's standards.  In fact, there is only
 * one client still in popular use that still uses this method: AOL for the
 * Macintosh, Version 5.0.  Obscure, yes, I know.  
 *
 * In modern (non-"legacy") clients, if the user tries to send a character
 * that is not ISO-8859-1 or ASCII, the client will send the entire message
 * as UNICODE, meaning that every character in the message will occupy the
 * full 16 bit UNICODE field, even if the high order byte would be zero.
 * Multipart messages prevent this wasted space by allowing the client to
 * only send the characters in UNICODE that need to be sent that way, and
 * the rest of the message can be sent in whatever the native character 
 * set is (probably ASCII).
 *
 * An important note is that sections will be displayed in the order that
 * they appear in the ICBM.  There is no facility for merging or rearranging
 * sections at run time.  So if you have, say, ASCII then UNICODE then ASCII,
 * you must supply two ASCII sections with a UNICODE in the middle, and incur
 * the associated overhead.
 *
 * Normally I would have laughed and given a firm 'no' to supporting this
 * seldom-used feature, but something is attracting me to it.  In the future,
 * it may be possible to abuse this to send mixed-media messages to other
 * open source clients (like encryption or something) -- see faimtest for
 * examples of how to do this.
 *
 * I would definitly recommend avoiding this feature unless you really
 * know what you are doing, and/or you have something neat to do with it.
 *
 */
faim_export int aim_mpmsg_init(aim_session_t *sess, aim_mpmsg_t *mpm)
{

	memset(mpm, 0, sizeof(aim_mpmsg_t));

	return 0;
}

static int mpmsg_addsection(aim_session_t *sess, aim_mpmsg_t *mpm, fu16_t charset, fu16_t charsubset, fu8_t *data, fu16_t datalen)
{
	aim_mpmsg_section_t *sec; 
	
	if (!(sec = malloc(sizeof(aim_mpmsg_section_t))))
		return -1;

	sec->charset = charset;
	sec->charsubset = charsubset;
	sec->data = data;
	sec->datalen = datalen;
	sec->next = NULL;

	if (!mpm->parts)
		mpm->parts = sec;
	else {
		aim_mpmsg_section_t *cur;

		for (cur = mpm->parts; cur->next; cur = cur->next)
			;
		cur->next = sec;
	}

	mpm->numparts++;

	return 0;
}

faim_export int aim_mpmsg_addraw(aim_session_t *sess, aim_mpmsg_t *mpm, fu16_t charset, fu16_t charsubset, const fu8_t *data, fu16_t datalen)
{
	fu8_t *dup;

	if (!(dup = malloc(datalen)))
		return -1;
	memcpy(dup, data, datalen);

	if (mpmsg_addsection(sess, mpm, charset, charsubset, dup, datalen) == -1) {
		free(dup);
		return -1;
	}

	return 0;
}

/* XXX should provide a way of saying ISO-8859-1 specifically */
faim_export int aim_mpmsg_addascii(aim_session_t *sess, aim_mpmsg_t *mpm, const char *ascii)
{
	fu8_t *dup;

	if (!(dup = strdup(ascii))) 
		return -1;

	if (mpmsg_addsection(sess, mpm, 0x0000, 0x0000, dup, strlen(ascii)) == -1) {
		free(dup);
		return -1;
	}

	return 0;
}

faim_export int aim_mpmsg_addunicode(aim_session_t *sess, aim_mpmsg_t *mpm, const fu16_t *unicode, fu16_t unicodelen)
{
	fu8_t *buf;
	aim_bstream_t bs;
	int i;

	if (!(buf = malloc(unicodelen * 2)))
		return -1;

	aim_bstream_init(&bs, buf, unicodelen * 2);

	/* We assume unicode is in /host/ byte order -- convert to network */
	for (i = 0; i < unicodelen; i++)
		aimbs_put16(&bs, unicode[i]);

	if (mpmsg_addsection(sess, mpm, 0x0002, 0x0000, buf, aim_bstream_curpos(&bs)) == -1) {
		free(buf);
		return -1;
	}
	
	return 0;
}

faim_export void aim_mpmsg_free(aim_session_t *sess, aim_mpmsg_t *mpm)
{
	aim_mpmsg_section_t *cur;

	for (cur = mpm->parts; cur; ) {
		aim_mpmsg_section_t *tmp;
		
		tmp = cur->next;
		free(cur->data);
		free(cur);
		cur = tmp;
	}
	
	mpm->numparts = 0;
	mpm->parts = NULL;

	return;
}

/*
 * Start by building the multipart structures, then pick the first 
 * human-readable section and stuff it into args->msg so no one gets
 * suspicious.
 *
 */
static int incomingim_ch1_parsemsgs(aim_session_t *sess, fu8_t *data, int len, struct aim_incomingim_ch1_args *args)
{
	static const fu16_t charsetpri[] = {
		0x0000, /* ASCII first */
		0x0003, /* then ISO-8859-1 */
		0x0002, /* UNICODE as last resort */
	};
	static const int charsetpricount = 3;
	int i;
	aim_bstream_t mbs;
	aim_mpmsg_section_t *sec;

	aim_bstream_init(&mbs, data, len);

	while (aim_bstream_empty(&mbs)) {
		fu16_t msglen, flag1, flag2;
		fu8_t *msgbuf;

		aimbs_get8(&mbs); /* 01 */
		aimbs_get8(&mbs); /* 01 */

		/* Message string length, including character set info. */
		msglen = aimbs_get16(&mbs);

		/* Character set info */
		flag1 = aimbs_get16(&mbs);
		flag2 = aimbs_get16(&mbs);

		/* Message. */
		msglen -= 4;

		/*
		 * For now, we don't care what the encoding is.  Just copy
		 * it into a multipart struct and deal with it later. However,
		 * always pad the ending with a NULL.  This makes it easier
		 * to treat ASCII sections as strings.  It won't matter for
		 * UNICODE or binary data, as you should never read past
		 * the specified data length, which will not include the pad.
		 *
		 * XXX There's an API bug here.  For sending, the UNICODE is
		 * given in host byte order (aim_mpmsg_addunicode), but here
		 * the received messages are given in network byte order.
		 *
		 */
		msgbuf = aimbs_getstr(&mbs, msglen);
		mpmsg_addsection(sess, &args->mpmsg, flag1, flag2, msgbuf, msglen);

	} /* while */

	args->icbmflags |= AIM_IMFLAGS_MULTIPART; /* always set */

	/*
	 * Clients that support multiparts should never use args->msg, as it
	 * will point to an arbitrary section.
	 *
	 * Here, we attempt to provide clients that do not support multipart
	 * messages with something to look at -- hopefully a human-readable
	 * string.  But, failing that, a UNICODE message, or nothing at all.
	 *
	 * Which means that even if args->msg is NULL, it does not mean the
	 * message was blank.
	 *
	 */
	for (i = 0; i < charsetpricount; i++) {
		for (sec = args->mpmsg.parts; sec; sec = sec->next) {

			if (sec->charset != charsetpri[i])
				continue;

			/* Great. We found one.  Fill it in. */
			args->charset = sec->charset;
			args->charsubset = sec->charsubset;
			args->icbmflags |= AIM_IMFLAGS_CUSTOMCHARSET;

			/* Set up the simple flags */
			if (args->charset == 0x0000)
				; /* ASCII */
			else if (args->charset == 0x0002)
				args->icbmflags |= AIM_IMFLAGS_UNICODE;
			else if (args->charset == 0x0003)
				args->icbmflags |= AIM_IMFLAGS_ISO_8859_1;
			else if (args->charset == 0xffff)
				; /* no encoding (yeep!) */

			if (args->charsubset == 0x0000)
				; /* standard subencoding? */
			else if (args->charsubset == 0x000b)
				args->icbmflags |= AIM_IMFLAGS_SUBENC_MACINTOSH;
			else if (args->charsubset == 0xffff)
				; /* no subencoding */
#if 0
			/* XXX this isn't really necesary... */	
			if (	((args.flag1 != 0x0000) &&
				 (args.flag1 != 0x0002) &&
				 (args.flag1 != 0x0003) &&
				 (args.flag1 != 0xffff)) ||
				((args.flag2 != 0x0000) &&
				 (args.flag2 != 0x000b) &&
				 (args.flag2 != 0xffff))) {
				faimdprintf(sess, 0, "icbm: **warning: encoding flags are being used! {%04x, %04x}\n", args.flag1, args.flag2);
			}
#endif

			args->msg = sec->data;
			args->msglen = sec->datalen;

			return 0;
		}
	}

	/* No human-readable sections found.  Oh well. */
	args->charset = args->charsubset = 0xffff;
	args->msg = NULL;
	args->msglen = 0;

	return 0;
}

static int incomingim_ch1(aim_session_t *sess, aim_module_t *mod, aim_frame_t *rx, aim_modsnac_t *snac, fu16_t channel, aim_userinfo_t *userinfo, aim_bstream_t *bs, fu8_t *cookie)
{
	fu16_t type, length;
	aim_rxcallback_t userfunc;
	int ret = 0;
	struct aim_incomingim_ch1_args args;
	int endpos;

	memset(&args, 0, sizeof(args));

	aim_mpmsg_init(sess, &args.mpmsg);

	/*
	 * This used to be done using tlvchains.  For performance reasons,
	 * I've changed it to process the TLVs in-place.  This avoids lots
	 * of per-IM memory allocations.
	 */
	while (aim_bstream_empty(bs)) {

		type = aimbs_get16(bs);
		length = aimbs_get16(bs);

		endpos = aim_bstream_curpos(bs) + length;

		if (type == 0x0002) { /* Message Block */

			/*
			 * This TLV consists of the following:
			 *   - 0501 -- Unknown
			 *   - Features: Don't know how to interpret these
			 *   - 0101 -- Unknown
			 *   - Message
			 *
			 */

			aimbs_get8(bs); /* 05 */
			aimbs_get8(bs); /* 01 */

			args.featureslen = aimbs_get16(bs);
			/* XXX XXX this is all evil! */
			args.features = bs->data + bs->offset;
			aim_bstream_advance(bs, args.featureslen);
			args.icbmflags |= AIM_IMFLAGS_CUSTOMFEATURES;

			/*
			 * The rest of the TLV contains one or more message
			 * blocks...
			 */
			incomingim_ch1_parsemsgs(sess, bs->data + bs->offset /* XXX evil!!! */, length - 2 - 2 - args.featureslen, &args);

		} else if (type == 0x0003) { /* Server Ack Requested */

			args.icbmflags |= AIM_IMFLAGS_ACK;

		} else if (type == 0x0004) { /* Message is Auto Response */

			args.icbmflags |= AIM_IMFLAGS_AWAY;

		} else if (type == 0x0006) { /* Message was received offline. */

			/* XXX not sure if this actually gets sent. */
			args.icbmflags |= AIM_IMFLAGS_OFFLINE;

		} else if (type == 0x0008) { /* I-HAVE-A-REALLY-PURTY-ICON Flag */

			args.iconlen = aimbs_get32(bs);
			aimbs_get16(bs); /* 0x0001 */
			args.iconsum = aimbs_get16(bs);
			args.iconstamp = aimbs_get32(bs);
			args.icbmflags |= AIM_IMFLAGS_HASICON;

		} else if (type == 0x0009) {

			args.icbmflags |= AIM_IMFLAGS_BUDDYREQ;

		} else if (type == 0x0017) {

			args.extdatalen = length;
			args.extdata = aimbs_getraw(bs, args.extdatalen);

		} else {
			faimdprintf(sess, 0, "incomingim_ch1: unknown TLV 0x%04x (len %d)\n", type, length);
		}

		/*
		 * This is here to protect ourselves from ourselves.  That
		 * is, if something above doesn't completly parse its value
		 * section, or, worse, overparses it, this will set the
		 * stream where it needs to be in order to land on the next
		 * TLV when the loop continues.
		 *
		 */
		aim_bstream_setpos(bs, endpos);
	}


	if ((userfunc = aim_callhandler(sess, rx->conn, snac->family, snac->subtype)))
		ret = userfunc(sess, rx, channel, userinfo, &args);

	aim_mpmsg_free(sess, &args.mpmsg);
	free(args.extdata);

	return ret;
}

static int incomingim_ch2_buddylist(aim_session_t *sess, aim_module_t *mod, aim_frame_t *rx, aim_modsnac_t *snac, aim_userinfo_t *userinfo, struct aim_incomingim_ch2_args *args, aim_tlvlist_t *list2)
{
	aim_rxcallback_t userfunc;
	int ret = 0;
	aim_tlv_t *tse;
	aim_bstream_t tbs;

	if (args->status != 0x0000)
		return 1; /* ignore it -- not sure what it means */

	tse = aim_gettlv(list2, 0x2711, 1);
	aim_bstream_init(&tbs, tse->value, tse->length);

	/*
	 * This goes like this...
	 *
	 *   group name length
	 *   group name
	 *     num of buddies in group
	 *     buddy name length
	 *     buddy name
	 *     buddy name length
	 *     buddy name
	 *     ...
	 *   group name length
	 *   group name
	 *     num of buddies in group
	 *     buddy name length
	 *     buddy name
	 *     ...
	 *   ...
	 */
	while (aim_bstream_empty(&tbs)) {
		fu16_t gnlen, numb;
		int i;
		char *gn;

		gnlen = aimbs_get16(&tbs);
		gn = aimbs_getstr(&tbs, gnlen);
		numb = aimbs_get16(&tbs);

		for (i = 0; i < numb; i++) {
			fu16_t bnlen;
			char *bn;

			bnlen = aimbs_get16(&tbs);
			bn = aimbs_getstr(&tbs, bnlen);

			faimdprintf(sess, 0, "got a buddy list from %s: group %s, buddy %s\n", userinfo->sn, gn, bn);

			free(bn);
		}

		free(gn);
	}

	if ((userfunc = aim_callhandler(sess, rx->conn, snac->family, snac->subtype)))
		ret = userfunc(sess, rx, 0x0002, userinfo, args);

	return ret;
}

static int incomingim_ch2_buddyicon(aim_session_t *sess, aim_module_t *mod, aim_frame_t *rx, aim_modsnac_t *snac, aim_userinfo_t *userinfo, struct aim_incomingim_ch2_args *args, aim_tlvlist_t *list2)
{
	aim_rxcallback_t userfunc;
	int ret = 0;
	aim_tlv_t *miscinfo;
	aim_bstream_t tbs;

	if (!(miscinfo = aim_gettlv(list2, 0x2711, 1)))
		return 0;
	aim_bstream_init(&tbs, miscinfo->value, miscinfo->length);

	args->info.icon.checksum = aimbs_get32(&tbs);
	args->info.icon.length = aimbs_get32(&tbs);
	args->info.icon.timestamp = aimbs_get32(&tbs);
	args->info.icon.icon = aimbs_getraw(&tbs, args->info.icon.length);

	if ((userfunc = aim_callhandler(sess, rx->conn, snac->family, snac->subtype)))
		ret = userfunc(sess, rx, 0x0002, userinfo, args);

	free(args->info.icon.icon);

	return ret;
}

static int incomingim_ch2_voice(aim_session_t *sess, aim_module_t *mod, aim_frame_t *rx, aim_modsnac_t *snac, aim_userinfo_t *userinfo, struct aim_incomingim_ch2_args *args, aim_tlvlist_t *list2)
{
	aim_msgcookie_t *cachedcook;
	int ret = 0;
	aim_rxcallback_t userfunc;

	faimdprintf(sess, 1, "rend: voice!\n");

	if (!(cachedcook = (aim_msgcookie_t*)calloc(1, sizeof(aim_msgcookie_t))))
		return 0;

	memcpy(cachedcook->cookie, args->cookie, 8);
	cachedcook->type = AIM_COOKIETYPE_OFTVOICE;
	cachedcook->data = NULL;

	if (aim_cachecookie(sess, cachedcook) == -1)
		faimdprintf(sess, 0, "ERROR caching message cookie\n");

	/* XXX: implement all this */

	if ((userfunc = aim_callhandler(sess, rx->conn, snac->family, snac->subtype))) 
		ret = userfunc(sess, rx, 0x0002, userinfo, args);

	return ret;
}

static int incomingim_ch2_chat(aim_session_t *sess, aim_module_t *mod, aim_frame_t *rx, aim_modsnac_t *snac, aim_userinfo_t *userinfo, struct aim_incomingim_ch2_args *args, aim_tlvlist_t *list2)
{
	aim_tlv_t *miscinfo;
	aim_bstream_t tbs;
	aim_rxcallback_t userfunc;
	int ret = 0;

	miscinfo = aim_gettlv(list2, 0x2711, 1);
	aim_bstream_init(&tbs, miscinfo->value, miscinfo->length);

	aim_chat_readroominfo(&tbs, &args->info.chat.roominfo);
		  
	if (aim_gettlv(list2, 0x000c, 1))
		args->info.chat.msg = aim_gettlv_str(list2, 0x000c, 1);
	
	if (aim_gettlv(list2, 0x000d, 1))
		args->info.chat.encoding = aim_gettlv_str(list2, 0x000d, 1);
	
	if (aim_gettlv(list2, 0x000e, 1))
		args->info.chat.lang = aim_gettlv_str(list2, 0x000e, 1);

	if ((userfunc = aim_callhandler(sess, rx->conn, snac->family, snac->subtype)))
		ret = userfunc(sess, rx, 0x0002, userinfo, args);

	/* XXX free_roominfo */
	free(args->info.chat.roominfo.name);
	free(args->info.chat.msg);
	free(args->info.chat.encoding);
	free(args->info.chat.lang);

	return ret;
}

static int incomingim_ch2_getfile(aim_session_t *sess, aim_module_t *mod, aim_frame_t *rx, aim_modsnac_t *snac, aim_userinfo_t *userinfo, struct aim_incomingim_ch2_args *args, aim_tlvlist_t *list2)
{
	char ip[30];
	aim_msgcookie_t *cachedcook;
	aim_tlv_t *miscinfo;
	aim_tlv_t *iptlv, *porttlv;
	int ret = 0;
	aim_rxcallback_t userfunc;

	memset(ip, 0, 30);

	if (!(cachedcook = calloc(1, sizeof(aim_msgcookie_t)))) {
		aim_freetlvchain(&list2);
		return 0;
	}

	if (!(miscinfo = aim_gettlv(list2, 0x2711, 1)) || 
		!(iptlv = aim_gettlv(list2, 0x0003, 1)) || 
		!(porttlv = aim_gettlv(list2, 0x0005, 1))) {
		
		faimdprintf(sess, 0, "rend: badly damaged file get request from %s...\n", userinfo->sn);
		aim_cookie_free(sess, cachedcook);
		aim_freetlvchain(&list2);
		
		return 0;
	}

	snprintf(ip, 30, "%d.%d.%d.%d:%d",
		aimutil_get8(iptlv->value+0),
		aimutil_get8(iptlv->value+1),
		aimutil_get8(iptlv->value+2),
		aimutil_get8(iptlv->value+3),
		aimutil_get16(porttlv->value));

	faimdprintf(sess, 0, "rend: file get request from %s (%s)\n", userinfo->sn, ip);

	args->info.getfile.ip = ip;
	memcpy(args->info.getfile.cookie, args->cookie, 8);

	if ((userfunc = aim_callhandler(sess, rx->conn, snac->family, snac->subtype)))
		ret = userfunc(sess, rx, 0x0002, userinfo, args);

	return ret;
}

static int incomingim_ch2_sendfile(aim_session_t *sess, aim_module_t *mod, aim_frame_t *rx, aim_modsnac_t *snac, aim_userinfo_t *userinfo, struct aim_incomingim_ch2_args *args, aim_tlvlist_t *list2)
{
#if 0
	char ip[30];
	aim_msgcookie_t *cachedcook;
	aim_tlv_t *miscinfo;
	aim_tlv_t *iptlv, *porttlv;
	int ret =0;
	aim_rxcallback_t userfunc;
	char *desc = NULL;

	memset(ip, 0, 30);

	if (!(cachedcook = calloc(1, sizeof(aim_msgcookie_t)))) 
		return 0;

	if (!(miscinfo = aim_gettlv(list2, 0x2711, 1)) || 
		!(iptlv = aim_gettlv(list2, 0x0003, 1)) || 
		!(porttlv = aim_gettlv(list2, 0x0005, 1))) {
	
		faimdprintf(sess, 0, "rend: badly damaged file get request from %s...\n", userinfo->sn);
		aim_cookie_free(sess, cachedcook);

		return 0;
	}

	snprintf(ip, 30, "%d.%d.%d.%d:%d",
		aimutil_get8(iptlv->value+0),
		aimutil_get8(iptlv->value+1),
		aimutil_get8(iptlv->value+2),
		aimutil_get8(iptlv->value+3),
		aimutil_get16(porttlv->value));

	if (aim_gettlv(list2, 0x000c, 1))
		desc = aim_gettlv_str(list2, 0x000c, 1);

	faimdprintf(sess, 0, "rend: file transfer request from %s: %s (%s)\n",
		userinfo->sn, desc, ip);

	memcpy(cachedcook->cookie, args->cookie, 8);

	ft = malloc(sizeof(struct aim_filetransfer_priv)); /* XXX */
	strncpy(ft->sn, userinfo.sn, sizeof(ft->sn));
	strncpy(ft->ip, ip, sizeof(ft->ip));
	strncpy(ft->fh.name, miscinfo->value+8, sizeof(ft->fh.name));
	cachedcook->type = AIM_COOKIETYPE_OFTSEND;
	cachedcook->data = ft;

	if (aim_cachecookie(sess, cachedcook) == -1)
		faimdprintf(sess, 0, "ERROR caching message cookie\n");

	aim_accepttransfer(sess, rx->conn, ft->sn, cookie, AIM_CAPS_SENDFILE);

	if (desc)
		free(desc);

	if ((userfunc = aim_callhandler(sess, rx->conn, snac->family, snac->subtype)))
		ret = userfunc(sess, rx, 0x0002, userinfo, &args);
#endif
	return 0;
}

static int incomingim_ch2_imimage(aim_session_t *sess, aim_module_t *mod, aim_frame_t *rx, aim_modsnac_t *snac, aim_userinfo_t *userinfo, struct aim_incomingim_ch2_args *args, aim_tlvlist_t *list2)
{
	aim_rxcallback_t userfunc;
	int ret = 0;

	/* Primary IP address */
	if (aim_gettlv(list2, 0x0003, 1)) {
		aim_tlv_t *tlv;

		tlv = aim_gettlv(list2, 0x0003, 1);

		snprintf(args->info.imimage.ip, sizeof(args->info.imimage.ip),
					"%d.%d.%d.%d:4443",
					tlv->value[0],
					tlv->value[1],
					tlv->value[2],
					tlv->value[3]);
	}

	/* 
	 * Alternate IP address
	 *
	 * Sort of.  The peer doesn't send this -- the OSCAR
	 * server does.  So it will be the IP address that the
	 * peer is directly connected to the internet with, which 
	 * may not be the same as the IP above.  If these two
	 * values differ, it's rather unlikely that this
	 * rendezvous is going to happen...
	 *
	 */
	if (aim_gettlv(list2, 0x0004, 1))
		;
		
	/* Port number (not correct -- ignore) */
	if (aim_gettlv(list2, 0x0005, 1)) 
		;

	/* Unknown -- two bytes = 0x0001 */
	if (aim_gettlv(list2, 0x000a, 1))
		;

	/* Unknown -- no value */
	if (aim_gettlv(list2, 0x000f, 1))
		;

	faimdprintf(sess, 1, "rend: directIM request from %s (%s)\n", userinfo->sn, args->info.imimage.ip);

	/* 
	 * XXX: there are a couple of different request packets for
	 *          different things 
	 */

	if ((userfunc = aim_callhandler(sess, rx->conn, snac->family, snac->subtype)))
		ret = userfunc(sess, rx, 0x0002, userinfo, args);

	return ret;
}

/* XXX Ugh.  I think its obvious. */
static int incomingim_ch2(aim_session_t *sess, aim_module_t *mod, aim_frame_t *rx, aim_modsnac_t *snac, fu16_t channel, aim_userinfo_t *userinfo, aim_tlvlist_t *tlvlist, fu8_t *cookie)
{
	aim_tlv_t *block1;
	aim_tlvlist_t *list2;
	int ret = 0;
	struct aim_incomingim_ch2_args args;
	aim_bstream_t bbs;
	fu8_t *cookie2;

	memset(&args, 0, sizeof(args));

	/*
	 * There's another block of TLVs embedded in the type 5 here. 
	 */
	if (!(block1 = aim_gettlv(tlvlist, 0x0005, 1)) || !block1->value) {
		faimdprintf(sess, 0, "no tlv 0x0005 in rendezvous transaction!\n");
		return 0;
	}

	aim_bstream_init(&bbs, block1->value, block1->length);

	/*
	 * First two bytes represent the status of the connection.
	 *
	 * 0 is a request, 2 is an accept
	 */ 
	args.status = aimbs_get16(&bbs);

	/*
	 * Next comes the cookie.  Should match the ICBM cookie.
	 */
	cookie2 = aimbs_getraw(&bbs, 8);
	if (memcmp(cookie, cookie2, 8) != 0) 
		faimdprintf(sess, 0, "rend: warning cookies don't match!\n");
	memcpy(args.cookie, cookie2, 8);
	free(cookie2);

	/*
	 * The next 16bytes are a capability block so we can
	 * identify what type of rendezvous this is.
	 *
	 * Thanks to Eric Warmenhoven <warmenhoven@linux.com> (of GAIM)
	 * for pointing some of this out to me.  In fact, a lot of 
	 * the client-to-client info comes from the work of the GAIM 
	 * developers. Thanks!
	 *
	 * Read off one capability string and we should have it ID'd.
	 * 
	 */
	if ((args.reqclass = aim_getcap(sess, &bbs, 0x10)) == 0x0000) {
		faimdprintf(sess, 0, "rend: no ID block\n");
		return 0;
	}

	/* 
	 * What follows may be TLVs or nothing, depending on the
	 * purpose of the message.
	 *
	 * Ack packets for instance have nothing more to them.
	 */
	list2 = aim_readtlvchain(&bbs);

#if 0 /* this should be in the per-type blocks */
	if (!list2 || ((args.reqclass != AIM_CAPS_IMIMAGE) && !(aim_gettlv(list2, 0x2711, 1)))) {
		aim_msgcookie_t *cook;
		int type;

		type = aim_msgcookie_gettype(args.reqclass); /* XXX: fix this shitty code */

		if ((cook = aim_checkcookie(sess, cookie, type)) == NULL) {
			faimdprintf(sess, 0, "non-data rendezvous thats not in cache (type %d)\n", type);
			aim_freetlvchain(&list2);
			return 1;
		}

		if (cook->type == AIM_COOKIETYPE_OFTGET) {
			struct aim_filetransfer_priv *ft;

			if (cook->data) {
				int errorcode = -1; /* XXX shouldnt this be 0? */

				ft = (struct aim_filetransfer_priv *)cook->data;

				if (args.status != 0x0002) {

					if (aim_gettlv(list2, 0x000b, 1))
						errorcode = aim_gettlv16(list2, 0x000b, 1);

					/* XXX this should make it up to the client, you know.. */
					if (errorcode)
						faimdprintf(sess, 0, "transfer from %s (%s) for %s cancelled (error code %d)\n", ft->sn, ft->ip, ft->fh.name, errorcode);
				} /* args.status != 0x0002 */

			} else {
				faimdprintf(sess, 0, "no data attached to file transfer\n");
			} /* !cook->data */

		} else if (cook->type == AIM_CAPS_VOICE) {

			faimdprintf(sess, 0, "voice request cancelled\n");
		
		} else {
		
			faimdprintf(sess, 0, "unknown cookie cache type %d\n", cook->type);
		}

		aim_freetlvchain(&list2);

		return 1;
	}
#endif

	/*
	 * The rest of the handling depends on what type it is.
	 */
	if (args.reqclass & AIM_CAPS_BUDDYICON)
		ret = incomingim_ch2_buddyicon(sess, mod, rx, snac, userinfo, &args, list2);
	else if (args.reqclass & AIM_CAPS_SENDBUDDYLIST)
		ret = incomingim_ch2_buddylist(sess, mod, rx, snac, userinfo, &args, list2);
	else if (args.reqclass & AIM_CAPS_VOICE)
		ret = incomingim_ch2_voice(sess, mod, rx, snac, userinfo, &args, list2);
	else if (args.reqclass & AIM_CAPS_IMIMAGE)
		ret = incomingim_ch2_imimage(sess, mod, rx, snac, userinfo, &args, list2);
	else if (args.reqclass & AIM_CAPS_CHAT)
		ret = incomingim_ch2_chat(sess, mod, rx, snac, userinfo, &args, list2);
	else if (args.reqclass & AIM_CAPS_GETFILE)
		ret = incomingim_ch2_getfile(sess, mod, rx, snac, userinfo, &args, list2);
	else if (args.reqclass & AIM_CAPS_SENDFILE)
		ret = incomingim_ch2_sendfile(sess, mod, rx, snac, userinfo, &args, list2);
	else
		faimdprintf(sess, 0, "rend: unknown rendezvous 0x%04x\n", args.reqclass);

	aim_freetlvchain(&list2);

	return ret;
}

/*
 * It can easily be said that parsing ICBMs is THE single
 * most difficult thing to do in the in AIM protocol.  In
 * fact, I think I just did say that.
 *
 * Below is the best damned solution I've come up with
 * over the past sixteen months of battling with it. This
 * can parse both away and normal messages from every client
 * I have access to.  Its not fast, its not clean.  But it works.
 *
 */
static int incomingim(aim_session_t *sess, aim_module_t *mod, aim_frame_t *rx, aim_modsnac_t *snac, aim_bstream_t *bs)
{
	int i, ret = 0;
	fu8_t cookie[8];
	fu16_t channel;
	aim_userinfo_t userinfo;

	memset(&userinfo, 0x00, sizeof(aim_userinfo_t));

	/*
	 * Read ICBM Cookie.  And throw away.
	 */
	for (i = 0; i < 8; i++)
		cookie[i] = aimbs_get8(bs);

	/*
	 * Channel ID.
	 *
	 * Channel 0x0001 is the message channel.  There are 
	 * other channels for things called "rendevous"
	 * which represent chat and some of the other new
	 * features of AIM2/3/3.5. 
	 *
	 * Channel 0x0002 is the Rendevous channel, which
	 * is where Chat Invitiations and various client-client
	 * connection negotiations come from.
	 * 
	 */
	channel = aimbs_get16(bs);

	/*
	 * Extract the standard user info block.
	 *
	 * Note that although this contains TLVs that appear contiguous
	 * with the TLVs read below, they are two different pieces.  The
	 * userinfo block contains the number of TLVs that contain user
	 * information, the rest are not even though there is no seperation.
	 * aim_extractuserinfo() returns the number of bytes used by the
	 * userinfo tlvs, so you can start reading the rest of them right
	 * afterward.  
	 *
	 * That also means that TLV types can be duplicated between the
	 * userinfo block and the rest of the message, however there should
	 * never be two TLVs of the same type in one block.
	 * 
	 */
	aim_extractuserinfo(sess, bs, &userinfo);

	/*
	 * From here on, its depends on what channel we're on.
	 *
	 * Technically all channels have a TLV list have this, however,
	 * for the common channel 1 case, in-place parsing is used for
	 * performance reasons (less memory allocation).
	 */
	if (channel == 1) {

		ret = incomingim_ch1(sess, mod, rx, snac, channel, &userinfo, bs, cookie);

	} else if (channel == 0x0002) {
		aim_tlvlist_t *tlvlist;

		/*
		 * Read block of TLVs (not including the userinfo data).  All 
		 * further data is derived from what is parsed here.
		 */
		tlvlist = aim_readtlvchain(bs);

		ret = incomingim_ch2(sess, mod, rx, snac, channel, &userinfo, tlvlist, cookie);

		/*
		 * Free up the TLV chain.
		 */
		aim_freetlvchain(&tlvlist);
	} else {
		faimdprintf(sess, 0, "icbm: ICBM received on an unsupported channel.  Ignoring.\n (chan = %04x)", channel);
		return 0;
	}

	return ret;
}

/*
 * Possible codes:
 *    AIM_TRANSFER_DENY_NOTSUPPORTED -- "client does not support"
 *    AIM_TRANSFER_DENY_DECLINE -- "client has declined transfer"
 *    AIM_TRANSFER_DENY_NOTACCEPTING -- "client is not accepting transfers"
 * 
 */
faim_export int aim_denytransfer(aim_session_t *sess, const char *sender, const char *cookie, fu16_t code)
{
	aim_conn_t *conn;
	aim_frame_t *fr;
	aim_snacid_t snacid;
	aim_tlvlist_t *tl = NULL;
	
	if (!sess || !(conn = aim_conn_findbygroup(sess, 0x0004)))
		return -EINVAL;

	if (!(fr = aim_tx_new(sess, conn, AIM_FRAMETYPE_FLAP, 0x02, 10+8+2+1+strlen(sender)+6)))
		return -ENOMEM;

	snacid = aim_cachesnac(sess, 0x0004, 0x000b, 0x0000, NULL, 0);
	aim_putsnac(&fr->data, 0x0004, 0x000b, 0x0000, snacid);
	
	aimbs_putraw(&fr->data, cookie, 8);

	aimbs_put16(&fr->data, 0x0002); /* channel */
	aimbs_put8(&fr->data, strlen(sender));
	aimbs_putraw(&fr->data, sender, strlen(sender));

	aim_addtlvtochain16(&tl, 0x0003, code);
	aim_writetlvchain(&fr->data, &tl);
	aim_freetlvchain(&tl);

	aim_tx_enqueue(sess, fr);

	return 0;
}

/*
 * aim_reqicbmparaminfo()
 *
 * Request ICBM parameter information.
 *
 */
faim_export int aim_reqicbmparams(aim_session_t *sess)
{
	aim_conn_t *conn;

	if (!sess || !(conn = aim_conn_findbygroup(sess, 0x0004)))
		return -EINVAL;

	return aim_genericreq_n(sess, conn, 0x0004, 0x0004);
}

/*
 *
 * I definitly recommend sending this.  If you don't, you'll be stuck
 * with the rather unreasonable defaults.  You don't want those.  Send this.
 * 
 */
faim_export int aim_seticbmparam(aim_session_t *sess, struct aim_icbmparameters *params)
{
	aim_conn_t *conn;
	aim_frame_t *fr;
	aim_snacid_t snacid;

	if (!sess || !(conn = aim_conn_findbygroup(sess, 0x0004)))
		return -EINVAL;

	if (!params)
		return -EINVAL;

	if (!(fr = aim_tx_new(sess, conn, AIM_FRAMETYPE_FLAP, 0x02, 10+16)))
		return -ENOMEM;

	snacid = aim_cachesnac(sess, 0x0004, 0x0002, 0x0000, NULL, 0);
	aim_putsnac(&fr->data, 0x0004, 0x0002, 0x0000, snacid);

	/* This is read-only (see Parameter Reply). Must be set to zero here. */
	aimbs_put16(&fr->data, 0x0000);

	/* These are all read-write */
	aimbs_put32(&fr->data, params->flags); 
	aimbs_put16(&fr->data, params->maxmsglen);
	aimbs_put16(&fr->data, params->maxsenderwarn); 
	aimbs_put16(&fr->data, params->maxrecverwarn); 
	aimbs_put32(&fr->data, params->minmsginterval);

	aim_tx_enqueue(sess, fr);

	return 0;
}

static int paraminfo(aim_session_t *sess, aim_module_t *mod, aim_frame_t *rx, aim_modsnac_t *snac, aim_bstream_t *bs)
{
	struct aim_icbmparameters params;
	aim_rxcallback_t userfunc;

	params.maxchan = aimbs_get16(bs);
	params.flags = aimbs_get32(bs);
	params.maxmsglen = aimbs_get16(bs);
	params.maxsenderwarn = aimbs_get16(bs);
	params.maxrecverwarn = aimbs_get16(bs);
	params.minmsginterval = aimbs_get32(bs);
	
	if ((userfunc = aim_callhandler(sess, rx->conn, snac->family, snac->subtype)))
		return userfunc(sess, rx, &params);

	return 0;
}

static int missedcall(aim_session_t *sess, aim_module_t *mod, aim_frame_t *rx, aim_modsnac_t *snac, aim_bstream_t *bs)
{
	int ret = 0;
	aim_rxcallback_t userfunc;
	fu16_t channel, nummissed, reason;
	aim_userinfo_t userinfo;

	while (aim_bstream_empty(bs)) {	

		channel = aimbs_get16(bs);
		aim_extractuserinfo(sess, bs, &userinfo);
		nummissed = aimbs_get16(bs);
		reason = aimbs_get16(bs);

		if ((userfunc = aim_callhandler(sess, rx->conn, snac->family, snac->subtype)))
			 ret = userfunc(sess, rx, channel, &userinfo, nummissed, reason);
	}

	return ret;
}

static int clienterr(aim_session_t *sess, aim_module_t *mod, aim_frame_t *rx, aim_modsnac_t *snac, aim_bstream_t *bs)
{
	int ret = 0;
	aim_rxcallback_t userfunc;
	fu16_t channel, reason;
	char *sn;
	fu8_t *ck, snlen;

	ck = aimbs_getraw(bs, 8);
	channel = aimbs_get16(bs);
	snlen = aimbs_get8(bs);
	sn = aimbs_getstr(bs, snlen);
	reason = aimbs_get16(bs);

	if ((userfunc = aim_callhandler(sess, rx->conn, snac->family, snac->subtype)))
		 ret = userfunc(sess, rx, channel, sn, reason);

	free(ck);
	free(sn);

	return ret;
}

static int msgack(aim_session_t *sess, aim_module_t *mod, aim_frame_t *rx, aim_modsnac_t *snac, aim_bstream_t *bs)
{
	aim_rxcallback_t userfunc;
	fu16_t type;
	fu8_t snlen, *ck;
	char *sn;
	int ret = 0;

	ck = aimbs_getraw(bs, 8);
	type = aimbs_get16(bs);
	snlen = aimbs_get8(bs);
	sn = aimbs_getstr(bs, snlen);

	if ((userfunc = aim_callhandler(sess, rx->conn, snac->family, snac->subtype)))
		ret = userfunc(sess, rx, type, sn);

	free(sn);
	free(ck);

	return ret;
}

static int snachandler(aim_session_t *sess, aim_module_t *mod, aim_frame_t *rx, aim_modsnac_t *snac, aim_bstream_t *bs)
{

	if (snac->subtype == 0x0005)
		return paraminfo(sess, mod, rx, snac, bs);
	else if (snac->subtype == 0x0006)
		return outgoingim(sess, mod, rx, snac, bs);
	else if (snac->subtype == 0x0007)
		return incomingim(sess, mod, rx, snac, bs);
	else if (snac->subtype == 0x000a)
		return missedcall(sess, mod, rx, snac, bs);
	else if (snac->subtype == 0x000b)
		return clienterr(sess, mod, rx, snac, bs);
	else if (snac->subtype == 0x000c)
		return msgack(sess, mod, rx, snac, bs);

	return 0;
}

faim_internal int msg_modfirst(aim_session_t *sess, aim_module_t *mod)
{

	mod->family = 0x0004;
	mod->version = 0x0001;
	mod->toolid = 0x0110;
	mod->toolversion = 0x047b;
	mod->flags = 0;
	strncpy(mod->name, "messaging", sizeof(mod->name));
	mod->snachandler = snachandler;

	return 0;
}
