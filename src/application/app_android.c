#include <android/asset_manager.h>
#include <android/log.h>
#include <android/native_activity.h>
#include <jni.h>
#include <pthread.h>
#include <string.h>

#include "application/app.h"
#include "glfm.h"
#define FILE_COMPAT_ANDROID_ACTIVITY glfmAndroidGetActivity()
static JNIEnv *_fc_jnienv(JavaVM *vm);


static FILE *_fc_android_fopen(const char *filename, const char *mode);

FILE* app_file_open(const char *filename, const char *mode) {
    return _fc_android_fopen(filename, mode);
}

int app_file_close(FILE *f) {
    return fclose(f);
}

int app_get_resdir(char *path, size_t path_max) {
    if (!path || path_max == 0) {
        return -1;
    }
    path[0] = 0;
    return 0;

}

int app_get_datadir(const char *app_id, char *path, size_t path_max) {
    (void)app_id;
    ANativeActivity *activity = FILE_COMPAT_ANDROID_ACTIVITY;
    if (!activity || !activity->internalDataPath) {
        path[0] = 0;
        return -1;
    }
    size_t length = strlen(activity->internalDataPath);
    if (length < path_max - 1) {
        strcpy(path, activity->internalDataPath);
        // Add trailing slash
        if (path[length - 1] != FC_DIRECTORY_SEPARATOR) {
            path[length] = FC_DIRECTORY_SEPARATOR;
            path[length + 1] = 0;
        }
        return 0;
    } else {
        path[0] = 0;
        return -1;
    }
}

int app_get_locale(char *locale, size_t locale_max) {
    if (!locale || locale_max < 3) {
        return -1;
    }
    int result = -1;
    ANativeActivity *activity = FILE_COMPAT_ANDROID_ACTIVITY;
    if (activity) {
        // getResources().getConfiguration().locale.toString()
        JNIEnv *jniEnv = _fc_jnienv(activity->vm);
        if ((*jniEnv)->ExceptionCheck(jniEnv)) {
            (*jniEnv)->ExceptionClear(jniEnv);
        }
        
        if ((*jniEnv)->PushLocalFrame(jniEnv, 16) == JNI_OK) {
            jclass activityClass = (*jniEnv)->GetObjectClass(jniEnv, activity->clazz);
            jmethodID getResourcesMethod = (*jniEnv)->GetMethodID(jniEnv, activityClass,
                                                                  "getResources", "()Landroid/content/res/Resources;");
            jobject resources = (*jniEnv)->CallObjectMethod(jniEnv, activity->clazz,
                                                            getResourcesMethod);
            jclass resourcesClass = (*jniEnv)->GetObjectClass(jniEnv, resources);
            jmethodID getConfigurationMethod = (*jniEnv)->GetMethodID(jniEnv, resourcesClass,
                                                                      "getConfiguration", "()Landroid/content/res/Configuration;");
            jobject configuration = (*jniEnv)->CallObjectMethod(jniEnv, resources,
                                                                getConfigurationMethod);
            jclass configurationClass = (*jniEnv)->GetObjectClass(jniEnv, configuration);
            jfieldID localeField = (*jniEnv)->GetFieldID(jniEnv, configurationClass, "locale",
                                                         "Ljava/util/Locale;");
            jobject localeObject = (*jniEnv)->GetObjectField(jniEnv, configuration, localeField);
            jclass localeClass = (*jniEnv)->GetObjectClass(jniEnv, localeObject);
            jmethodID toStringMethod = (*jniEnv)->GetMethodID(jniEnv, localeClass, "toString",
                                                              "()Ljava/lang/String;");
            jstring valueString = (*jniEnv)->CallObjectMethod(jniEnv, localeObject, toStringMethod);
            
            const char *nativeString = (*jniEnv)->GetStringUTFChars(jniEnv, valueString, 0);
            if (nativeString) {
                result = 0;
                strncpy(locale, nativeString, locale_max);
                locale[locale_max - 1] = 0;
                (*jniEnv)->ReleaseStringUTFChars(jniEnv, valueString, nativeString);
            }
            if ((*jniEnv)->ExceptionCheck(jniEnv)) {
                (*jniEnv)->ExceptionClear(jniEnv);
            }
            (*jniEnv)->PopLocalFrame(jniEnv, NULL);
        }
    }
    
    if (result == 0) {
        // Convert underscore to dash ("en_US" to "en-US")
        // Remove encoding ("en-US.UTF-8" to "en-US")
        char *ch = locale;
        while (*ch != 0) {
            if (*ch == '_') {
                *ch = '-';
            } else if (*ch == '.') {
                *ch = 0;
                break;
            }
            ch++;
        }
    } else {
        locale[0] = 0;
    }
    return result;
}


#if !defined(_BSD_SOURCE)
FILE* funopen(const void* __cookie,
              int (*__read_fn)(void*, char*, int),
              int (*__write_fn)(void*, const char*, int),
              fpos_t (*__seek_fn)(void*, fpos_t, int),
              int (*__close_fn)(void*));
#endif /* _BSD_SOURCE */

static pthread_key_t _fc_jnienv_key;
static pthread_once_t _fc_jnienv_key_once = PTHREAD_ONCE_INIT;

static void _fc_jnienv_detach(void *value) {
    if (value) {
        JavaVM *vm = (JavaVM *)value;
        (*vm)->DetachCurrentThread(vm);
    }
}

static void _fc_create_jnienv_key() {
    pthread_key_create(&_fc_jnienv_key, _fc_jnienv_detach);
}

static JNIEnv *_fc_jnienv(JavaVM *vm) {
    JNIEnv *jniEnv = NULL;
    int setThreadLocal = 0;
    setThreadLocal =  ((*vm)->GetEnv(vm, (void **)&jniEnv, JNI_VERSION_1_4) != JNI_OK &&
                       (*vm)->AttachCurrentThread(vm, &jniEnv, NULL) == JNI_OK);

    if (setThreadLocal) {
        pthread_once(&_fc_jnienv_key_once, _fc_create_jnienv_key);
        pthread_setspecific(_fc_jnienv_key, vm);
    }
    return jniEnv;
}

static int _fc_android_read(void *cookie, char *buf, int size) {
    return AAsset_read((AAsset *)cookie, buf, (size_t)size);
}

static int _fc_android_write(void *cookie, const char *buf, int size) {
    (void)cookie;
    (void)buf;
    (void)size;
    errno = EACCES;
    return -1;
}

static fpos_t _fc_android_seek(void *cookie, fpos_t offset, int whence) {
    return AAsset_seek((AAsset *)cookie, offset, whence);
}

static int _fc_android_close(void *cookie) {
    AAsset_close((AAsset *)cookie);
    return 0;
}

FILE *_fc_android_fopen(const char *filename, const char *mode) {
    ANativeActivity *activity = FILE_COMPAT_ANDROID_ACTIVITY;
    AAssetManager *assetManager = NULL;
    AAsset *asset = NULL;
    if (activity) {
        assetManager = activity->assetManager;
    }
    if (assetManager && mode && mode[0] == 'r') {
        asset = AAssetManager_open(assetManager, filename, AASSET_MODE_UNKNOWN);
    }
    if (asset) {
        return funopen(asset, _fc_android_read, _fc_android_write, _fc_android_seek,
                       _fc_android_close);
    } else {
        return fopen(filename, mode);
    }
}
