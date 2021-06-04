# Include this makefile in your project to build APKs
# You will need to define some things before hand:
# 1. APK information (APPNAME, PACKAGENAME, APKFILE)
# 2. Your custom c file sources in ANDROIDSRCS

# Example Makefile:
#
# APPNAME:= MyAPKName
# PACKAGENAME:= com.example.myapp
#
# ANDROIDSRCS:= app.c app_utils.c
#
# include makefiles/AndroidAPK.mk

include $(ROOT_DIR)/makefiles/OS.mk
include $(ROOT_DIR)/makefiles/Directories.mk 
include $(ROOT_DIR)/makefiles/AndroidToolchain.mk

# APK INFORMATION
APPNAME?=App
LABEL?=$(APPNAME)
APKFILE ?= $(APPNAME).apk
PACKAGENAME?=com.example.$(APPNAME)
ANDROID_FULLSCREEN?=y

# Android Cflags
ANDROID_CFLAGS?=-Wall #-ffunction-sections -fdata-sections -fvisibility=hidden
ANDROID_CFLAGS+=-Os -DANDROID -DAPPNAME=\"$(APPNAME)\"
ifeq (ANDROID_FULLSCREEN,y)
ANDROID_CFLAGS +=-DANDROID_FULLSCREEN
endif
ANDROID_CFLAGS+= -I$(ROOT_DIR)/extern/freetype/include -I$(ROOT_DIR)/extern/glfm/include -I$(ROOT_DIR)/include -I$(NDK)/sources/android/native_app_glue -I$(NDK)/toolchains/llvm/prebuilt/$(OS_NAME)/sysroot/usr/include -I$(NDK)/toolchains/llvm/prebuilt/$(OS_NAME)/sysroot/usr/include/android  -fPIC -DANDROIDVERSION=$(ANDROIDVERSION)


# Android ldflags
ANDROID_LDFLAGS?=-Wl,--gc-sections -s
ANDROID_LDFLAGS += -lm -lGLESv3 -lEGL -landroid -llog -lOpenSLES -lz
ANDROID_LDFLAGS += -lMobile # Our custom lib
ANDROID_LDFLAGS += -shared -uANativeActivity_onCreate

# Which binaries to build? Just comment/uncomment these lines:
ANDROID_TARGETS += build/apk/lib/arm64-v8a/lib$(APPNAME).so
ANDROID_TARGETS += build/apk/lib/armeabi-v7a/lib$(APPNAME).so
ANDROID_TARGETS += build/apk/lib/x86/lib$(APPNAME).so
ANDROID_TARGETS += build/apk/lib/x86_64/lib$(APPNAME).so


STOREPASS?=password
DNAME:="CN=example.com, OU=ID, O=Example, L=Doe, S=John, C=GB"
KEYSTOREFILE:=my-release-key.keystore
ALIASNAME?=standkey

# Show Android Configuration
show_android_config:
	@echo "SDK and NDK paths"
	@echo "SDK:\t\t" $(ANDROIDSDK)
	@echo "NDK:\t\t" $(NDK)
	@echo "Build Tools:\t" $(BUILD_TOOLS)
	@echo
	@echo "Sources"
	@echo $(ANDROIDSRCS)
	@echo
	@echo "Android Build Targets"
	@echo $(ANDROID_TARGETS)


# Setup android requirements

keystore : $(KEYSTOREFILE)

$(KEYSTOREFILE) :
	keytool -genkey -v -keystore $(KEYSTOREFILE) -alias $(ALIASNAME) -keyalg RSA -keysize 2048 -validity 10000 -storepass $(STOREPASS) -keypass $(STOREPASS) -dname $(DNAME)

manifest: AndroidManifest.xml

AndroidManifest.xml :

	rm -rf build/apk/AndroidManifest.xml
	PACKAGENAME=$(PACKAGENAME) \
		ANDROIDVERSION=$(ANDROIDVERSION) \
		ANDROIDTARGET=$(ANDROIDTARGET) \
		APPNAME=$(APPNAME) \
		LABEL=$(LABEL) envsubst '$$ANDROIDTARGET $$ANDROIDVERSION $$APPNAME $$PACKAGENAME $$LABEL' \
		< $(ROOT_DIR)/resources/AndroidManifest.xml.template > build/apk/AndroidManifest.xml


# Build android native library
folders:
	mkdir -p build/apk/lib/arm64-v8a
	mkdir -p build/apk/lib/armeabi-v7a
	mkdir -p build/apk/lib/x86
	mkdir -p build/apk/lib/x86_64

build/apk/lib/arm64-v8a/lib$(APPNAME).so : $(ANDROIDSRCS)
	mkdir -p build/apk/lib/arm64-v8a
	$(ANDROID_CC_ARM64) $(ANDROID_CFLAGS) $(ANDROID_CFLAGS_ARM64) -o $@ $^ -L$(BUILD_DIR)/android/arm64-v8a -L$(NDK)/toolchains/llvm/prebuilt/$(OS_NAME)/sysroot/usr/lib/aarch64-linux-android/$(ANDROIDVERSION) $(ANDROID_LDFLAGS)

build/apk/lib/armeabi-v7a/lib$(APPNAME).so : $(ANDROIDSRCS)
	mkdir -p build/apk/lib/armeabi-v7a
	$(ANDROID_CC_ARM32) $(ANDROID_CFLAGS) $(ANDROID_CFLAGS_ARM32) -o $@ $^ -L$(BUILD_DIR)/android/armeabi-v7a -L$(NDK)/toolchains/llvm/prebuilt/$(OS_NAME)/sysroot/usr/lib/arm-linux-androideabi/$(ANDROIDVERSION) $(ANDROID_LDFLAGS)

build/apk/lib/x86/lib$(APPNAME).so : $(ANDROIDSRCS)
	mkdir -p build/apk/lib/x86
	$(ANDROID_CC_x86) $(ANDROID_CFLAGS) $(ANDROID_CFLAGS_x86) -o $@ $^ -L$(BUILD_DIR)/android/x86 -L$(NDK)/toolchains/llvm/prebuilt/$(OS_NAME)/sysroot/usr/lib/i686-linux-android/$(ANDROIDVERSION) $(ANDROID_LDFLAGS)

build/apk/lib/x86_64/lib$(APPNAME).so : $(ANDROIDSRCS)
	mkdir -p build/apk/lib/x86_64
	$(ANDROID_CC_x86) $(ANDROID_CFLAGS) $(ANDROID_CFLAGS_x86_64) -o $@ $^ -L$(BUILD_DIR)/android/x86_64 -L$(NDK)/toolchains/llvm/prebuilt/$(OS_NAME)/sysroot/usr/lib/x86_64-linux-android/$(ANDROIDVERSION) $(ANDROID_LDFLAGS)


apk : $(ANDROID_TARGETS) AndroidManifest.xml keystore
	mkdir -p build/apk/assets
	cp -r src/assets/* build/apk/assets
	rm -rf build/temp.apk
	$(AAPT) package -f -F build/temp.apk -I $(ANDROIDSDK)/platforms/android-$(ANDROIDVERSION)/android.jar -M build/apk/AndroidManifest.xml -S src/res -A build/apk/assets -v --target-sdk-version $(ANDROIDTARGET)
	unzip -o build/temp.apk -d build/apk
	rm -rf build/apk.apk
	cd build/apk && zip -D9r ../apk.apk . && zip -D0r ../apk.apk ./resources.arsc ./AndroidManifest.xml
	jarsigner -sigalg SHA1withRSA -digestalg SHA1 -verbose -keystore $(KEYSTOREFILE) -storepass $(STOREPASS) build/apk.apk $(ALIASNAME)
	rm -rf build/$(APKFILE)
	$(BUILD_TOOLS)/zipalign -v 4 build/apk.apk build/$(APKFILE)
# Using the apksigner in this way is only required on Android 30+
	$(BUILD_TOOLS)/apksigner sign --key-pass pass:$(STOREPASS) --ks-pass pass:$(STOREPASS) --ks $(KEYSTOREFILE) build/$(APKFILE)
	rm -rf build/temp.apk
	rm -rf build/apk.apk
	@ls -lh build/$(APKFILE)


# Android run on device

uninstall : 
	($(ADB) uninstall $(PACKAGENAME))||true

push : apk
	@echo "Installing" $(PACKAGENAME)
	$(ADB) install -r build/$(APKFILE)

run : push
	$(eval ACTIVITYNAME:=$(shell $(AAPT) dump badging build/$(APKFILE) | grep "launchable-activity" | cut -f 2 -d"'"))
	$(ADB) shell am start -n $(PACKAGENAME)/$(ACTIVITYNAME)


