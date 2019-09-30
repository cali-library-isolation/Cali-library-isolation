#ifndef LIBNSJAIL_CMDLINE_UTILS_H
#define LIBNSJAIL_CMDLINE_UTILS_H

#include <zconf.h>
#include <pwd.h>
#include <grp.h>
#include <ctype.h>
#include "../nsjail/logs.h"

static bool cmdlineIsANumber(const char *s)
{
	for (int i = 0; s[i]; s++) {
		if (!isdigit(s[i]) && s[i] != 'x') {
			return false;
		}
	}
	return true;
}

#endif //LIBNSJAIL_CMDLINE_UTILS_H