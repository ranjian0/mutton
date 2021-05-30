# Android version
ANDROIDVERSION?=24
ANDROIDTARGET?=$(ANDROIDVERSION)


# Find android SDK and NDK
SDK_LOCATIONS += $(ANDROID_HOME) $(ANDROID_SDK_ROOT) ~/Android/Sdk $(HOME)/Library/Android/sdk
ANDROIDSDK?=$(firstword $(foreach dir, $(SDK_LOCATIONS), $(basename $(dir) ) ) )
NDK?=$(firstword $(ANDROID_NDK) $(ANDROID_NDK_HOME) $(wildcard $(ANDROIDSDK)/ndk/*) $(wildcard $(ANDROIDSDK)/ndk-bundle/*) )
BUILD_TOOLS?=$(lastword $(wildcard $(ANDROIDSDK)/build-tools/*) )

ifeq ($(NDK),)
ANDROIDSDK := ~/Android/Sdk
endif

ifeq ($(ANDROIDSDK),)
$(error ANDROIDSDK directory not found)
endif
ifeq ($(NDK),)
$(error NDK directory not found)
endif
ifeq ($(BUILD_TOOLS),)
$(error BUILD_TOOLS directory not found)
endif

# Android Tools Packaging and Debugging
ADB:=adb
AAPT:=$(BUILD_TOOLS)/aapt

# Android compilers and compiler flags
ANDROID_CC_ARM64:=$(NDK)/toolchains/llvm/prebuilt/$(OS_NAME)/bin/aarch64-linux-android$(ANDROIDVERSION)-clang
ANDROID_CC_ARM32:=$(NDK)/toolchains/llvm/prebuilt/$(OS_NAME)/bin/armv7a-linux-androideabi$(ANDROIDVERSION)-clang
ANDROID_CC_x86:=$(NDK)/toolchains/llvm/prebuilt/$(OS_NAME)/bin/i686-linux-android$(ANDROIDVERSION)-clang
ANDROID_CC_x86_64=$(NDK)/toolchains/llvm/prebuilt/$(OS_NAME)/bin/x86_64-linux-android$(ANDROIDVERSION)-clang

ANDROID_AR_ARM64:=$(NDK)/toolchains/llvm/prebuilt/$(OS_NAME)/bin/aarch64-linux-android-ar
ANDROID_AR_ARM32:=$(NDK)/toolchains/llvm/prebuilt/$(OS_NAME)/bin/arm-linux-androideabi-ar
ANDROID_AR_x86:=$(NDK)/toolchains/llvm/prebuilt/$(OS_NAME)/bin/i686-linux-android-ar
ANDROID_AR_x86_64=$(NDK)/toolchains/llvm/prebuilt/$(OS_NAME)/bin/x86_64-linux-android-ar


ANDROID_CFLAGS_ARM64:=-m64
ANDROID_CFLAGS_ARM32:=-mfloat-abi=softfp -m32
ANDROID_CFLAGS_x86:=-march=i686 -mtune=intel -mssse3 -mfpmath=sse -m32
ANDROID_CFLAGS_x86_64:=-march=x86-64 -msse4.2 -mpopcnt -m64 -mtune=intel
