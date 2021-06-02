#include "mutton.h"

int mutton_get_resdir(char *path, size_t path_max) {
    if (!path || path_max == 0) {
        return -1;
    }
#if defined(PLATFORM_WINDOWS)
    DWORD length = GetModuleFileNameA(NULL, path, path_max);
    if (length > 0 && length < path_max) {
        for (DWORD i = length - 1; i > 0; i--) {
            if (path[i] == FC_DIRECTORY_SEPARATOR) {
                path[i + 1] = 0;
                return 0;
            }
        }
    }
    path[0] = 0;
    return -1;
#elif defined(PLATFORM_LINUX) && !defined(PLATFORM_ANDROID)
    ssize_t length = readlink("/proc/self/exe", path, path_max - 1);
    if (length > 0 && (size_t)length < path_max) {
        for (ssize_t i = length - 1; i > 0; i--) {
            if (path[i] == FC_DIRECTORY_SEPARATOR) {
                path[i + 1] = 0;
                return 0;
            }
        }
    }
    path[0] = 0;
    return -1;
#elif defined(PLATFORM_IOS)
    int result = -1;
    FC_AUTORELEASEPOOL_BEGIN
    CFBundleRef bundle = CFBundleGetMainBundle();
    if (bundle) {
        CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(bundle);
        if (resourcesURL) {
            Boolean success = CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path,
                                                               (CFIndex)path_max - 1);
            CFRelease(resourcesURL);
            if (success) {
                unsigned long length = strlen(path);
                if (length > 0 && length < path_max - 1) {
                    // Add trailing slash
                    if (path[length - 1] != FC_DIRECTORY_SEPARATOR) {
                        path[length] = FC_DIRECTORY_SEPARATOR;
                        path[length + 1] = 0;
                    }
                    result = 0;
                }
            }
        }
    }
    FC_AUTORELEASEPOOL_END
    if (result != 0) {
        path[0] = 0;
    }
    return result;
#elif defined(PLATFORM_ANDROID)
    path[0] = 0;
    return 0;
#else
#error Unsupported platform
#endif
}

int mutton_get_datadir(const char *app_id, char *path, size_t path_max) {
#if defined(PLATFORM_WINDOWS)
    wchar_t *wpath = NULL;
    size_t count = 0; // Output count including NULL
    size_t app_id_length = strlen(app_id);
    int success = (SUCCEEDED(SHGetKnownFolderPath(&FOLDERID_RoamingAppData, 0, NULL, &wpath)) &&
                   wcstombs_s(&count, path, path_max, wpath, path_max - 1) == 0 &&
                   count > 1 && count + app_id_length + 2 <= path_max);
    CoTaskMemFree(wpath);
    if (!success) {
        path[0] = 0;
        return -1;
    }
    if (path[count - 2] != FC_DIRECTORY_SEPARATOR) {
        path[count - 1] = FC_DIRECTORY_SEPARATOR;
        path[count] = 0;
        count++;
    }
    strcpy_s(path + count - 1, path_max - count, app_id);
    count += app_id_length;
    if (path[count - 2] != FC_DIRECTORY_SEPARATOR) {
        path[count - 1] = FC_DIRECTORY_SEPARATOR;
        path[count] = 0;
    }
    int result = SHCreateDirectoryExA(NULL, path, NULL);
    if (result == ERROR_SUCCESS || result == ERROR_ALREADY_EXISTS) {
        return 0;
    } else {
        path[0] = 0;
        return -1;
    }
#elif (defined(PLATFORM_LINUX) && !defined(PLATFORM_ANDROID))
    const char *home_path = getenv("XDG_DATA_HOME");
    int result = -1;
    if (home_path && *home_path) {
        result = snprintf(path, path_max, "%s/%s/", home_path, app_id);
    } else {
        home_path = getenv("HOME");
        if (home_path && *home_path) {
            result = snprintf(path, path_max, "%s/.local/share/%s/", home_path, app_id);
        }
    }
    if (result <= 0 || (size_t)result >= path_max) {
        path[0] = 0;
        return -1;
    }
    char *ch = path;
    while (*(++ch)) {
        if (*ch == '/') {
            *ch = 0;
            if (mkdir(path, 0700) != 0 && errno != EEXIST) {
                path[0] = 0;
                return -1;
            }
            *ch = '/';
        }
    }
    return 0;
#elif defined(PLATFORM_ANDROID)
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
#elif defined(PLATFORM_IOS)
    int result = -1;
    const NSUInteger NSApplicationSupportDirectory = 14;
    const NSUInteger NSUserDomainMask = 1;

#if TARGET_OS_OSX
    CFBundleRef bundle = NULL;
    int bundle_id_appended = 0;
#endif

    CFStringRef dir = NULL;
    Boolean success = NO;
    unsigned long length = 0;

    FC_AUTORELEASEPOOL_BEGIN
    CFArrayRef array =
#if __has_feature(objc_arc)
    (__bridge CFArrayRef)
#else
    (CFArrayRef)
#endif
    NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory,
                                        NSUserDomainMask, TRUE);
    if (!array || CFArrayGetCount(array) == 0) {
        goto fc_datadir_fail;
    }
    dir = (CFStringRef)CFArrayGetValueAtIndex(array, 0);
    success = CFStringGetFileSystemRepresentation(dir, path, (CFIndex)path_max - 1);
    if (!success) {
        goto fc_datadir_fail;
    }
    length = strlen(path);
    if (length == 0 || length + 1 >= path_max) {
        goto fc_datadir_fail;
    }
    // Add trailing slash
    if (path[length - 1] != FC_DIRECTORY_SEPARATOR) {
        path[length] = FC_DIRECTORY_SEPARATOR;
        path[length + 1] = 0;
        length++;
    }
    if (mkdir(path, 0700) != 0 && errno != EEXIST) {
        goto fc_datadir_fail;
    }
    result = 0;
    
#if TARGET_OS_OSX
    bundle = CFBundleGetMainBundle();
    if (bundle) {
        CFStringRef bundle_id = CFBundleGetIdentifier(bundle);
        if (bundle_id) {
            if (CFStringFind(dir, bundle_id, 0).length != 0) {
                // macOS sandboxed app
                bundle_id_appended = 1;
            } else {
                // Append bundle_id (macOS bundled, non-sandboxed app)
                CFIndex bundle_id_length = CFStringGetLength(bundle_id);
                bundle_id_length = CFStringGetMaximumSizeForEncoding(bundle_id_length,
                                                                     kCFStringEncodingUTF8);
                if (bundle_id_length > 0 &&
                    length + (unsigned long)bundle_id_length + 1 < path_max - 1 &&
                    CFStringGetCString(bundle_id, path + length, bundle_id_length,
                                       kCFStringEncodingUTF8)) {
                    path[length + (unsigned long)bundle_id_length] = FC_DIRECTORY_SEPARATOR;
                    path[length + (unsigned long)bundle_id_length + 1] = 0;
                    if (mkdir(path, 0700) != 0 && errno != EEXIST) {
                        result = -1;
                        goto fc_datadir_fail;
                    }
                    bundle_id_appended = 1;
                }
            }
        }
    }
    if (!bundle_id_appended) {
        // Append app_id (macOS executable)
        if (!app_id || !*app_id) {
            result = -1;
        } else {
            size_t app_id_length = strlen(app_id);
            if (length + app_id_length + 1 < path_max - 1) {
                strcpy(path + length, app_id);
                path[length + app_id_length] = FC_DIRECTORY_SEPARATOR;
                path[length + app_id_length + 1] = 0;
                if (mkdir(path, 0700) != 0 && errno != EEXIST) {
                    result = -1;
                    goto fc_datadir_fail;
                }
            } else {
                result = -1;
            }
        }
    }
#else
    (void)app_id;
#endif

fc_datadir_fail:
    if (result != 0) {
        path[0] = 0;
    }
    FC_AUTORELEASEPOOL_END
    return result;
#else
#error Unsupported platform
#endif
}

int mutton_get_locale(char *locale, size_t locale_max) {
    if (!locale || locale_max < 3) {
        return -1;
    }
    int result = -1;
#if defined(PLATFORM_WINDOWS)
    wchar_t wlocale[LOCALE_NAME_MAX_LENGTH];
    if (GetUserDefaultLocaleName(wlocale, LOCALE_NAME_MAX_LENGTH) > 0) {
        size_t count = 0;
        if (wcstombs_s(&count, locale, locale_max, wlocale, locale_max - 1) == 0) {
            result = 0;
        }
    }
#elif defined(PLATFORM_LINUX) && !defined(PLATFORM_ANDROID)
    setlocale(LC_ALL, "");
    char *lang = setlocale(LC_ALL, NULL);
    if (lang && lang[0] != 0 && !(lang[0] == 'C' && lang[1] == 0)) {
        result = 0;
        strncpy(locale, lang, locale_max);
        locale[locale_max - 1] = 0;
    }
#elif defined(PLATFORM_IOS)
    FC_AUTORELEASEPOOL_BEGIN
    CFArrayRef languages = CFLocaleCopyPreferredLanguages();
    if (languages) {
        if (CFArrayGetCount(languages) > 0) {
            CFStringRef language = (CFStringRef)CFArrayGetValueAtIndex(languages, 0);
            if (language) {
                CFIndex length = CFStringGetLength(language);
                if (length > (CFIndex)locale_max - 1) {
                    length = (CFIndex)locale_max - 1;
                }
                CFIndex outLength = CFStringGetBytes(language, CFRangeMake(0, length),
                                                     kCFStringEncodingUTF8, 0, FALSE,
                                                     (UInt8 *)locale, (CFIndex)locale_max - 1, NULL);
                locale[outLength] = 0;
                result = 0;
            }
        }
        CFRelease(languages);
    }
    FC_AUTORELEASEPOOL_END
#elif defined(PLATFORM_ANDROID)
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

#else
#error Unsupported platform
#endif
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


/* 
    ## Redefined Functions (Windows, Android):

    | Function            | Windows                      | Android
    |---------------------|------------------------------|-----------------------------------------
    | `printf`            | Uses `OutputDebugString`*    | Uses `__android_log_print`
    | `fopen`             | Uses `fopen_s`               | Uses `AAssetManager_open` if read mode
    | `fclose`            | Adds `NULL` check            | No change
*/

/* MARK: Windows */

#if defined(PLATFORM_WINDOWS)

static inline FILE *_fc_windows_fopen(const char *filename, const char *mode) {
    FILE *file = NULL;
    fopen_s(&file, filename, mode);
    return file;
}

static inline int _fc_windows_fclose(FILE *stream) {
    // The Windows fclose() function will crash if stream is NULL
    if (stream) {
        return fclose(stream);
    } else {
        return 0;
    }
}

#define fopen(filename, mode) _fc_windows_fopen(filename, mode)
#define fclose(file) _fc_windows_fclose(file)

#if defined(_DEBUG)

// Outputs to debug window if there is no console and IsDebuggerPresent() returns true.
static int _fc_printf(const char *format, ...) {
    int result;
    if (IsDebuggerPresent() && GetStdHandle(STD_OUTPUT_HANDLE) == NULL) {
        char buffer[1024];
        va_list args;
        va_start(args, format);
        result = vsprintf_s(buffer, sizeof(buffer), format, args);
        va_end(args);
        if (result >= 0) {
            OutputDebugStringA(buffer);
        }
    } else {
        va_list args;
        va_start(args, format);
        result = vprintf(format, args);
        va_end(args);
    }
    return result;
}

#define printf(format, ...) _fc_printf(format, __VA_ARGS__)

#endif /* _DEBUG */

#endif /* PLATFORM_WINDOWS */

/* MARK: Android */

#if defined(PLATFORM_ANDROID)

#if !defined(_BSD_SOURCE)
FILE* funopen(const void* __cookie,
              int (*__read_fn)(void*, char*, int),
              int (*__write_fn)(void*, const char*, int),
              fpos_t (*__seek_fn)(void*, fpos_t, int),
              int (*__close_fn)(void*));
#endif /* _BSD_SOURCE */

#if !defined(FILE_COMPAT_ANDROID_ACTIVITY)
#error FILE_COMPAT_ANDROID_ACTIVITY must be defined as a reference to an ANativeActivity (or NULL).
#endif

static pthread_key_t _fc_jnienv_key;
static pthread_once_t _fc_jnienv_key_once = PTHREAD_ONCE_INIT;

static void _fc_jnienv_detach(void *value) {
    if (value) {
        JavaVM *vm = (JavaVM *)value;
#ifdef __cplusplus
        vm->DetachCurrentThread();
#else
        (*vm)->DetachCurrentThread(vm);
#endif
    }
}

static void _fc_create_jnienv_key() {
    pthread_key_create(&_fc_jnienv_key, _fc_jnienv_detach);
}

static JNIEnv *_fc_jnienv(JavaVM *vm) {
    JNIEnv *jniEnv = NULL;
    int setThreadLocal = 0;
#ifdef __cplusplus
    setThreadLocal = (vm->GetEnv((void **)&jniEnv, JNI_VERSION_1_4) != JNI_OK &&
            vm->AttachCurrentThread(&jniEnv, NULL) == JNI_OK);
#else
    setThreadLocal =  ((*vm)->GetEnv(vm, (void **)&jniEnv, JNI_VERSION_1_4) != JNI_OK &&
            (*vm)->AttachCurrentThread(vm, &jniEnv, NULL) == JNI_OK);
#endif
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

static FILE *_fc_android_fopen(const char *filename, const char *mode) {
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

#define printf(...) __android_log_print(ANDROID_LOG_INFO, "stdout", __VA_ARGS__)
#define fopen(filename, mode) _fc_android_fopen(filename, mode)

#endif /* PLATFORM_ANDROID */
