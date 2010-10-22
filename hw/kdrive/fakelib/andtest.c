
#ifdef HAVE_CONFIG_H
#include <kdrive-config.h>
#endif

#include <android/log.h>

#define LOG_TAG "AndroiX"
//define LOG(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOG(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

int main() {
    LOG("test success");
}
