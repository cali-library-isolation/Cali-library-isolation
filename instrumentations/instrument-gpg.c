#include "utils.h"
#include <stdlib.h>

#define VERBOSE 4u

#define ZLIB_VERBOSE (VERBOSE & 0x1u)
#define SQLITE_VERBOSE (VERBOSE & 0x2u)
#define KSBA_VERBOSE (VERBOSE & 0x4u)

#include "lib-zlib.h"
#include "lib-sqlite3.h"
#include "lib-ksba.h"
