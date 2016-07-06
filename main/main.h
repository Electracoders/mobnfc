#pragma once

#include "../MyLib/_base.h"
#include "../MyLib/thread/fast_mutex.h"
#include "../MyLib/thread/tinythread.h"
#include "../MyLib/filesystem/FileApi.h"

/*MACROS*/
#define MAX_EVENTS 1024 /*Max. number of events to process at one go*/
#define LEN_NAME 16 /*Assuming that the length of the filename won't exceed 16 bytes*/
#define EVENT_SIZE  ( sizeof (struct inotify_event) ) /*size of one event*/
#define BUF_LEN     ( MAX_EVENTS * ( EVENT_SIZE + LEN_NAME )) /*buffer to store the data of events*/

typedef struct _inotifyFd
{
	char *path;
	int fd;
	int wd;
} inotifyFd;
