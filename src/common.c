#include <stdio.h>
#include <stdio.h>
#include <stdlib.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h> 
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <math.h>
#include <errno.h>

#include "common.h"
#include "debug.h"

/**
  @brief  시간 측정
  @param  void
  @return long
  @author hjmin@pineone.com
*/
inline long GF_GetElapsedTime( void )
{
	struct timeval tv;

	gettimeofday( &tv, NULL );
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

static char weekday[7][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
char *GF_GetTime(char *strbuf, char *format)
{
 	int i;
    int index = 0;

    time_t cur_time;
    struct tm *pTm;

    cur_time = time((time_t *)NULL);
    pTm = localtime(&cur_time);

    for(i=0; i < (int)strlen(format); i++)
    {
        switch(format[i])
        {
            case 'Y':		//년 (1900년 기준)
                sprintf(strbuf+index, "%04d", pTm->tm_year+1900);
                index +=4;
                break;
				
            case 'M':		//월 (0~11)
                sprintf(strbuf+index, "%02d", pTm->tm_mon+1);
                index += 2;
                break;
				
            case 'D':		//일 (1~31)
                sprintf(strbuf+index, "%02d", pTm->tm_mday);
                index += 2;
                break;
				
            case 'w':		//요일 (일~토 == 0~6)
                sprintf(strbuf+index, "%s", weekday[pTm->tm_wday]);
				index += strlen(weekday[pTm->tm_wday]);
                break;
				
		    case 'n':		//AM, PM
				(pTm->tm_hour <= 12) ? sprintf(strbuf+index, "AM") : sprintf(strbuf+index, "PM");
				index += 2;
				break;
				
            case 'h':		//시간 (00~23)
                sprintf(strbuf+index, "%02d", pTm->tm_hour);
                index += 2;
                break;
				
            case 'm':		// 분 (00 ~ 59)
                sprintf(strbuf+index, "%02d", pTm->tm_min);
                index += 2;
                break;
				
            case 's':		// 초 (00 ~ 59)
                sprintf(strbuf+index, "%02d", pTm->tm_sec);
                index += 2;
                break;
				
            default:
                strbuf[index] = format[i];
                index++;
        }//switch
    }//for

    strbuf[index] = '\0';
	
    return strbuf;
}

char *GF_DumpTime(void)
{
	static char timeStrBuf[32]={0,};
	struct timeval tv;
	struct tm tt;
	
	gettimeofday(&tv, NULL);
	localtime_r(&tv.tv_sec, &tt);

	memset(timeStrBuf, 0x00, 32);
	strftime(timeStrBuf, 32, "%H:%M:%S", &tt);
	snprintf(timeStrBuf+8, 5, ".%03ld", tv.tv_usec / 1000);
	
	return timeStrBuf;
}

void *GF_AddAlloc(void *ptr, size_t len, char *buffName)
{
	void *_ptr = NULL;
	if(ptr) 
	{
		_ptr = realloc(ptr, len);  
	//	dlp("ReAlloc - %s (0x%x : %d byte) \n", buffName, (unsigned int)_ptr, len);
	}
	else
	{
		_ptr = malloc(len);  
	//	dlp("Alloc - %s (0x%x : %d byte) \n", buffName, (unsigned int)_ptr, len);
	}

	if(_ptr == NULL)
	{
		dlperr("Failed to Alloc !! - %s (%p : %ld byte)\n", buffName, (void *)_ptr, len);
	}
	else
	{
		memset(_ptr, 0x00, len);
	}

	return _ptr;
}

void GF_Free(void *ptr, char *buffName)
{
	//	dlp("Free - %s (0x%x) \n", buffName, (unsigned int)ptr);
	if(ptr) 
	{
		free(ptr);
		ptr = NULL;
	}
}

#define DIGIT_MAX 10
char *GF_GetRandNum(int digit)
{
	static char randNum[DIGIT_MAX] = {0,};
	int range = 0;
	unsigned int seed = (unsigned)time(NULL) + (unsigned)getpid();

	if(DIGIT_MAX < digit)
		digit = DIGIT_MAX;
//	dlp( "digit: %d\n", digit);
		
	range = (int)(pow(10, digit));
//	dlp( "range: %d\n", range);

	srand(seed);
	do{
		sprintf(randNum, "%d", rand() % range + 1);
	}while( digit > (int)strlen(randNum) );
//	dlp( "random: %s\n", randNum);

	return randNum;
}

char *GF_GetIpAddr(char *nic)
{
	int sock_fd;
	struct ifreq ifr;

	static char ip_addr[16] = {0,};

	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

	/* I want to get an IPv4 IP address */
	ifr.ifr_addr.sa_family = AF_INET;

	strncpy(ifr.ifr_name, nic, IFNAMSIZ-1);

	ioctl(sock_fd, SIOCGIFADDR, &ifr);

	close(sock_fd);

	strncpy(ip_addr, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr), sizeof(ip_addr)-1);
	//dlp("ip_addr : %s \n", ip_addr);

	return ip_addr;
}

char *GF_GetMacAddr(char *nic)
{
	int sock_fd;
    struct ifreq ifr;

	unsigned char *mac = NULL;
    static char mac_addr[18] = {0,};
     
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
 
	/* I want to get an IPv4 IP address */
    ifr.ifr_addr.sa_family = AF_INET;

    strncpy(ifr.ifr_name , nic , IFNAMSIZ-1);
 
    ioctl(sock_fd, SIOCGIFHWADDR, &ifr);
 
    close(sock_fd);
     
    mac = (unsigned char *)ifr.ifr_hwaddr.sa_data;
     
    sprintf(mac_addr, "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	//dlp("mac_addr : %s \n", mac_addr);

    return mac_addr;
}

int GF_GetCmdStr(char *buf, char *src, int len)
{
	FILE *fp;

	fp = popen(src, "r");
	if(NULL == fp)
	{
		dlperr("Failed to popen() \n");
		return -1;
	}

	if(NULL == fgets(buf, len, fp))
	{
		dlperr("Failed to fgets() \n");
		return -1;
	}

	if(fp) pclose(fp);

	return 0;
}

char *GF_GetCurPath(void)
{
	return getcwd(NULL, 0);
}

int GF_GetFileSize(char *file_name)
{
    struct stat statbuf;

    if( stat(file_name, &statbuf) == -1 ) 
		dlperr("Failed to stat() \n");
    
	return(statbuf.st_size);  /* 파일 크기       */
}

ssize_t GF_SaveToFile(char *file_name, void *buffer, long data_len, int appnd_yn)
{
	int fd;
	ssize_t wrLen;

	if(appnd_yn) fd = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else         fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if( 0 > fd )
    {
		dlperr("Failed to open() - %d:%s\n", errno, strerror(errno));
		goto err;
	}
	
	wrLen = write(fd, buffer, data_len);
	if( 0 > wrLen )
	{
		dlperr("Failed to write() - %d:%s\n", errno, strerror(errno));
		goto err;
	}
	
	/* Sync mem and nand : Flushing data on disk or on nand flash.*/
	if(-1 == fsync(fd)) 
	{
		if(errno == EINVAL) 
		{
			if(-1 == fdatasync(fd)) 
			{
				dlperr("Failed to fdatasync() - %d:%s\n", errno, strerror(errno));
				goto err;
			}
		}
		else
		{
			dlperr("Failed to fsync() - %d:%s\n", errno, strerror(errno));
			goto err;
		}
	}
	//dlp("\e[1m Sync Data OK! \e[0m\n");

	close(fd);

	if(0 == GF_GetFileSize(file_name))
	{
		dlperr("Failed to GetFileSize \"%s\". - %d:%s n",file_name, errno, strerror(errno));
		goto err;
	}

	return wrLen;

err:
	close(fd);
	return -1;
}

void GF_NSleep(time_t sec, long nsec)
{
	struct timespec ts;

	if( nsec )
	{
		ts.tv_sec = sec;                            
		ts.tv_nsec = nsec;
	}
	else
	{
		ts.tv_sec = 0;
		ts.tv_nsec = 999999999L;
	}

	while(nanosleep(&ts, NULL))
	{
		if( EINTR != errno)
			break;
	}
}

void GF_SelectSleep(int sec, long msec)
{
	struct timeval tv;
	tv.tv_sec = sec;
	tv.tv_usec = msec;
	select(0, NULL, NULL, NULL, &tv);
}

