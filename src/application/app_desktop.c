#include "utils/Defs.h"

int app_file_close(FILE *f) {
    if(f) {
        return fclose(f);
    } else {
        return 0;
    }
}

FILE* app_file_open(const char *filename, const char *mode) {
#if defined(PLATFORM_WINDOWS)
    FILE *file = NULL;
    fopen_s(&file, filename, mode);
    return file;
#else
    return fopen(filename, mode);
#endif
}

void app_print(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf(fmt, args);
    va_end(args);        

}


int app_get_resdir(char *path, size_t path_max) {
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
#elif defined(PLATFORM_APPLE)
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
#else
#error Unsupported platform
#endif
}

int app_get_datadir(const char *app_id, char *path, size_t path_max) {
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
#elif defined(PLATFORM_APPLE)
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

int app_get_locale(char *locale, size_t locale_max) {
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
#elif defined(PLATFORM_APPLE)
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