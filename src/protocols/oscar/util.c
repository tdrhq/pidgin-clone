/*
 * A little bit of this
 * A little bit of that
 * It started with a kiss
 * Now we're up to bat
 */

#define FAIM_INTERNAL
#include <aim.h>
#include <ctype.h>

#ifdef _WIN32
#include "win32dep.h"
#endif

faim_internal void faimdprintf(aim_session_t *sess, int dlevel, const char *format, ...)
{
	if (!sess) {
		fprintf(stderr, "faimdprintf: no session! boo! (%d, %s)\n", dlevel, format);
		return;
	}
	if ((dlevel <= sess->debug) && sess->debugcb) {
		va_list ap;
		va_start(ap, format);
		sess->debugcb(sess, dlevel, format, ap);
		va_end(ap);
	}

	return;
}

faim_export int aimutil_putstr(char *dest, const char *src, int len)
{
	memcpy(dest, src, len);
	return len;
}

/*
 * Tokenizing functions.  Used to portably replace strtok/sep.
 *   -- DMP.
 *
 */
faim_export int aimutil_tokslen(char *toSearch, int theindex, char dl)
{
	int curCount = 1;
	char *next;
	char *last;
	int toReturn;

	last = toSearch;
	next = strchr(toSearch, dl);

	while(curCount < theindex && next != NULL) {
		curCount++;
		last = next + 1;
		next = strchr(last, dl);
	}

	if ((curCount < theindex) || (next == NULL))
		toReturn = strlen(toSearch) - (curCount - 1);
	else
		toReturn = next - toSearch - (curCount - 1);

	return toReturn;
}

faim_export int aimutil_itemcnt(char *toSearch, char dl)
{
	int curCount;
	char *next;

	curCount = 1;

	next = strchr(toSearch, dl);

	while(next != NULL) {
		curCount++;
		next = strchr(next + 1, dl);
	}

	return curCount;
}

faim_export char *aimutil_itemindex(char *toSearch, int theindex, char dl)
{
	int curCount;
	char *next;
	char *last;
	char *toReturn;

	curCount = 0;

	last = toSearch;
	next = strchr(toSearch, dl);

	while (curCount < theindex && next != NULL) {
		curCount++;
		last = next + 1;
		next = strchr(last, dl);
	}

	if (curCount < theindex) {
		toReturn = malloc(sizeof(char));
		*toReturn = '\0';
	}
	next = strchr(last, dl);

	if (curCount < theindex) {
		toReturn = malloc(sizeof(char));
		*toReturn = '\0';
	} else {
		if (next == NULL) {
			toReturn = malloc((strlen(last) + 1) * sizeof(char));
			strcpy(toReturn, last);
		} else {
			toReturn = malloc((next - last + 1) * sizeof(char));
			memcpy(toReturn, last, (next - last));
			toReturn[next - last] = '\0';
		}
	}
	return toReturn;
}

/**
 * Calculate the checksum of a given icon.
 *
 */
faim_export fu16_t aimutil_iconsum(const fu8_t *buf, int buflen)
{
	fu32_t sum;
	int i;

	for (i=0, sum=0; i+1<buflen; i+=2)
		sum += (buf[i+1] << 8) + buf[i];
	if (i < buflen)
		sum += buf[i];
	sum = ((sum & 0xffff0000) >> 16) + (sum & 0x0000ffff);

	return sum;
}

/**
 * Check if the given screen name is a valid AIM screen name.
 * Example: BobDole
 * Example: Henry_Ford@mac.com
 *
 * @return 1 if the screen name is valid, 0 if not.
 */
static int aim_snvalid_aim(const char *sn)
{
	int i;

	for (i = 0; sn[i] != '\0'; i++) {
		if (!isalnum(sn[i]) && (sn[i] != ' ') &&
			(sn[i] != '@') && (sn[i] != '.') &&
			(sn[i] != '_') && (sn[i] != '-'))
			return 0;
	}

	return 1;
}

/**
 * Check if the given screen name is a valid ICQ screen name.
 * Example: 1234567
 *
 * @return 1 if the screen name is valid, 0 if not.
 */
static int aim_snvalid_icq(const char *sn)
{
	int i;

	for (i = 0; sn[i] != '\0'; i++) {
		if (!isdigit(sn[i]))
			return 0;
	}

	return 1;
}

/**
 * Check if the given screen name is a valid SMS screen name.
 * Example: +19195551234
 *
 * @return 1 if the screen name is valid, 0 if not.
 */
static int aim_snvalid_sms(const char *sn)
{
	int i;

	if (sn[0] != '+')
		return 0;

	for (i = 1; sn[i] != '\0'; i++) {
		if (!isdigit(sn[i]))
			return 0;
	}

	return 1;
}

/**
 * Check if the given screen name is a valid oscar screen name.
 *
 * @return 1 if the screen name is valid, 0 if not.
 */
faim_export int aim_snvalid(const char *sn)
{
	if ((sn == NULL) || (*sn == '\0'))
		return 0;

	if (isalpha(sn[0]))
		return aim_snvalid_aim(sn);
	else if (isdigit(sn[0]))
		return aim_snvalid_icq(sn);
	else if (sn[0] == '+')
		return aim_snvalid_sms(sn);

	return 0;
}

/**
 * Determine if a given screen name is an ICQ screen name 
 * (i.e. it begins with a number).
 *
 * @sn A valid AIM or ICQ screen name.
 * @return 1 if the screen name is an ICQ screen name.  Otherwise 0
 *         is returned.
 */
faim_export int aim_sn_is_icq(const char *sn)
{
	if (isalpha(sn[0]))
		return 0;
	return 1;
}

/*
* int snlen(const char *)
* 
* This takes a screen name and returns its length without
* spaces.  If there are no spaces in the SN, then the 
* return is equal to that of strlen().
*
*/
faim_export int aim_snlen(const char *sn)
{
	int i = 0;

	if (!sn)
		return 0;

	while (*sn != '\0') {
		if (*sn != ' ')
			i++;
		sn++;
	}

	return i;
}

/*
* int sncmp(const char *, const char *)
*
* This takes two screen names and compares them using the rules
* on screen names for AIM/AOL.  Mainly, this means case and space
* insensitivity (all case differences and spacing differences are
* ignored, with the exception that screen names can not start with 
* a space).
*
* Return: 0 if equal
*     non-0 if different
*
*/
faim_export int aim_sncmp(const char *sn1, const char *sn2)
{

	if ((sn1 == NULL) || (sn2 == NULL))
		return -1;

	do {
		while (*sn2 == ' ')
			sn2++;
		while (*sn1 == ' ')
			sn1++;
		if (toupper(*sn1) != toupper(*sn2))
			return 1;
	} while ((*sn1 != '\0') && sn1++ && sn2++);

	return 0;
}
