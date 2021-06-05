cmake_minimum_required(VERSION 3.10)
project(Mutton)

option(BUILD_DESKTOP "Build the static library for desktop" OFF)
option(BUILD_ANDROID "Build the static library for android" OFF)

if(WIN32)
    set(OS_NAME windows-x86_64)
endif()

if(APPLE)
    set(OS_NAME darwin-x86_64)
endif()

if(UNIX AND NOT APPLE)
    set(OS_NAME linux-x86_64)
endif()


# DEFINE ALL THE SOURCE FILES
# ---------------------------

# Freetype
set(FREETYPE_SRCS 
        extern/freetype/src/autofit/autofit.c
        extern/freetype/src/base/ftbase.c
        extern/freetype/src/base/ftbbox.c
        extern/freetype/src/base/ftbitmap.c
        extern/freetype/src/base/ftdbgmem.c
        extern/freetype/src/base/ftdebug.c
        extern/freetype/src/base/ftglyph.c
        extern/freetype/src/base/ftinit.c
        extern/freetype/src/base/ftstroke.c
        extern/freetype/src/base/ftsynth.c
        extern/freetype/src/base/ftsystem.c
        extern/freetype/src/bdf/bdf.c
        extern/freetype/src/bzip2/ftbzip2.c
        extern/freetype/src/cache/ftcache.c
        extern/freetype/src/cff/cff.c
        extern/freetype/src/cid/type1cid.c
        extern/freetype/src/gxvalid/gxvalid.c
        extern/freetype/src/gzip/ftgzip.c
        extern/freetype/src/lzw/ftlzw.c
        extern/freetype/src/otvalid/otvalid.c
        extern/freetype/src/pcf/pcf.c
        extern/freetype/src/pfr/pfr.c
        extern/freetype/src/psaux/psaux.c
        extern/freetype/src/pshinter/pshinter.c
        extern/freetype/src/psnames/psnames.c
        extern/freetype/src/raster/raster.c
        extern/freetype/src/sfnt/sfnt.c
        extern/freetype/src/smooth/smooth.c
        extern/freetype/src/truetype/truetype.c
        extern/freetype/src/type1/type1.c
        extern/freetype/src/type42/type42.c
        extern/freetype/src/winfonts/winfnt.c    
)

# Glad
set(GLAD_SRCS 
        extern/GLAD/glad_gl.c 
        extern/GLAD/glad_vulkan.c
)

# GLFM
set(GLFM_SRCS
        extern/glfm/src/glfm_platform_android.c
)

# GLFW
set(GLFW_SRCS 
            extern/glfw/src/context.c 
            extern/glfw/src/init.c 
            extern/glfw/src/input.c 
            extern/glfw/src/monitor.c 
            extern/glfw/src/vulkan.c 
            extern/glfw/src/window.c
)

if(WIN32)
    set(GLFW_CFLAGS -D_GLFW_WIN32)
    set(GLFW_LDFLAGS -lgdi32)
    list(APPEND GLFW_SRCS
            extern/glfw/src/win32_init.c 
            extern/glfw/src/win32_joystick.c 
            extern/glfw/src/win32_monitor.c 
            extern/glfw/src/win32_time.c 
            extern/glfw/src/win32_thread.c 
            extern/glfw/src/win32_window.c 
            extern/glfw/src/wgl_context.c 
            extern/glfw/src/egl_context.c 
            extern/glfw/src/osmesa_context.c
)
endif()

if(APPLE)
    # for MacOS X or iOS, watchOS, tvOS (since 3.10.3)
    set(GLFW_CFLAGS -D_GLFW_COCOA)
    set(GLFW_LDFLAGS -framework Cocoa -framework IOKit -framework CoreFoundation)

    list(APPEND GLFW_SRCS
        extern/glfw/src/cocoa_init.m 
        extern/glfw/src/cocoa_joystick.m 
        extern/glfw/src/cocoa_monitor.m 
        extern/glfw/src/cocoa_window.m 
        extern/glfw/src/cocoa_time.c 
        extern/glfw/src/posix_thread.c 
        extern/glfw/src/nsgl_context.m 
        extern/glfw/src/egl_context.c 
        extern/glfw/src/osmesa_context.c    
)
endif()

if(UNIX AND NOT APPLE)
    # for Linux, BSD, Solaris, Minix
    set(GLFW_CFLAGS -D_GLFW_X11)
    set(GLFW_LDFLAGS -ldl -lm -lrt -lX11 -lGL -lpthread)

    list(APPEND GLFW_SRCS
        extern/glfw/src/x11_init.c 
        extern/glfw/src/linux_joystick.c 
        extern/glfw/src/x11_monitor.c 
        extern/glfw/src/x11_window.c 
        extern/glfw/src/xkb_unicode.c 
        extern/glfw/src/posix_time.c 
        extern/glfw/src/posix_thread.c 
        extern/glfw/src/glx_context.c 
        extern/glfw/src/egl_context.c 
        extern/glfw/src/osmesa_context.c    
)
endif()

# Mutton
set(MUTTON_SRCS
            src/platform/platform.c
			src/application/app.c
			src/utils/String.c
			src/utils/Math.c
)

# BUILD THE STATIC LIBRARY FOR DESKTOP
# ------------------------------------
if(BUILD_DESKTOP)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib/${OS_NAME})

    set(CMAKE_C_FLAGS "-DFT2_BUILD_LIBRARY=1 ${GLFW_CFLAGS}")
    add_library(MuttonDesktop STATIC 
                                ${FREETYPE_SRCS}
                                ${GLAD_SRCS}
                                ${GLFW_SRCS}
                                ${MUTTON_SRCS}
    )

    target_include_directories(MuttonDesktop PRIVATE
                                include
                                extern/GLAD
                                extern/glfw/include
                                extern/freetype/include 
    )
    ADD_CUSTOM_COMMAND(TARGET MuttonDesktop POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E rename
        $<TARGET_FILE:MuttonDesktop>
        $<TARGET_FILE_DIR:MuttonDesktop>/$<TARGET_FILE_PREFIX:MuttonDesktop>Mutton$<TARGET_FILE_SUFFIX:MuttonDesktop>
        COMMENT "Renaming 'MuttonDesktop' to 'Mutton'"
    )

endif()

# BUILD THE STATIC LIBRARY FOR ANDROID
# ------------------------------------
if(BUILD_ANDROID)
    set(ANDROIDVERSION 24)
    set(ANDROIDTARGET ${ANDROIDVERSION})

    # -- find the android SDK
    set(SDK_LOCATIONS 
            $ENV{ANDROID_HOME}
            $ENV{ANDROID_SDK_ROOT}
            $ENV{HOME}/Android/Sdk 
            $ENV{HOME}/Library/Android/sdk
    )

    foreach(sdk_dir IN LISTS SDK_LOCATIONS)
        if(EXISTS ${sdk_dir})
            set(ANDROIDSDK ${sdk_dir})
            break()
        endif()
    endforeach()

    if(NOT DEFINED ANDROIDSDK)
        message(FATAL_ERROR "Android SDK was not found")
    else()
        message(STATUS "Android SDK found at ${ANDROIDSDK}")
    endif()

    if(WIN32)
        # -- fix windows paths
        file(TO_CMAKE_PATH ${ANDROIDSDK} sdk)
        set(ANDROIDSDK ${sdk})
    endif()

    # -- find the android NDK
    set(NDK_LOCATIONS 
            $ENV{ANDROID_NDK}
            $ENV{ANDROID_NDK_HOME}
            ${ANDROIDSDK}/ndk-bundle
    )
    FILE(GLOB ndk_a ABSOLUTE ${ANDROIDSDK}/ndk/*)
    list(APPEND NDK_LOCATIONS ${ndk_a})
    list(REVERSE NDK_LOCATIONS) # if there are multiple versions, use latest

    foreach(ndk_dir IN LISTS NDK_LOCATIONS)
        if(EXISTS ${ndk_dir})
            set(NDK ${ndk_dir})
            break()
        endif()
    endforeach()

    if(NOT DEFINED NDK)
        message(FATAL_ERROR "Android NDK was not found")
    else()
        message(STATUS "Android NDK found at ${NDK}")
    endif()

    if(WIN32)
        # -- fix windows paths
        file(TO_CMAKE_PATH ${NDK} ndk)
        set(ANDROIDNDK ${ndk})
    endif()


    # -- get the latest android build tools
    FILE(GLOB BUILD_TOOL_LOCATIONS ABSOLUTE ${ANDROIDSDK}/build-tools/*)
    list(POP_BACK BUILD_TOOL_LOCATIONS BUILD_TOOLS)

    # Android Toolchains and flags
    set(ADB adb)
    set(AAPT ${BUILD_TOOLS}/aapt)

    set(ANDROID_CC_ARM64 ${NDK}/toolchains/llvm/prebuilt/${OS_NAME}/bin/aarch64-linux-android${ANDROIDVERSION}-clang)
    set(ANDROID_CC_ARM32 ${NDK}/toolchains/llvm/prebuilt/${OS_NAME}/bin/armv7a-linux-androideabi${ANDROIDVERSION}-clang)
    set(ANDROID_CC_x86 ${NDK}/toolchains/llvm/prebuilt/${OS_NAME}/bin/i686-linux-android${ANDROIDVERSION}-clang)
    set(ANDROID_CC_x86_64 ${NDK}/toolchains/llvm/prebuilt/${OS_NAME}/bin/x86_64-linux-android${ANDROIDVERSION}-clang)

    set(ANDROID_AR_ARM64 ${NDK}/toolchains/llvm/prebuilt/${OS_NAME}/bin/aarch64-linux-android-ar)
    set(ANDROID_AR_ARM32 ${NDK}/toolchains/llvm/prebuilt/${OS_NAME}/bin/arm-linux-androideabi-ar)
    set(ANDROID_AR_x86 ${NDK}/toolchains/llvm/prebuilt/${OS_NAME}/bin/i686-linux-android-ar)
    set(ANDROID_AR_x86_64 ${NDK}/toolchains/llvm/prebuilt/${OS_NAME}/bin/x86_64-linux-android-ar)

    set(ANDROID_CFLAGS_ARM64 -DANDROID_ABI=arm64-v8a -m64)
    set(ANDROID_CFLAGS_ARM32 -DANDROID_ABI=armeabi-v7a -mfloat-abi=softfp -m32)
    set(ANDROID_CFLAGS_x86 -DANDROID_ABI=x86 -march=i686 -mtune=intel -mssse3 -mfpmath=sse -m32)
    set(ANDROID_CFLAGS_x86_64 -DANDROID_ABI=x86_64 -march=x86-64 -msse4.2 -mpopcnt -m64 -mtune=intel)


    set(ANDROID_TARGETS arm64-v8a;armeabi-v7a;x86;x86_64)


    set(ANDROID_COMPILERS 
                ${ANDROID_CC_ARM64}
                ${ANDROID_CC_ARM32}
                ${ANDROID_CC_x86}
                ${ANDROID_CC_x86_64}
    )

    set(ANDROID_AR 
                ${ANDROID_AR_ARM64}
                ${ANDROID_AR_ARM32}
                ${ANDROID_AR_x86}
                ${ANDROID_AR_x86_64}
    )

    set(ANDROID_CFLAGS 
                ${ANDROID_CFLAGS_ARM64}
                ${ANDROID_CFLAGS_ARM32}
                ${ANDROID_CFLAGS_x86}
                ${ANDROID_CFLAGS_x86_64}
    )

    # -- build mobile lib for each android target
    # foreach(option IN ZIP_LISTS ANDROID_COMPILERS ANDROID_AR ANDROID_CFLAGS ANDROID_TARGETS)
    foreach(index RANGE 0 3)
        list(GET ANDROID_TARGETS ${index} target)
        list(GET ANDROID_COMPILERS ${index} compiler)
        list(GET ANDROID_AR ${index} ar)
        list(GET ANDROID_CFLAGS ${index} cflags)
        
        set(CMAKE_C_COMPILER ${compiler})
        set(CMAKE_C_COMPILER_AR ${ar})
        set(CMAKE_C_FLAGS "-fPIC -DANDROIDVERSION=${ANDROIDVERSION} -DFT2_BUILD_LIBRARY=1 ${cflags}")
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib/android/${target})
        add_library(MuttonMobile${target} STATIC
                                    ${NDK}/sources/android/native_app_glue/android_native_app_glue.c
                                    ${FREETYPE_SRCS}
                                    ${GLFM_SRCS}
                                    ${MUTTON_SRCS}
        )

        target_include_directories(MuttonMobile${target} PRIVATE
                                    include
                                    extern/glfm/include
                                    extern/freetype/src
                                    extern/freetype/include
                                    ${NDK}/sources/android/native_app_glue
                                    ${NDK}/toolchains/llvm/prebuilt/${OS_NAME}/sysroot/usr/include 
                                    ${NDK}/toolchains/llvm/prebuilt/${OS_NAME}/sysroot/usr/include/android 
        )

        set(trg MuttonMobile${target})
        ADD_CUSTOM_COMMAND(TARGET MuttonMobile${target} POST_BUILD
                            COMMAND ${CMAKE_COMMAND} -E rename
                            $<TARGET_FILE:${trg}>
                            $<TARGET_FILE_DIR:${trg}>/$<TARGET_FILE_PREFIX:${trg}>Mutton$<TARGET_FILE_SUFFIX:${trg}>
                            COMMENT "Renaming 'MuttonMobile${target}' to 'MuttonMobile'"
        )

        
    endforeach()
endif()

function(print_variables)
    get_cmake_property(_variableNames VARIABLES)
    list (REVERSE _variableNames)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()    
endfunction()

function(print_env)
    execute_process(COMMAND "${CMAKE_COMMAND}" "-E" "environment")  
endfunction()

print_variables()

# add_compile_options(
#   "-Wall" "-Wpedantic" "-Wextra" "-fexceptions"
#   "$<$<CONFIG:DEBUG>:-O0;-g3;-ggdb>"
# )