#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <dirent.h>

#include <error.h>
#include <limits.h>

#include "debug.h"
#include "common.h"


int main(int argc, char *argv[]) 
{
	dlp("Project root: %s\n", GF_GetCurPath());

	int start = 0;
	int end = 0;
	int i = 0;

	start = GF_GetElapsedTime();
#if 0
	for(i = 0; i < 200000; i++)
	{
		sleep(0);
	}
#else
	for(i = 0; i < 200000; i++)
	{
		usleep(0);
	}
#endif
	end = GF_GetElapsedTime() - start;
	dlp("[7m Total Complete Time: %ld.%lds [0m\n\n", end/1000, end%1000);


	return 0;
} /* end func main */

