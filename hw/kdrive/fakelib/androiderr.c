
#ifdef HAVE_CONFIG_H
#include <kdrive-config.h>
#endif
#include "fakelib.h"

#include <android/log.h>

#define LOG_TAG "AndroiX"
//define LOG(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOG(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

#ifdef DDXOSVERRORF
#warn Android logging enabled
void
OsVendorVErrorF (const char *pszFormat, va_list va_args)
{
	int len;
	char buf[1024];
	if ((len = strlen(pszFormat)) >= 1024) len = 1024;
	snprintf(buf, len, pszFormat, &va_args);
	LOG(buf);
	LOG("[logging]");
	fprintf(stderr, "[OSVERROR] %s", buf);
}
#endif
