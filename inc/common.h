#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef _cplusplus
extern "C" {
#endif

	inline long GF_GetElapsedTime( void );
	char *GF_GetTime(char *strbuf, char *format);
	char *GF_DumpTime(void);
	void *GF_AddAlloc(void *ptr, size_t len, char *buffName);
	void  GF_Free(void *ptr, char *buffName);
	char *GF_GetRandNum(int digit);
	char *GF_GetIpAddr(char *nic);
	char *GF_GetMacAddr(char *nic);
	int   GF_GetCmdStr(char *buf, char *src, int len);
	char *GF_GetCurPath(void);
	int   GF_GetFileSize(char *file_name);
	ssize_t GF_SaveToFile(char *file_name, void *buffer, long data_len, int appnd_yn);
	void GF_NSleep(time_t sec, long nsec);
	void GF_SelectSleep(int sec, long msec);


#ifdef _cplusplus
}
#endif

#endif //__COMMON_H__
