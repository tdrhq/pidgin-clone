#ifndef _GAIM_PERL_COMMON_H_
#define _GAIM_PERL_COMMON_H_

#include <XSUB.h>
#include <EXTERN.h>
#include <perl.h>
#include <glib.h>

#include "value.h"

#define is_hvref(o) \
	((o) && SvROK(o) && SvRV(o) && (SvTYPE(SvRV(o)) == SVt_PVHV))

#define hvref(o) \
	(is_hvref(o) ? (HV *)SvRV(o) : NULL);

#define GAIM_PERL_BOOT(x) \
	{ \
		extern void boot_Gaim__##x(pTHX_ CV *cv); \
		gaim_perl_callXS(boot_Gaim__##x, cv, mark); \
	}

SV *newSVGChar(const char *str);

void gaim_perl_callXS(void (*subaddr)(pTHX_ CV *cv), CV *cv, SV **mark);
void gaim_perl_bless_plain(const char *stash, void *object);
SV *gaim_perl_bless_object(void *object, const char *stash);
gboolean gaim_perl_is_ref_object(SV *o);
void *gaim_perl_ref_object(SV *o);

int execute_perl(const char *function, int argc, char **args);

#if 0
gboolean gaim_perl_value_from_sv(GaimValue *value, SV *sv);
SV *gaim_perl_sv_from_value(const GaimValue *value);
#endif

SV *gaim_perl_sv_from_vargs(const GaimValue *value, va_list args);

#endif /* _GAIM_PERL_COMMON_H_ */
