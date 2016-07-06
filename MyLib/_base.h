#pragma once
#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
#include <unistd.h>
#include <dirent.h>
#include <sys/time.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/time.h>
#include <limits.h>
//#include <list>
#include <errno.h>
#include <sys/inotify.h>

#define MAX_STRING_LEN 255
#define MAX_EXT_SIZE 64

typedef enum _NfcBool
{
	FALSE = 0, TRUE = 1,
} NfcBool;
