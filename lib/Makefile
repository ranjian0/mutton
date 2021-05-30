include ../makefiles/OS.mk
include ../makefiles/Directories.mk
include ../makefiles/AndroidToolchain.mk

all: build_android build_desktop

# Build Android Static Library
ANDROID_LIB_NAME:=libMobile.a
ANDROID_LIB_SRCS:= $(NDK)/sources/android/native_app_glue/android_native_app_glue.c
ANDROID_LIB_SRCS+= $(LIB_DIR)/extern/glfm/src/glfm_platform_android.c
ANDROID_LIB_OBJS:=$(patsubst %.c,%.o,$(ANDROID_LIB_SRCS))

ANDROID_CFLAGS+= -I$(LIB_DIR)/extern/glfm/include -I$(NDK)/sources/android/native_app_glue -I$(NDK)/toolchains/llvm/prebuilt/$(OS_NAME)/sysroot/usr/include -I$(NDK)/toolchains/llvm/prebuilt/$(OS_NAME)/sysroot/usr/include/android -fPIC -DANDROIDVERSION=$(ANDROIDVERSION)

%.o: %.c
	@mkdir -p $(ANDROID_BUILD_DIR)/arm64-v8a
	$(ANDROID_CC_ARM64) $(ANDROID_CFLAGS) $(ANDROID_CFLAGS_ARM64) -c -o $(ANDROID_BUILD_DIR)/arm64-v8a/$(notdir $@) $^

	@mkdir -p $(ANDROID_BUILD_DIR)/armeabi-v7a
	$(ANDROID_CC_ARM32) $(ANDROID_CFLAGS) $(ANDROID_CFLAGS_ARM32) -c -o $(ANDROID_BUILD_DIR)/armeabi-v7a/$(notdir $@) $^

	@mkdir -p $(ANDROID_BUILD_DIR)/x86
	$(ANDROID_CC_x86) $(ANDROID_CFLAGS) $(ANDROID_CFLAGS_x86) -c -o $(ANDROID_BUILD_DIR)/x86/$(notdir $@) $^

	@mkdir -p $(ANDROID_BUILD_DIR)/x86_64
	$(ANDROID_CC_x86_64) $(ANDROID_CFLAGS) $(ANDROID_CFLAGS_x86_64) -c -o $(ANDROID_BUILD_DIR)/x86_64/$(notdir $@) $^ 


$(ANDROID_BUILD_DIR)/arm64-v8a/$(ANDROID_LIB_NAME) : $(ANDROID_LIB_OBJS)
	$(ANDROID_AR_ARM64)	rcs $@ $(addprefix $(ANDROID_BUILD_DIR)/arm64-v8a/, $(notdir $^))

$(ANDROID_BUILD_DIR)/armeabi-v7a/$(ANDROID_LIB_NAME) : $(ANDROID_LIB_OBJS)
	$(ANDROID_AR_ARM32)	rcs $@ $(addprefix $(ANDROID_BUILD_DIR)/armeabi-v7a/, $(notdir $^))

$(ANDROID_BUILD_DIR)/x86/$(ANDROID_LIB_NAME) : $(ANDROID_LIB_OBJS)
	$(ANDROID_AR_x86) rcs $@ $(addprefix $(ANDROID_BUILD_DIR)/x86/, $(notdir $^))

$(ANDROID_BUILD_DIR)/x86_64/$(ANDROID_LIB_NAME) : $(ANDROID_LIB_OBJS)
	$(ANDROID_AR_x86_64) rcs $@ $(addprefix $(ANDROID_BUILD_DIR)/x86_64/, $(notdir $^))


ANDROID_LIB_TARGETS += $(ANDROID_BUILD_DIR)/arm64-v8a/$(ANDROID_LIB_NAME)
ANDROID_LIB_TARGETS += $(ANDROID_BUILD_DIR)/armeabi-v7a/$(ANDROID_LIB_NAME)
ANDROID_LIB_TARGETS += $(ANDROID_BUILD_DIR)/x86/$(ANDROID_LIB_NAME)
ANDROID_LIB_TARGETS += $(ANDROID_BUILD_DIR)/x86_64/$(ANDROID_LIB_NAME)
build_android: $(ANDROID_LIB_TARGETS)

# Build Desktop Static Library
include ../makefiles/GLFW.mk 

DESKTOP_LIB_NAME:=libDesktop.a
DESKTOP_LIB_SRCS:=$(foreach src,$(GLFW_SRCS),$(LIB_DIR)/extern/glfw/src/$(src))
DESKTOP_LIB_SRCS+=$(shell find $(LIB_DIR)/extern/GLAD -name "*.c")

DESKTOP_LIB_OBJS:=$(patsubst %.c,%.o,$(DESKTOP_LIB_SRCS))

DESKTOP_CC=clang
DESKTOP_CFLAGS+= -I$(LIB_DIR)/extern/glfw/include -I$(LIB_DIR)/extern/GLAD #-fPIC
DESKTOP_CFLAGS+= $(GLFW_CFLAGS)

$(DESKTOP_BUILD_DIR)/$(DESKTOP_LIB_NAME) : desktop_objs
	@mkdir -p $(DESKTOP_BUILD_DIR)
	ar rcs $@ $(DESKTOP_LIB_OBJS)

desktop_objs:
	$(foreach src, $(DESKTOP_LIB_SRCS), $(DESKTOP_CC) $(DESKTOP_CFLAGS) -c -o $(basename $(src)).o $(src);)

DESKTOP_LIB_TARGETS += $(DESKTOP_BUILD_DIR)/$(DESKTOP_LIB_NAME)
build_desktop: $(DESKTOP_LIB_TARGETS)

clean:
	rm -f $(DESKTOP_LIB_OBJS)
