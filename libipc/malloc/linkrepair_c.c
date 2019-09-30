#include <stddef.h>
#include <unistd.h>
#include <hp-timing.h>


size_t _dl_pagesize;


/*char * _itoa_word (_ITOA_WORD_TYPE value, char *buflim,
				   unsigned int base, int upper_case)
{
	const char *digits = (upper_case
						  ? _itoa_upper_digits
						  : _itoa_lower_digits);

	switch (base)
	{
# define SPECIAL(Base)							      \
    case Base:								      \
      do								      \
	*--buflim = digits[value % Base];				      \
      while ((value /= Base) != 0);					      \
      break

		SPECIAL (10);
		SPECIAL (16);
		SPECIAL (8);
		default:
			do
				*--buflim = digits[value % base];
			while ((value /= base) != 0);
	}
	return buflim;
}
# undef SPECIAL
*/

const char _itoa_upper_digits[37] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";




#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <libioP.h>


int __fxprintf (FILE *fp, const char *fmt, ...)
{
	if (fp == NULL)
		fp = stderr;

	va_list ap;
	va_start (ap, fmt);

	int res;
	if (_IO_fwide (fp, 0) > 0)
	{
		size_t len = strlen (fmt) + 1;
		wchar_t wfmt[len];
		for (size_t i = 0; i < len; ++i)
		{
			assert (isascii (fmt[i]));
			wfmt[i] = fmt[i];
		}
		res = vfwprintf (fp, wfmt, ap);
	}
	else
		res = _IO_vfprintf (fp, fmt, ap);

	va_end (ap);

	return res;
}
