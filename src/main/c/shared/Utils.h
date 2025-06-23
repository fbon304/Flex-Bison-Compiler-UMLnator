#ifndef UTILS_HEADER
#define UTILS_HEADER

#include "Type.h"
#include "Environment.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

boolean isTimestamp(char * timestamp);

boolean isTime(char * time);

boolean isDate(char * date);

boolean is_uuid(const char *uuid);

FILE *open_session_file(void);

#endif